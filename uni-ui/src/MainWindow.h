#pragma once

#include <QMainWindow>

class SearchBar;
class ResultsModel;
class ProcessManager;
class PackageDetailPane;
class DownloadQueue;
class IndexWatcher;
class SourceBenchmark;
class QListView;
class QLabel;
class QStackedWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onQueryChanged(const QString &query);
    void onSearchResults(const QJsonArray &results);
    void onResultSelected(const QModelIndex &index);
    void onBenchmarkUpdated();

private:
    void buildUi();
    void wireSignals();
    void kickoffStartupTasks();

    SearchBar         *m_searchBar      = nullptr;
    QListView         *m_resultsView    = nullptr;
    ResultsModel      *m_resultsModel   = nullptr;
    ProcessManager    *m_processManager = nullptr;
    PackageDetailPane *m_detailPane     = nullptr;
    DownloadQueue     *m_downloadQueue  = nullptr;
    IndexWatcher      *m_indexWatcher   = nullptr;
    SourceBenchmark   *m_sourceBenchmark = nullptr;
    QLabel            *m_statusLabel    = nullptr;
};
