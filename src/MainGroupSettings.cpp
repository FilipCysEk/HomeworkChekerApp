//
// Created by Filip on 07.09.2022.
//

#include "MainGroupSettings.h"

bool MainGroupSettings::load() {
    std::ifstream f(settingsFile);
    if (f.is_open()) {
        data = json::parse(f);

        isOpened = true;
        return true;
    }
    isOpened = false;
    return false;
}

const json &MainGroupSettings::getGroupObject() {
    if (!isOpened)
        return {};

    return data["mainGroups"];
}

void MainGroupSettings::addNewGroup(const QString &groupName, const QString &groupPath, const QString &groupParent) {
    int id;
    if (data.contains("lastId")) {
        data["lastId"].get_to(id);
        ++id;
    } else {
        id = 0;
    }
    data["lastId"] = id;

    json newObject;
    newObject["id"] = id;
    newObject["name"] = groupName.toStdString();
    newObject["path"] = groupPath.toStdString();
    newObject["parent"] = groupParent.toStdString();

    data["mainGroups"].push_back(newObject);

    GroupSettings gs(id);
    gs.createMainGroup(id);
    gs.saveToFile();
}

void MainGroupSettings::saveToFile() {
    std::ofstream f(settingsFile);
    f << data;
    f.close();
}

void
MainGroupSettings::editGroup(int id, const QString &groupName, const QString &groupPath, const QString &groupParent) {
    for (auto &group: data["mainGroups"]) {
        if (group["id"] == id) {
            group["name"] = groupName.toStdString();
            group["parent"] = groupParent.toStdString();
            group["path"] = groupPath.toStdString();
            break;
        }
    }
}

void MainGroupSettings::deleteGroup(int id) {
    int i = 0;
    for (; i < data["mainGroups"].size(); ++i) {
        if (data["mainGroups"][i]["id"] == id) {
            break;
        }
    }
    data["mainGroups"].erase(i);

    GroupSettings gs(id);
    gs.deleteGroup();
}
