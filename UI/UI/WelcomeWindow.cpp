//
// Created by Filip on 01.09.2022.
//

#include "WelcomeWindow.h"
#include "ui_WelcomeWindow.h"
#include <QMessageBox>


WelcomeWindow::WelcomeWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::WelcomeWindow) {
    treeModel = std::make_unique<TreeModel>(this);
}

WelcomeWindow::~WelcomeWindow() {
    delete ui;
}

void WelcomeWindow::closeEvent(QCloseEvent *event) {
    AppSettings::appSettings.setValue("welcomeWindow/geometry", saveGeometry());
    AppSettings::appSettings.setValue("welcomeWindow/tvHeader", ui->tv_groupsList->header()->saveState());
    QWidget::closeEvent(event);
}

void WelcomeWindow::on_b_addGroup_clicked() {
    auto addMainGroup = AddNewMainGroup(parentsList, this);
    if (addMainGroup.exec() == QDialog::Accepted) {
        mainGroupSettings.addNewGroup(addMainGroup.getGroupName(), addMainGroup.getGroupPath(),
                                      addMainGroup.getGroupParent());
        mainGroupSettings.saveToFile();

        reloadGroups();
    }

}

void WelcomeWindow::showWindow() {
    ui->setupUi(this);

    if (AppSettings::appSettings.contains("welcomeWindow/geometry"))
        restoreGeometry(AppSettings::appSettings.value("welcomeWindow/geometry").toByteArray());

    loadSettings();

    if (AppSettings::appSettings.contains("welcomeWindow/tvHeader"))
        ui->tv_groupsList->header()->restoreState(
                AppSettings::appSettings.value("welcomeWindow/tvHeader").toByteArray());

    if (AppSettings::appSettings.contains("welcomeWindow/tvExpanded"))
        restoreExpanded();

    show();
}

void WelcomeWindow::loadSettings() {
    treeModel->setColumnsTitle({"Name", "Path"}, {"id"});
    try {
        mainGroupSettings.load();
        auto groupsData = mainGroupSettings.getGroupObject();

        if (groupsData.is_array()) {
            loadSettingsReadGroups(groupsData);
        }
    } catch (nlohmann::json::parse_error &e) {
        showLoadSettingsErrorDialog(this, QString(e.what()));
    }
    ui->tv_groupsList->setModel(treeModel.get());
}

void WelcomeWindow::loadSettingsReadGroups(const json &groupsData) {
    std::string temp;

    for (auto &tabElem: groupsData) {
        //todo validation
        int groupId;
        tabElem["id"].get_to(groupId);

        tabElem["name"].get_to(temp);
        QString groupName = QString::fromStdString(temp);

        tabElem["path"].get_to(temp);
        QString groupPath = QString::fromStdString(temp);

        tabElem["parent"].get_to(temp);
        QString groupParent = QString::fromStdString(temp);

        if (groupParent == "") {
            treeModel->pushBackChild({groupName, groupPath, groupId});
        } else {
            bool flgFound = false;
            addToParentList(groupParent);

            for (int i = 0; i < treeModel->rowCount(); ++i) {
                auto index = treeModel->index(i, 0);
                if (index.data() == groupParent) {
                    flgFound = true;
                    static_cast<TreeItem *>(index.internalPointer())->pushBackChildren({groupName, groupPath, groupId});
                }
            }

            if (!flgFound) {
                auto parentElem = treeModel->pushBackChild(style()->standardIcon(QStyle::SP_DirIcon), {groupParent});
                parentElem->pushBackChildren({groupName, groupPath, groupId});
            }
        }
    }
}

