#include "ProcessManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QUuid>

ProcessManager::ProcessManager(QObject *parent) : QObject(parent) {
    // Best-effort: prefer ~/.local/bin/uni, fall back to PATH lookup.
    const QString localBin = QDir::homePath() + "/.local/bin/uni";
    if (QFileInfo(localBin).isExecutable())
        m_uniPath = localBin;
}

ProcessManager::~ProcessManager() {
    if (m_searchProc) m_searchProc->kill();
    for (auto &ip : m_installs)
        if (ip.proc) ip.proc->kill();
}

void ProcessManager::setUniPath(const QString &path) {
    if (!path.isEmpty()) m_uniPath = path;
}

QProcess *ProcessManager::spawnUni(const QStringList &args) {
    auto *p = new QProcess(this);
    p->setProcessChannelMode(QProcess::SeparateChannels);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("NO_COLOR", "1"); // strip ANSI from any stderr noise
    p->setProcessEnvironment(env);
    p->setProgram(m_uniPath);
    p->setArguments(args);
    return p;
}

void ProcessManager::searchCached(const QString &query) {
    if (m_searchProc) {
        m_searchProc->disconnect(this);
        m_searchProc->kill();
        m_searchProc->deleteLater();
        m_searchProc = nullptr;
    }
    m_pendingSearchOut.clear();

    auto *p = spawnUni({"search", "--cached", query});
    m_searchProc = p;

    connect(p, &QProcess::readyReadStandardOutput, this, [this, p] {
        m_pendingSearchOut += QString::fromUtf8(p->readAllStandardOutput());
    });
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, p](int code, QProcess::ExitStatus) {
        if (p != m_searchProc) { p->deleteLater(); return; }
        if (code != 0) {
            emit searchFailed(QString::fromUtf8(p->readAllStandardError()));
        } else {
            QJsonParseError err{};
            const auto doc = QJsonDocument::fromJson(m_pendingSearchOut.toUtf8(), &err);
            if (err.error == QJsonParseError::NoError && doc.isArray())
                emit searchResults(doc.array());
            else
                emit searchFailed(err.errorString());
        }
        p->deleteLater();
        m_searchProc = nullptr;
        m_pendingSearchOut.clear();
    });

    p->start();
}

void ProcessManager::ensureIndexBuilt() {
    // Cheap check: does index.db exist? If not, kick off `uni index --refresh`.
    const QString idx = QDir::homePath() + "/.local/share/uni/index.db";
    if (QFile::exists(idx)) return;
    auto *p = spawnUni({"index", "--refresh"});
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            p, &QObject::deleteLater);
    p->start();
}

QString ProcessManager::install(const QString &package, const QString &source) {
    const QString token = package; // demux key in emitted events
    QStringList args = {"install", "--json"};
    if (!source.isEmpty()) args << "--source" << source;
    args << package;

    auto *p = spawnUni(args);
    InstallProc rec;
    rec.proc = p;
    rec.package = package;
    m_installs.insert(token, rec);

    connect(p, &QProcess::readyReadStandardOutput, this, [this, p, token] {
        auto it = m_installs.find(token);
        if (it == m_installs.end()) return;
        it->buffer += QString::fromUtf8(p->readAllStandardOutput());
        int nl;
        while ((nl = it->buffer.indexOf('\n')) >= 0) {
            const QString line = it->buffer.left(nl).trimmed();
            it->buffer.remove(0, nl + 1);
            if (line.isEmpty()) continue;
            QJsonParseError err{};
            const auto doc = QJsonDocument::fromJson(line.toUtf8(), &err);
            if (err.error == QJsonParseError::NoError && doc.isObject())
                emit installEvent(token, doc.object());
        }
    });

    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, p, token](int code, QProcess::ExitStatus) {
        // Synthesize a terminal event if the script died without emitting one.
        QJsonObject closing;
        closing["package"] = token;
        closing["stage"]   = "done";
        closing["success"] = (code == 0);
        if (code != 0) {
            closing["error"] = QString::fromUtf8(p->readAllStandardError()).left(500);
        }
        emit installEvent(token, closing);
        m_installs.remove(token);
        p->deleteLater();
    });

    p->start();
    return token;
}

void ProcessManager::cancelInstall(const QString &token) {
    auto it = m_installs.find(token);
    if (it == m_installs.end() || !it->proc) return;
    it->proc->terminate();
    if (!it->proc->waitForFinished(1000))
        it->proc->kill();
}

QJsonObject ProcessManager::readSourcesCache() const {
    const QString path = QDir::homePath() + "/.local/share/uni/sources.json";
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return {};
    QJsonParseError err{};
    const auto doc = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) return {};
    return doc.object();
}

void ProcessManager::refreshSourceBenchmark() {
    auto *p = spawnUni({"sources", "--benchmark"});
    QString *buf = new QString();
    connect(p, &QProcess::readyReadStandardOutput, this,
            [p, buf] { *buf += QString::fromUtf8(p->readAllStandardOutput()); });
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, p, buf](int code, QProcess::ExitStatus) {
        if (code == 0) {
            QJsonParseError err{};
            const auto doc = QJsonDocument::fromJson(buf->toUtf8(), &err);
            if (err.error == QJsonParseError::NoError && doc.isObject())
                emit sourcesUpdated(doc.object());
        }
        delete buf;
        p->deleteLater();
    });
    p->start();
}
