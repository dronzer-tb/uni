#include "IndexWatcher.h"

#include <QDir>
#include <QFileInfo>

IndexWatcher::IndexWatcher(QObject *parent) : QObject(parent) {
    m_indexPath = QDir::homePath() + "/.local/share/uni/index.db";
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, [this](const QString &) { emit indexUpdated(); });
}

void IndexWatcher::start() {
    if (QFileInfo::exists(m_indexPath))
        m_watcher.addPath(m_indexPath);
    // If the db doesn't exist yet, MainWindow's ensureIndexBuilt() will create it
    // and ProcessManager will emit when ready; we can add the path then.
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, [this](const QString &) {
                if (QFileInfo::exists(m_indexPath) && !m_watcher.files().contains(m_indexPath))
                    m_watcher.addPath(m_indexPath);
            });
    m_watcher.addPath(QDir::homePath() + "/.local/share/uni");
}
