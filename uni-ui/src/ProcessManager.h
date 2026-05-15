#pragma once

#include <QHash>
#include <QJsonArray>
#include <QObject>
#include <QPointer>
#include <QString>

class QProcess;

// Owns all `uni` subprocesses (search, install, info, index, sources).
// One persistent search worker is reused per keystroke to avoid bash startup cost.
class ProcessManager : public QObject {
    Q_OBJECT
public:
    explicit ProcessManager(QObject *parent = nullptr);
    ~ProcessManager() override;

    void setUniPath(const QString &path);  // override for ui.conf
    QString uniPath() const { return m_uniPath; }

    // Async search via `uni search --cached <q>`. Latest call wins.
    void searchCached(const QString &query);

    // Build/refresh the local index if missing or older than 6h.
    void ensureIndexBuilt();

    // Start an install — caller subscribes to installEvent for the returned token.
    // Returns a unique token used as the package key in emitted events.
    QString install(const QString &package, const QString &source = QString());

    // Cancel an in-flight install (best-effort).
    void cancelInstall(const QString &token);

    // Read cached sources.json (sync, fast).
    QJsonObject readSourcesCache() const;

    // Trigger a benchmark refresh in the background. ranksUpdated fires when done.
    void refreshSourceBenchmark();

signals:
    void searchResults(const QJsonArray &results);
    void searchFailed(const QString &error);

    // package/stage/etc. — token == package field of the event.
    void installEvent(const QString &token, const QJsonObject &event);

    void sourcesUpdated(const QJsonObject &sources);

private:
    QProcess *spawnUni(const QStringList &args);

    QString m_uniPath = "uni";
    QPointer<QProcess> m_searchProc;
    QString m_pendingSearchOut;

    struct InstallProc {
        QPointer<QProcess> proc;
        QString package;
        QString buffer;
    };
    QHash<QString, InstallProc> m_installs;
};
