#include <QApplication>
#include "qmlapplicationviewer.h"

#include "starmenu.h"
#include "staritem.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;

    qmlRegisterType<StarMenu>("StarMenu", 1, 0, "StarMenu");
    qmlRegisterType<StarItem>("StarMenu", 1, 0, "StarItem");

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/variation00/main.qml"));
    //viewer.showExpanded();
    viewer.showFullScreen();

    return app->exec();
}
