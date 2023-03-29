//
// Created by Filip on 07.09.2022.
//

#ifndef CHECKER_MAINGROUPSETTINGS_H
#define CHECKER_MAINGROUPSETTINGS_H

#include "constDeclarations.h"
#include "GroupSettings.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class MainGroupSettings {
    const std::string settingsFile = appDataDir.toStdString() + "mainGroups.json" ;
    json data = json();
    bool isOpened = false;
public:
    bool load();
    const json &getGroupObject();
    void addNewGroup(const QString &groupName, const QString &groupPath, const QString &groupParent);
    void editGroup(int id, const QString &groupName, const QString &groupPath, const QString &groupParent);
    void deleteGroup(int id);
    void saveToFile();
};


#endif //CHECKER_MAINGROUPSETTINGS_H
