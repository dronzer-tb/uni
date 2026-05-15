#pragma once

#include <QApplication>
#include <QColor>
#include <QString>

namespace Theme {

// Palette tokens — keep in sync with resources/styles/uni-ui.qss
namespace Color {
    inline const QColor BgBase      {"#0d0f12"};
    inline const QColor BgSurface   {"#141720"};
    inline const QColor BgElevated  {"#1c2030"};
    inline const QColor Border      {"#252a38"};
    inline const QColor TextPrimary {"#e2e8f0"};
    inline const QColor TextSecondary{"#64748b"};
    inline const QColor TextMono    {"#94a3b8"};

    inline const QColor AccentApt    {"#f97316"};
    inline const QColor AccentFlatpak{"#6366f1"};
    inline const QColor AccentSnap   {"#e11d48"};
    inline const QColor AccentGithub {"#22d3ee"};

    inline const QColor SpeedFast {"#22c55e"};
    inline const QColor SpeedMid  {"#eab308"};
    inline const QColor SpeedSlow {"#ef4444"};
}

void apply(QApplication &app);
QColor backendColor(const QString &backend);
QColor speedColor(int rank);

} // namespace Theme
