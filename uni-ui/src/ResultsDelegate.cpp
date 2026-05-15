#include "ResultsDelegate.h"

#include "ResultsModel.h"
#include "Theme.h"

#include <QApplication>
#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>

namespace {
QString formatSize(qint64 bytes) {
    if (bytes <= 0) return QString();
    const double kb = bytes / 1024.0;
    if (kb < 1024.0) return QStringLiteral("%1 KB").arg(qRound(kb));
    const double mb = kb / 1024.0;
    if (mb < 1024.0) return QStringLiteral("%1 MB").arg(QString::number(mb, 'f', 1));
    return QStringLiteral("%1 GB").arg(QString::number(mb / 1024.0, 'f', 2));
}
} // namespace

ResultsDelegate::ResultsDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void ResultsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    const bool selected = option.state & QStyle::State_Selected;
    const bool hovered  = option.state & QStyle::State_MouseOver;

    QColor bg = Theme::Color::BgSurface;
    if (selected)      bg = Theme::Color::BgElevated;
    else if (hovered)  bg = Theme::Color::BgElevated.lighter(108);

    QRect r = option.rect.adjusted(0, 0, 0, -1);
    painter->fillRect(r, bg);
    painter->setPen(Theme::Color::Border);
    painter->drawLine(r.bottomLeft(), r.bottomRight());

    const QString name    = index.data(ResultsModel::NameRole).toString();
    const QString desc    = index.data(ResultsModel::DescriptionRole).toString();
    const QString version = index.data(ResultsModel::VersionRole).toString();
    const QString backend = index.data(ResultsModel::BackendRole).toString();
    const qint64  size    = index.data(ResultsModel::SizeBytesRole).toLongLong();
    const int     rank    = index.data(ResultsModel::SpeedRankRole).toInt();

    const int padX = 14;
    const int padY = 10;
    QRect content = r.adjusted(padX, padY, -padX, -padY);

    // Backend badge (top-right corner)
    QFont badgeFont = option.font;
    badgeFont.setBold(true);
    badgeFont.setPointSizeF(badgeFont.pointSizeF() - 1.0);
    painter->setFont(badgeFont);
    const QString badge = backend.toUpper();
    const QFontMetrics bm(badgeFont);
    const int bw = bm.horizontalAdvance(badge) + 16;
    const int bh = bm.height() + 4;
    QRect badgeRect(content.right() - bw, content.top(), bw, bh);
    QPainterPath bp;
    bp.addRoundedRect(badgeRect, 2, 2);
    painter->fillPath(bp, Theme::backendColor(backend));
    painter->setPen(Theme::Color::BgBase);
    painter->drawText(badgeRect, Qt::AlignCenter, badge);

    // Speed indicator (dot to the left of badge)
    const int dotR = 5;
    const int dotX = badgeRect.left() - dotR * 2 - 6;
    const int dotY = badgeRect.center().y();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Theme::speedColor(rank));
    painter->drawEllipse(QPoint(dotX, dotY), dotR, dotR);

    // Package name (monospace)
    QFont nameFont = option.font;
    nameFont.setFamily("IBM Plex Mono");
    nameFont.setPointSizeF(nameFont.pointSizeF() + 1.0);
    nameFont.setBold(true);
    painter->setFont(nameFont);
    painter->setPen(Theme::Color::TextPrimary);
    const int nameY = content.top() + QFontMetrics(nameFont).ascent();
    painter->drawText(content.left(), nameY, name);

    // Version (monospace, dim, after name)
    const int nameW = QFontMetrics(nameFont).horizontalAdvance(name);
    QFont verFont = nameFont;
    verFont.setBold(false);
    painter->setFont(verFont);
    painter->setPen(Theme::Color::TextMono);
    painter->drawText(content.left() + nameW + 10, nameY, version);

    // Description (secondary)
    QFont descFont = option.font;
    painter->setFont(descFont);
    painter->setPen(Theme::Color::TextSecondary);
    const int descY = nameY + 6 + QFontMetrics(descFont).ascent();
    const QString elidedDesc = QFontMetrics(descFont).elidedText(
        desc, Qt::ElideRight, content.width() - 100);
    painter->drawText(content.left(), descY, elidedDesc);

    // Size (bottom-right, mono dim)
    const QString sz = formatSize(size);
    if (!sz.isEmpty()) {
        QFont szFont = option.font;
        szFont.setFamily("IBM Plex Mono");
        painter->setFont(szFont);
        painter->setPen(Theme::Color::TextMono);
        painter->drawText(content, Qt::AlignRight | Qt::AlignBottom, sz);
    }

    painter->restore();
}

QSize ResultsDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &) const {
    Q_UNUSED(option);
    return QSize(0, 64);
}
