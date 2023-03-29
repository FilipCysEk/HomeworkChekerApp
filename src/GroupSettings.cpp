//
// Created by Filip on 21.09.2022.
//

#include "GroupSettings.h"

bool GroupSettings::loadGroup() {
    std::ifstream f(settingsFile);
    if (f.is_open()) {
        data = json::parse(f);

        isOpened = true;
        return true;
    }
    isOpened = false;
    return false;
}

std::vector<std::pair<int, std::string>> GroupSettings::getGroups() {
    if(isOpened && data.contains("subGroups") && data["subGroups"].is_array()) {
        std::vector<std::pair<int, std::string>> groupsList;
        int i_tmp;
        std::string s_tmp;
        for(auto &group: data["subGroups"]) {
            group["id"].get_to(i_tmp);
            group["name"].get_to(s_tmp);
            groupsList.emplace_back(i_tmp, s_tmp);
        }

        return std::move(groupsList);
    }
    return {};
}

void GroupSettings::createMainGroup(int id) {
    if(!std::filesystem::exists(appDataDir.toStdString() + "groups"))
        std::filesystem::create_directories(appDataDir.toStdString() + "groups");

    data["groupId"] = id;
}

void GroupSettings::saveToFile() {
    printf("%s", settingsFile.c_str());
    std::ofstream f(settingsFile);
    f << data;
    f.close();
}

void GroupSettings::deleteGroup() {
    std::filesystem::remove(settingsFile);
}


GroupSettings::GroupSettings(int id): settingsFile(appDataDir.toStdString() + "groups/g" + std::to_string(id) + ".json") {

}

json GroupSettings::getStudentsList(int group) {
    if(isOpened && data.contains("subGroups") && data["subGroups"].is_array()) {
        for(auto &jsonGroup: data["subGroups"]) {
            if (jsonGroup["id"] == group) {
                return jsonGroup["studentsList"];
            }
        }
    }
    return {};
}

json GroupSettings::getAssigments() {
    if(isOpened && data.contains("assigments") && data["assigments"].is_array()) {
        return data["assigments"];
    }
    return {};
}

void GroupSettings::addGroup(const std::string &name) {
    int lastGroupId = 0;
    if(data.contains("lastGroupId")) {
        lastGroupId = data["lastGroupId"];
    }
    ++lastGroupId;
    data["lastGroupId"] = lastGroupId;

    json newGroup;
    newGroup["id"] = lastGroupId;
    newGroup["name"] = name;
    newGroup["studentsList"] = {};

    data["subGroups"].push_back(newGroup);
}

void GroupSettings::deleteSubGroup(int id) {
    for (int i = 0; i < data["subGroups"].size(); ++i) {
        if(data["subGroups"][i]["id"] == id) {
            data["subGroups"].erase(i);
            break;
        }
    }
}

void GroupSettings::updateGroupName(int id, const std::string &name) {
    for (int i = 0; i < data["subGroups"].size(); ++i) {
        if(data["subGroups"][i]["id"] == id) {
            data["subGroups"][i]["name"] = name;
            break;
        }
    }
}

void GroupSettings::addStudent(int groupId, const std::string &email, const std::string &firstName,
                               const std::string &lastName, const std::string &github, const std::string &studentId) {

    for (auto &group: data["subGroups"]) {
        if (group["id"] == groupId) {
            json newStudent;
            newStudent["email"] = email;
            newStudent["firstName"] = firstName;
            newStudent["github"] = github;
            newStudent["lastName"] = lastName;
            newStudent["studentId"] = studentId;

            group["studentsList"].push_back(newStudent);
            break;
        }
    }

}