void WelcomeWindow::on_b_editGroup_clicked() {
    auto elem = getSelectedGroup();
    if(elem == nullptr)
        return;

    AddNewMainGroup editGroup(parentsList, this);
    auto parentGroupName = treeModel->getColumn("Name", elem->getParent()).toString().isEmpty() ? "root" :
            treeModel->getColumn("Name", elem->getParent()).toString();

    editGroup.setValues(parentGroupName,
                        treeModel->getColumn("Name", elem).toString(),
                        treeModel->getColumn("Path", elem).toString());

    if (editGroup.exec() == QDialog::Accepted) {
        mainGroupSettings.editGroup(treeModel->getColumn("id", elem).toInt(), editGroup.getGroupName(),
                                    editGroup.getGroupPath(), editGroup.getGroupParent());
        mainGroupSettings.saveToFile();
        reloadGroups();
    }
}

void WelcomeWindow::on_b_deleteGroup_clicked() {
    auto elem = getSelectedGroup();
    if(elem == nullptr)
        return;

    if (warningDeleteGroup(treeModel->getColumn("Name", elem).toString())) {
        mainGroupSettings.deleteGroup(treeModel->getColumn("id", elem).toInt());
        mainGroupSettings.saveToFile();
        reloadGroups();
    }
}

void WelcomeWindow::on_tv_groupsList_collapsed() {
    saveExpandedCollapsed();
}

void WelcomeWindow::on_tv_groupsList_expanded() {
    saveExpandedCollapsed();
}

void WelcomeWindow::saveExpandedCollapsed() {
    QStringList expanded;
    for (int i = 0; i < treeModel->rowCount(); ++i) {
        auto index = treeModel->index(i, 0);
        if (ui->tv_groupsList->isExpanded(index))
            expanded.push_back(treeModel->data(index).toString());
    }

    AppSettings::appSettings.setValue("welcomeWindow/tvExpanded", expanded);
}

void WelcomeWindow::restoreExpanded() {
    auto expanded = AppSettings::appSettings.value("welcomeWindow/tvExpanded").value<QStringList>();
    for (int i = 0; i < treeModel->rowCount(); ++i) {
        auto index = treeModel->index(i, 0);
        for (auto &expandedItem: expanded) {
            if (treeModel->data(index).toString() == expandedItem)
                ui->tv_groupsList->expand(index);
        }
    }
}

void WelcomeWindow::on_tv_groupsList_doubleClicked() {
    openAssigment();
}

void WelcomeWindow::addToParentList(const QString &element) {
    if (parentsList.indexOf(element) < 0)
        parentsList.push_back(element);
}

void WelcomeWindow::reloadGroups() {
    auto model = ui->tv_groupsList->model();
    if(model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    loadSettingsReadGroups(mainGroupSettings.getGroupObject());
    restoreExpanded();
}

void WelcomeWindow::warningNotSelectedGroup() {
    warningMessageBox(this, "Not selected group", "Group in the list is not selected");
}

bool WelcomeWindow::warningDeleteGroup(const QString &groupName) {
    QString text = tr("Are you sure to delete group: ");
    return warningConfirmBox(this, tr("Are you sure?"), text + groupName + "?");
}

TreeItem *WelcomeWindow::getSelectedGroup() {
    auto selected = ui->tv_groupsList->selectionModel()->selectedIndexes();
    if (selected.empty()) {
        warningNotSelectedGroup();
        return nullptr;
    }

    auto selectedIndex = selected[0];
    auto elem =  static_cast<TreeItem *>(selectedIndex.internalPointer());

    if (treeModel->getColumn("Path", elem).toString().isEmpty()) {
        warningMessageBox(this, "Bad group", "Can't do that with parent group!\nYou must do that with all child's");
        return nullptr;
    }

    return elem;
}

void WelcomeWindow::on_b_openGroup_clicked() {
    openAssigment();
}

void WelcomeWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete) {
        on_b_deleteGroup_clicked();
    } else if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        openAssigment();
    }

}

void WelcomeWindow::showLoadSettingsErrorDialog(QWidget *parent, const QString &what) {
    QString errorText = "Error on loading file with group settings (mainGroups.json)!\n<" + what + ">";
    errorText += "\n\nClick continue to create new configuration.";

    criticalMessageBoxExit(this, "Error on loading config file", errorText);
}