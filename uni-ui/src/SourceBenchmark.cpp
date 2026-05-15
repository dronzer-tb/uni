#include "SourceBenchmark.h"

#include "ProcessManager.h"

SourceBenchmark::SourceBenchmark(ProcessManager *pm, QObject *parent)
    : QObject(parent), m_pm(pm) {
    connect(m_pm, &ProcessManager::sourcesUpdated,
            this, &SourceBenchmark::onSourcesUpdated);
}

void SourceBenchmark::refresh() {
    // 1) immediate apply from cache (if any) so the UI has something to work with
    apply(m_pm->readSourcesCache());
    // 2) kick off a fresh benchmark in the background
    m_pm->refreshSourceBenchmark();
}

void SourceBenchmark::onSourcesUpdated(const QJsonObject &sources) {
    apply(sources);
}

void SourceBenchmark::apply(const QJsonObject &sources) {
    if (sources.isEmpty()) return;
    m_ranks.clear();
    m_speeds.clear();
    m_latencies.clear();
    for (auto it = sources.constBegin(); it != sources.constEnd(); ++it) {
        const QJsonObject o = it.value().toObject();
        m_ranks.insert(it.key(),    o.value("rank").toInt(99));
        m_speeds.insert(it.key(),   o.value("speed_mbps").toDouble(0.0));
        m_latencies.insert(it.key(), o.value("latency_ms").toInt(0));
    }
    emit ranksUpdated();
}
