#pragma once

#include <QFileSystemWatcher>
#include <QObject>
#include <QString>

// Watches ~/.local/share/uni/index.db for changes and emits indexUpdated.
// The bash CLI rewrites the index in WAL mode; this gives the UI a hook
// to refresh the current query without polling.
class IndexWatcher : public QObject {
    Q_OBJECT
public:
    explicit IndexWatcher(QObject *parent = nullptr);

    void start();
    QString indexPath() const { return m_indexPath; }

signals:
    void indexUpdated();

private:
    QFileSystemWatcher m_watcher;
    QString            m_indexPath;
};
