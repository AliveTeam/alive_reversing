#include "Exporter.hpp"
#include <QFileDialog>
#include <QUuid>
#include "ReliveApiWrapper.hpp"
#include "file_api.hpp"

bool exportJsonToLvl(QString jsonPath, QString lvlPath, QString partialTemporaryFilePath, std::function<void(const QString)> onFailure, std::set<std::string>& resourceSources, ReliveAPI::Context& context)
{
    EditorFileIO fileIo;
    auto fnExport = [&]()
    {
        resourceSources.insert(lvlPath.toStdString());
        std::vector<std::string> resourceSourcesVec;
        for (const auto& src : resourceSources)
        {
            resourceSourcesVec.emplace_back(src);
        }

        QUuid uuid = QUuid::createUuid();
        QString tempFileFullPath = QDir::toNativeSeparators(
            QDir::tempPath() + "/" +
            partialTemporaryFilePath +
            "_" +
            uuid.toString(QUuid::WithoutBraces) + ".lvl.tmp");

        // Export to a temp lvl file
        ReliveAPI::ImportPathJsonToBinary(
            fileIo,
            jsonPath.toStdString(),
            lvlPath.toStdString(),
            tempFileFullPath.toStdString(),
            resourceSourcesVec,
            context);

        // Then overwrite the original lvl with the temp one
        if (!QFile::remove(lvlPath))
        {
            onFailure("Failed to delete " + lvlPath + " in order to replace it with the updated lvl");
            QFile::remove(tempFileFullPath);
            return false;
        }

        if (!QFile::rename(tempFileFullPath, lvlPath))
        {
            onFailure("Failed to rename from " + tempFileFullPath + " to " + lvlPath);
            return false;
        }

        return true;
    };

    return ExecApiCall(fnExport, onFailure);
}
