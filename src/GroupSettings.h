//
// Created by Filip on 21.09.2022.
//

#ifndef CHECKER_GROUPSETTINGS_H
#define CHECKER_GROUPSETTINGS_H

#include "constDeclarations.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
using json = nlohmann::json;


class GroupSettings {
    const std::string settingsFile;
    json data = json();
    bool isOpened = false;

public:
    GroupSettings(int id);

    bool loadGroup();

    [[nodiscard]]
    std::vector<std::pair<int, std::string>> getGroups();

    [[nodiscard]]
    json getStudentsList(int group);

    [[nodiscard]]
    json getAssigments();

    void addStudent(int groupId, const std::string &email, const std::string &firstName, const std::string &lastName,
                    const std::string &github,  const std::string &studentId);

    void addGroup(const std::string &name);

    void updateGroupName(int id, const std::string &name);

    void createMainGroup(int id);

    void deleteGroup();

    void deleteSubGroup(int id);

    void saveToFile();
};


#endif //CHECKER_GROUPSETTINGS_H
