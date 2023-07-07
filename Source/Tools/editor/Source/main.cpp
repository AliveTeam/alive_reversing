#include "EditorMainWindow.hpp"
#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QDebug>
#include "Exporter.hpp"
#include <functional>
#include <QtCore/qcommandlineparser.h>
#include "ReliveApiWrapper.hpp"

void DoMapSizeTests();

static int exportJsonToLvlCommandLine(const QStringList& args)
{
    if (args.size() != 2)
    {
        std::cerr << "Incorrect usage of the --export option, should be --export source dest" << std::endl;
        return 1;
    }
    const QString source = args.at(0);
    const QString destination = args.at(1);
    int runResult = 0;

    ReliveAPI::Context context;
    std::set<std::string> resourceSources;
    exportJsonToLvl(source, destination, "relive_export", [&runResult](const QString& text) mutable
        {
            std::cerr << "Exporting failed. " << text << std::endl;
            runResult = 1;
        }, resourceSources, context);

    if (!context.Ok())
    {
        for (const auto& remapped : context.RemappedEnumValues())
        {
            std::cout << "Enum value " << remapped.mEnumValueInJson << " remapped to " << remapped.mValueUsed << " for enum type " << remapped.mEnumTypeName << std::endl;
        }

        for (const auto& missingProperty : context.MissingJsonProperties())
        {
            std::cout << "Property " << missingProperty.mPropertyName << " was not found in type " << missingProperty.mStructureTypeName << std::endl;
        }
        // TODO: there are other context warnings to show
    }
    return runResult;
}

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

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file."));
    parser.addPositionalArgument("dest", QCoreApplication::translate("main", "Destination file."));

    QCommandLineOption exportJsonToLvlOption("export", QCoreApplication::translate("main", "Export the .json file to the .lvl file. Usage: --export source dest"));
    parser.addOption(exportJsonToLvlOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    if (parser.isSet(exportJsonToLvlOption))
    {
        return exportJsonToLvlCommandLine(args);
    }

    EditorMainWindow w;

    app.setWindowIcon(QIcon(":/icons/rsc/icons/icon.png"));
    w.setWindowIcon(QIcon(":/icons/rsc/icons/icon.png"));

    w.show();
    return app.exec();
}
