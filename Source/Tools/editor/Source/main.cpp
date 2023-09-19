#include "EditorMainWindow.hpp"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDebug>
#include <functional>
#include <QtCore/qcommandlineparser.h>
#include "ReliveApiWrapper.hpp"

void DoMapSizeTests();

int main(int argc, char *argv[])
{
    DoMapSizeTests();

    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
        && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
        && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
        && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS"))
    {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    QTranslator translator;

    if (!translator.load("qt-editor_German"))
    {
        qDebug() << "Translator load failed";
    }

    QApplication app(argc, argv);
    app.installTranslator(&translator);

    EditorMainWindow w;

    app.setWindowIcon(QIcon(":/icons/rsc/icons/icon.png"));
    w.setWindowIcon(QIcon(":/icons/rsc/icons/icon.png"));

    w.show();
    return app.exec();
}
