#pragma once

#include <QStyledItemDelegate>

class ResultsDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ResultsDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};
