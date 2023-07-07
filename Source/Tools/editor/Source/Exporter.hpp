#pragma once

#include <functional>
#include <QString>
#include <set>

namespace ReliveAPI
{
    class Context;
}

bool exportJsonToLvl(QString jsonPath, QString lvlPath, QString partialTemporaryFilePath, std::function<void(const QString)> onFailure, std::set<std::string>& resourceSources, ReliveAPI::Context& context);
