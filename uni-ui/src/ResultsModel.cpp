#include "ResultsModel.h"

ResultsModel::ResultsModel(QObject *parent) : QAbstractListModel(parent) {}

int ResultsModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_results.size();
}

QVariant ResultsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_results.size())
        return {};
    const QJsonObject pkg = m_results.at(index.row()).toObject();

    switch (role) {
    case Qt::DisplayRole:
    case NameRole:        return pkg.value("name").toString();
    case DescriptionRole: return pkg.value("description").toString();
    case VersionRole:     return pkg.value("version").toString();
    case BackendRole:     return pkg.value("backend").toString();
    case SourceUrlRole:   return pkg.value("source_url").toString();
    case SizeBytesRole:   return pkg.value("size_bytes").toVariant().toLongLong();
    case SpeedRankRole: {
        const auto v = pkg.value("source_speed_rank");
        if (v.isDouble()) return v.toInt();
        // Fallback: use the per-backend rank map.
        return m_ranks.value(pkg.value("backend").toString(), 99);
    }
    case PackageJsonRole: return pkg;
    }
    return {};
}

QHash<int, QByteArray> ResultsModel::roleNames() const {
    auto r = QAbstractListModel::roleNames();
    r.insert(NameRole,        "name");
    r.insert(DescriptionRole, "description");
    r.insert(VersionRole,     "version");
    r.insert(BackendRole,     "backend");
    r.insert(SourceUrlRole,   "source_url");
    r.insert(SizeBytesRole,   "size_bytes");
    r.insert(SpeedRankRole,   "source_speed_rank");
    r.insert(PackageJsonRole, "package_json");
    return r;
}

void ResultsModel::clear() {
    if (m_results.isEmpty()) return;
    beginResetModel();
    m_results = {};
    endResetModel();
}

void ResultsModel::setResults(const QJsonArray &results) {
    beginResetModel();
    m_results = results;
    endResetModel();
}

void ResultsModel::setSourceRanks(const QMap<QString, int> &ranks) {
    m_ranks = ranks;
    if (m_results.isEmpty()) return;
    emit dataChanged(index(0), index(m_results.size() - 1), {SpeedRankRole});
}

QJsonObject ResultsModel::packageAt(int row) const {
    if (row < 0 || row >= m_results.size()) return {};
    return m_results.at(row).toObject();
}
