#pragma once

#include "ReliveApiWrapper.hpp"
#include <QMessageBox>

inline void ShowContext(const ReliveAPI::Context& context)
{
    // TODO: Should be a dialog showing the source file of each warning
    QString fatMessage;
    for (const auto& remapped : context.RemappedEnumValues())
    {
        QString tmp;
        tmp = "Enum value " + QString(remapped.mEnumValueInJson.c_str()) + QString(" remapped to ") + QString(remapped.mValueUsed.c_str()) + QString(" for enum type ") + QString(remapped.mEnumTypeName.c_str()) + QString("\n");
        fatMessage += tmp;
    }

    for (const auto& missingProperty : context.MissingJsonProperties())
    {
        QString tmp;
        tmp = QString("Property ") + QString(missingProperty.mPropertyName.c_str()) + QString(" was not found in type ") + QString(missingProperty.mStructureTypeName.c_str()) + QString("\n");
        fatMessage += tmp;
    }

    for (const auto& missingLvlFile : context.MissingLvlFilesForCams())
    {
        QString tmp;
        tmp = QString("File ") + QString(missingLvlFile.c_str()) + QString(" couldn't be opened for camera resources as it was not found\n");
        fatMessage += tmp;
    }

    for (const auto& missingLvlFile : context.MissingLvlFiles())
    {
        QString tmp;
        tmp = QString("File ") + QString(missingLvlFile.c_str()) + QString(" couldn't be added to the LVL as it was not found in any source LVL\n");
        fatMessage += tmp;
    }

    for (const auto& missingCamResource : context.MissingCamResources())
    {
        QString tmp;
        tmp = QString("File ") + QString(missingCamResource.first.c_str()) + QString(" couldn't be added to a camera in the LVL as it was not found in any source LVL\n");
        fatMessage += tmp;
    }

    for (const auto& missingCamResource : context.SourceLvlOpenFailures())
    {
        QString tmp;
        tmp = QString("LVL file ") + QString(missingCamResource.c_str()) + QString(" couldn't be opened to use as a resource source.\n");
        fatMessage += tmp;
    }

    QMessageBox::warning(nullptr, "Context warnings", fatMessage);

}
