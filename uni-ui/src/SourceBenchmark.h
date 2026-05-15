#pragma once

#include <QJsonObject>
#include <QMap>
#include <QObject>

class ProcessManager;

// Drives `uni sources --benchmark` and exposes per-backend ranks.
// On refresh(), first uses the cached sources.json if present, then
// triggers a fresh benchmark in the background.
class SourceBenchmark : public QObject {
    Q_OBJECT
public:
    explicit SourceBenchmark(ProcessManager *pm, QObject *parent = nullptr);

    void refresh();
    QMap<QString, int>     ranks()    const { return m_ranks;    }
    QMap<QString, double>  speeds()   const { return m_speeds;   }
    QMap<QString, int>     latencies() const { return m_latencies; }

signals:
    void ranksUpdated();

private slots:
    void onSourcesUpdated(const QJsonObject &sources);

private:
    void apply(const QJsonObject &sources);

    ProcessManager        *m_pm = nullptr;
    QMap<QString, int>     m_ranks;
    QMap<QString, double>  m_speeds;
    QMap<QString, int>     m_latencies;
};
