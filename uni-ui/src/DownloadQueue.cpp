#include "DownloadQueue.h"

#include "ProcessManager.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

DownloadQueue::DownloadQueue(QWidget *parent) : QWidget(parent) {
    setObjectName("DownloadQueue");
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(6);
    updateVisibility();
}

void DownloadQueue::enqueueInstall(ProcessManager *pm, const QString &pkg, const QString &source) {
    if (!pm || pkg.isEmpty()) return;

    DownloadEntry entry = buildCard(pkg);
    entry.backend = source;
    m_active.insert(pkg, entry);
    m_layout->addWidget(entry.card);
    updateVisibility();

    // Connect once per pm — but our enqueue can fire multiple times so use UniqueConnection.
    connect(pm, &ProcessManager::installEvent,
            this, &DownloadQueue::onInstallEvent,
            Qt::UniqueConnection);

    pm->install(pkg, source);
}

DownloadEntry DownloadQueue::buildCard(const QString &pkg) {
    DownloadEntry e;
    e.card = new QWidget(this);
    e.card->setObjectName("DownloadCard");

    auto *row = new QHBoxLayout(e.card);
    row->setContentsMargins(10, 6, 10, 6);
    row->setSpacing(10);

    e.title = new QLabel(pkg, e.card);
    e.title->setObjectName("DownloadTitle");

    e.bar = new QProgressBar(e.card);
    e.bar->setRange(0, 0); // indeterminate until we get progress events
    e.bar->setTextVisible(false);
    e.bar->setFixedHeight(8);

    e.status = new QLabel("Resolving…", e.card);
    e.status->setObjectName("DownloadStatus");

    row->addWidget(e.title, 0);
    row->addWidget(e.bar, 1);
    row->addWidget(e.status, 0);
    return e;
}

void DownloadQueue::onInstallEvent(const QString &token, const QJsonObject &event) {
    auto it = m_active.find(token);
    if (it == m_active.end()) return;

    const QString stage = event.value("stage").toString();
    if (stage == "resolving") {
        it->status->setText("Resolving…");
    } else if (stage == "downloading") {
        it->status->setText("Downloading…");
        const double prog = event.value("progress").toDouble(-1);
        if (prog >= 0.0) {
            it->bar->setRange(0, 100);
            it->bar->setValue(int(prog * 100));
        }
    } else if (stage == "installing") {
        it->bar->setRange(0, 0);
        it->status->setText("Installing…");
    } else if (stage == "done") {
        const bool ok = event.value("success").toBool();
        it->bar->setRange(0, 100);
        it->bar->setValue(100);
        it->status->setText(ok ? "Done ✓" : "Failed ✗");
        QString key = token;
        QWidget *card = it->card;
        m_active.erase(it);
        QTimer::singleShot(ok ? 3000 : 8000, this, [this, card] {
            if (card) card->deleteLater();
            updateVisibility();
        });
    }
}

void DownloadQueue::updateVisibility() {
    setVisible(!m_active.isEmpty());
}
