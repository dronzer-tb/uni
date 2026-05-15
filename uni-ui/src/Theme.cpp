#include "Theme.h"

#include <QFile>
#include <QFontDatabase>
#include <QPalette>

namespace Theme {

void apply(QApplication &app) {
    QPalette p = app.palette();
    p.setColor(QPalette::Window,        Color::BgBase);
    p.setColor(QPalette::WindowText,    Color::TextPrimary);
    p.setColor(QPalette::Base,          Color::BgSurface);
    p.setColor(QPalette::AlternateBase, Color::BgElevated);
    p.setColor(QPalette::Text,          Color::TextPrimary);
    p.setColor(QPalette::Button,        Color::BgSurface);
    p.setColor(QPalette::ButtonText,    Color::TextPrimary);
    p.setColor(QPalette::Highlight,     Color::BgElevated);
    p.setColor(QPalette::HighlightedText, Color::TextPrimary);
    p.setColor(QPalette::PlaceholderText, Color::TextSecondary);
    app.setPalette(p);

    QFile qss(":/resources/styles/uni-ui.qss");
    if (qss.open(QIODevice::ReadOnly)) {
        app.setStyleSheet(QString::fromUtf8(qss.readAll()));
    }
}

QColor backendColor(const QString &backend) {
    const QString b = backend.toLower();
    if (b == "apt")     return Color::AccentApt;
    if (b == "flatpak") return Color::AccentFlatpak;
    if (b == "snap")    return Color::AccentSnap;
    if (b == "github")  return Color::AccentGithub;
    return Color::TextSecondary;
}

QColor speedColor(int rank) {
    if (rank <= 1) return Color::SpeedFast;
    if (rank <= 2) return Color::SpeedMid;
    return Color::SpeedSlow;
}

} // namespace Theme
