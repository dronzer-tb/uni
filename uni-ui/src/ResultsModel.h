#pragma once

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>

class ResultsModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        VersionRole,
        BackendRole,
        SourceUrlRole,
        SizeBytesRole,
        SpeedRankRole,
        PackageJsonRole,
    };

    explicit ResultsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void setResults(const QJsonArray &results);
    void setSourceRanks(const QMap<QString, int> &ranks);

    QJsonObject packageAt(int row) const;

private:
    QJsonArray m_results;
    QMap<QString, int> m_ranks;
};
