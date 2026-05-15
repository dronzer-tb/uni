#include "MainWindow.h"

#include "DownloadQueue.h"
#include "IndexWatcher.h"
#include "PackageDetailPane.h"
#include "ProcessManager.h"
#include "ResultsDelegate.h"
#include "ResultsModel.h"
#include "SearchBar.h"
#include "SourceBenchmark.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QListView>
#include <QShortcut>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("uni — Universal Package Manager");
    resize(1200, 760);
    buildUi();
    wireSignals();
    kickoffStartupTasks();
    m_searchBar->setFocus();
}

MainWindow::~MainWindow() = default;

void MainWindow::buildUi() {
    auto *central = new QWidget(this);
    auto *root    = new QVBoxLayout(central);
    root->setContentsMargins(16, 16, 16, 0);
    root->setSpacing(12);

    m_searchBar     = new SearchBar(central);
    m_resultsModel  = new ResultsModel(this);
    m_resultsView   = new QListView(central);
    m_resultsView->setModel(m_resultsModel);
    m_resultsView->setItemDelegate(new ResultsDelegate(m_resultsView));
    m_resultsView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_resultsView->setUniformItemSizes(true);
    m_resultsView->setObjectName("ResultsView");

    m_detailPane = new PackageDetailPane(central);
    m_detailPane->setMinimumWidth(360);
    m_detailPane->hide();

    auto *splitter = new QSplitter(Qt::Horizontal, central);
    splitter->addWidget(m_resultsView);
    splitter->addWidget(m_detailPane);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 2);
    splitter->setChildrenCollapsible(false);

    root->addWidget(m_searchBar);
    root->addWidget(splitter, 1);

    m_downloadQueue = new DownloadQueue(central);
    root->addWidget(m_downloadQueue);

    setCentralWidget(central);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName("StatusLabel");
    statusBar()->addPermanentWidget(m_statusLabel);
    statusBar()->setSizeGripEnabled(false);

    // Esc clears search and focus
    auto *escShortcut = new QShortcut(QKeySequence("Esc"), this);
    connect(escShortcut, &QShortcut::activated, this, [this] {
        m_searchBar->clear();
        m_searchBar->setFocus();
    });
}

void MainWindow::wireSignals() {
    m_processManager = new ProcessManager(this);

    connect(m_searchBar, &SearchBar::queryChanged,
            this, &MainWindow::onQueryChanged);

    connect(m_processManager, &ProcessManager::searchResults,
            this, &MainWindow::onSearchResults);

    connect(m_resultsView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, [this](const QModelIndex &current, const QModelIndex &) {
                onResultSelected(current);
            });

    m_indexWatcher = new IndexWatcher(this);
    connect(m_indexWatcher, &IndexWatcher::indexUpdated,
            this, [this] {
                // Re-run the current query so users see fresh results live.
                if (m_searchBar)
                    onQueryChanged(m_searchBar->text());
            });

    m_sourceBenchmark = new SourceBenchmark(m_processManager, this);
    connect(m_sourceBenchmark, &SourceBenchmark::ranksUpdated,
            this, &MainWindow::onBenchmarkUpdated);

    connect(m_detailPane, &PackageDetailPane::installRequested,
            this, [this](const QString &pkg, const QString &source) {
                m_downloadQueue->enqueueInstall(m_processManager, pkg, source);
            });
}

void MainWindow::kickoffStartupTasks() {
    m_indexWatcher->start();
    m_sourceBenchmark->refresh(); // reads cached sources.json or triggers a benchmark
    m_processManager->ensureIndexBuilt();
    m_statusLabel->setText(QStringLiteral("Ready"));
}

void MainWindow::onQueryChanged(const QString &query) {
    if (query.trimmed().isEmpty()) {
        m_resultsModel->clear();
        m_statusLabel->setText(QStringLiteral("Ready"));
        return;
    }
    m_statusLabel->setText(QStringLiteral("Searching \"%1\"…").arg(query));
    m_processManager->searchCached(query);
}

void MainWindow::onSearchResults(const QJsonArray &results) {
    m_resultsModel->setResults(results);
    m_statusLabel->setText(QStringLiteral("%1 results").arg(results.size()));
}

void MainWindow::onResultSelected(const QModelIndex &index) {
    if (!index.isValid()) {
        m_detailPane->hide();
        return;
    }
    const QJsonObject pkg = m_resultsModel->packageAt(index.row());
    m_detailPane->setPackage(pkg);
    m_detailPane->show();
}

void MainWindow::onBenchmarkUpdated() {
    m_resultsModel->setSourceRanks(m_sourceBenchmark->ranks());
}
