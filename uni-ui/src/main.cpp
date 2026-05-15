#include "MainWindow.h"
#include "Theme.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("uni-ui");
    app.setApplicationVersion(UNI_UI_VERSION);
    app.setOrganizationName("DronzerStudios");
    app.setDesktopFileName("uni");

    Theme::apply(app);

    MainWindow w;
    w.show();
    return app.exec();
}
