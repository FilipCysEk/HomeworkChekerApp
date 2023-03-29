//
// Created by Filip on 21.09.2022.
//

#include "AssigmentMW.h"
#include "ui_AssigmentsMainWindow.h"

AssigmentMW::AssigmentMW(int groupId, const std::string &path, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::AssigmentsMW), groupId(groupId), path(path), gSettings(GroupSettings(groupId)) {

    groupList = std::make_unique<TreeModel>();
    groupList->setColumnsTitle({"Name"}, {"id"});

    studentsList = std::make_unique<TreeModel>();
    studentsList->setColumnsTitle({"firstName", "lastName", "idNumber", "githubAddress"});

    assigmentsList = std::make_unique<TreeModel>();
    assigmentsList->setColumnsTitle({"name"}, {"path", "scriptToRun", "source"});
}

AssigmentMW::~AssigmentMW() {
    delete ui;
}

void AssigmentMW::showWindow() {
    ui->setupUi(this);

    restoreWindowGeometry();
    restoreStudentsTableState();
    try {
        ui->l_groupsList->setModel(groupList.get());
        ui->t_studentsList->setModel(studentsList.get());
        ui->l_assigmentsList->setModel(assigmentsList.get());

        gSettings.loadGroup();
        refreshGroupList();
        refreshAssigmentsList();

    } catch (nlohmann::json::exception &e) {
        QString text = "Error on parsing group database file (" + QString::number(groupId) + ".json)\n\n";
        text += QString(e.what());
        criticalMessageBoxExit(this, "Error on loading data", text);
    }
    show();
}

void AssigmentMW::refreshGroupList() {
    auto groupsList = gSettings.getGroups();

    auto model = ui->l_groupsList->model();
    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    for (auto &group: groupsList) {
        groupList->pushBackChild({QString::fromStdString(group.second), group.first});
    }
}

void AssigmentMW::refreshStudentsList(int groupId) {
    auto model = ui->t_studentsList->model();
    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    if (groupId > -1) {
        try {
            auto jsonStudentsList = gSettings.getStudentsList(groupId);
            int i = 0;
            for (auto &student: jsonStudentsList) {
                studentsList->pushBackChild({QString::fromStdString(student["firstName"]),
                                             QString::fromStdString(student["lastName"]),
                                             QString::fromStdString(student["studentId"]),
                                             QString::fromStdString(student["github"])});
                ui->t_studentsList->setRowHeight(i, 20);
                ++i;
            }
        } catch (nlohmann::json::exception &e) {
            QString text = "Error on loading group database file (" + QString::number(groupId) +
                    ".json) - section students\n\n";
            text += QString(e.what());
            criticalMessageBoxExit(this, "Error on loading data", text);
        }
    }
}

void AssigmentMW::on_l_groupsList_clicked() {
    refreshStudentsList(getIdSelectedGroup());
}

TreeItem *AssigmentMW::getSelectedItem(QModelIndexList selected, TreeModel *treeModel) {
    if (selected.empty()) {
        return nullptr;
    }

    auto selectedIndex = selected[0];
    return static_cast<TreeItem *>(selectedIndex.internalPointer());
}

int AssigmentMW::getIdSelectedGroup() {
    auto elem = getSelectedItem(ui->l_groupsList->selectionModel()->selectedIndexes(), groupList.get());
    if (elem == NULL)
        return -1;

    return groupList->getColumn("id", elem).toInt();
}

void AssigmentMW::refreshAssigmentsList() {
    auto model = ui->l_assigmentsList->model();
    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    if (groupId > -1) {
        try {
            auto jsonAssigmentsList = gSettings.getAssigments();
            for (auto &assigment: jsonAssigmentsList) {
                assigmentsList->pushBackChild({QString::fromStdString(assigment["name"]),
                                               QString::fromStdString(assigment["path"]),
                                               QString::fromStdString(assigment["scriptToRun"]),
                                               QString::fromStdString(assigment["source"])});
            }
        } catch (nlohmann::json::exception &e) {
            QString text = "Error on loading group database file (" + QString::number(groupId) +
                    ".json) - section assigments.\n\n";
            text += QString(e.what());
            criticalMessageBoxExit(this, "Error on loading data", text);
        }
    }
}

void AssigmentMW::saveWindowGeometry() {
    AppSettings::appSettings.setValue("assigmentsMainWindow/geometry", saveGeometry());
}

void AssigmentMW::restoreWindowGeometry() {
    if (AppSettings::appSettings.contains("assigmentsMainWindow/geometry"))
        restoreGeometry(AppSettings::appSettings.value("assigmentsMainWindow/geometry").toByteArray());
}

void AssigmentMW::saveStudentsTableState() {
    AppSettings::appSettings.setValue("assigmentsMainWindow/studentsTableHeader",
                                      ui->t_studentsList->horizontalHeader()->saveState());
}

void AssigmentMW::restoreStudentsTableState() {
    if (AppSettings::appSettings.contains("assigmentsMainWindow/studentsTableHeader"))
        ui->t_studentsList->horizontalHeader()->restoreState(
                AppSettings::appSettings.value("assigmentsMainWindow/studentsTableHeader").toByteArray());
}

