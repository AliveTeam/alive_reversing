#pragma once

#define ADD_UPGRADE_STEP_FROM(version, upgradeObjType) mUpgraders[version] = []() -> std::unique_ptr<IJsonUpgrader> { return std::make_unique<upgradeObjType>(); }
