#pragma once

#include <QHash>
#include <QJsonObject>
#include <QWidget>

class ProcessManager;
class QVBoxLayout;
class QLabel;
class QProgressBar;
class QPushButton;

struct DownloadEntry {
    QWidget      *card    = nullptr;
    QLabel       *title   = nullptr;
    QLabel       *status  = nullptr;
    QProgressBar *bar     = nullptr;
    QString       backend;
};

class DownloadQueue : public QWidget {
    Q_OBJECT
public:
    explicit DownloadQueue(QWidget *parent = nullptr);

    // Starts an install via the given ProcessManager and adds a row to the queue.
    void enqueueInstall(ProcessManager *pm, const QString &pkg, const QString &source);

private slots:
    void onInstallEvent(const QString &token, const QJsonObject &event);

private:
    DownloadEntry buildCard(const QString &pkg);
    void updateVisibility();

    QVBoxLayout                  *m_layout = nullptr;
    QHash<QString, DownloadEntry> m_active;
};