void AssigmentMW::closeEvent(QCloseEvent *event) {
    saveWindowGeometry();
    saveStudentsTableState();
    QWidget::closeEvent(event);
}

void AssigmentMW::on_menuGroup_triggered(QAction *action) {
    if (action->objectName() == "actionNew_group") {
        createNewGroup();
    } else if (action->objectName() == "actionDelete_group") {
        deleteGroup();
    } else if (action->objectName() == "actionEdit_group") {
        editGroup();
    }
}

void AssigmentMW::createNewGroup() {
    auto dialog = QInputDialog(this);
    dialog.setWindowTitle("Creating group");
    dialog.setLabelText("Group name");
    if(dialog.exec() == QDialog::Accepted) {
        auto newGroupName = dialog.textValue();
        std::regex pattern(R"([a-zA-Z0-9 !@#$%^&*()_\-\[\]\{\}=+()\/?|]{3,30})");

        if(!std::regex_match(newGroupName.toStdString(), pattern)) {
            warningMessageBox(this, "Bad input", "Too short or illegal char!\nName should be have minimum 3 - 20 chars.");
        } else {
            gSettings.addGroup(newGroupName.toStdString());
            gSettings.saveToFile();
            refreshGroupList();
        }
    }
}

void AssigmentMW::deleteGroup() {
    auto selectedGroup = getIdSelectedGroup();
    if (selectedGroup == -1) {
        warningMessageBox(this, tr("Not selected group"), tr("Not selected group! Nothing to do."));
        return;
    }

    if(warningConfirmBox(this, tr("Are you sure?"), tr("Are you sure to delete selected group?"))) {
        gSettings.deleteSubGroup(selectedGroup);
        gSettings.saveToFile();
        refreshGroupList();
    }
}

void AssigmentMW::editGroup() {
    auto selectedGroup = getIdSelectedGroup();
    if (selectedGroup == -1) {
        warningMessageBox(this, tr("Not selected group"), tr("Not selected group! Nothing to do."));
        return;
    }

    auto editingGroup = getSelectedItem(ui->l_groupsList->selectionModel()->selectedIndexes(), groupList.get());
    auto dialog = QInputDialog(this);
    dialog.setWindowTitle(tr("Edit group"));
    dialog.setLabelText("Group name");
    dialog.setTextValue(groupList->getColumn("Name", editingGroup).toString());

    if(dialog.exec() == QDialog::Accepted) {
        gSettings.updateGroupName(selectedGroup, dialog.textValue().toStdString());
        gSettings.saveToFile();
        refreshGroupList();
    }
}

void AssigmentMW::importStudentsFromCsv() {
    if (getIdSelectedGroup() == -1) {
        warningMessageBox(this, tr("Not selected group"), tr("Not selected group! Please select group then try to add students to it."));
        return;
    }

    if(studentsList->rowCount() > 0) {
        warningMessageBox(this, tr("Not supported"), tr("Importing students list in to existing student list is not supported yet."));
        return;
    }

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("*.csv");
    if (dialog.exec()) {
        studentsCsvLoad(dialog.selectedFiles()[0].toStdString());
    }
}

void AssigmentMW::on_menuStudents_triggered(QAction *action) {
    if (action->objectName() == "actionImport_students_from_CSV") {
        importStudentsFromCsv();
    }
}

void AssigmentMW::studentsCsvLoad(const std::string& path) {
    CSVlib studentsCsv(path);
    studentsCsv.setIgnoreWhiteChars(true);
    if(!studentsCsv.isOk()) {
        warningMessageBox(this, tr("Error with students list"), tr("Error on opening students list!"));
        return;
    }

    auto head = studentsCsv.getHead();
    if(std::find(head.begin(), head.end(), "email") == head.end() ||
            std::find(head.begin(), head.end(), "firstName") == head.end() ||
            std::find(head.begin(), head.end(), "github") == head.end() ||
            std::find(head.begin(), head.end(), "lastName") == head.end() ||
            std::find(head.begin(), head.end(), "studentId") == head.end()) {
        warningMessageBox(this, tr("Error with students list"), tr("Error on preprocessing file! Not found on of head in csv file\n First line of file should be: email, firstName, github, lastName, studentId"));
        return;
    }

    auto groupId = getIdSelectedGroup();
    do {
        auto rowMap = studentsCsv.getRowMap();
        gSettings.addStudent(groupId, rowMap["email"], rowMap["firstName"], rowMap["lastName"],
                             rowMap["github"], rowMap["studentId"]);
    } while(studentsCsv.next());

    if(studentsCsv.getReadLinesError() > 0) {
        auto errors = studentsCsv.getErrorsList();
        QString text = tr("Loading completed with errors:\n");
        for(auto &err: errors) {
            text += QString::fromStdString(err);
            text += "\n";
        }

        warningMessageBox(this, tr("Errors on loading students list"), text);
    } else {
        warningMessageBox(this, tr("Students loaded!"), tr("Students list loaded successfully!"));
    }

    gSettings.saveToFile();
    refreshStudentsList(groupId);
}











