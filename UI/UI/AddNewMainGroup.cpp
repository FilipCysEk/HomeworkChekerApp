//
// Created by Filip on 02.09.2022.
//

#include "AddNewMainGroup.h"
#include "ui_AddNewMainGroup.h"
#include "Settings.h"
#include <QFileDialog>
#include <QMessageBox>


AddNewMainGroup::AddNewMainGroup(const QStringList &parentsElements, QWidget *parent) :
        QDialog(parent), ui(new Ui::DialogAddNewBaseGroup), parents(parentsElements) {
    ui->setupUi(this);
    updateParentsDropdown();
}

AddNewMainGroup::~AddNewMainGroup() {
    delete ui;
}

void AddNewMainGroup::updateParentsDropdown() {
    ui->dd_parent->clear();
    ui->dd_parent->addItem("root");

    QString lastParent = "root";
    if (AppSettings::appSettings.contains("addNewMainGroup/lastParent")) {
        lastParent = AppSettings::appSettings.value("addNewMainGroup/lastParent").toString();
    }

    for (auto &parentItem: parents) {
        ui->dd_parent->addItem(parentItem);

        if (parentItem == lastParent) {
            ui->dd_parent->setCurrentIndex(ui->dd_parent->count() - 1);
        }
    }
}

void AddNewMainGroup::on_bb_footer_accepted() {
    AppSettings::appSettings.setValue("addNewMainGroup/lastParent", ui->dd_parent->currentText());
}

void AddNewMainGroup::on_b_openDirChooser_clicked() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);

    if(QDir(ui->ti_path->text()).exists()) {
        dialog.setDirectory(QDir(ui->ti_path->text()));
    }

    if (dialog.exec())
        ui->ti_path->setText(dialog.selectedFiles()[0]);
}

void AddNewMainGroup::accept() {
    if(ui->dd_parent->currentText().size() < 4) {
        QMessageBox mb(this);
        mb.setWindowTitle("Error parent group name");
        mb.setText("Parent group name should be longer!\n (Minimum 4 chars)");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        return;
    }

    if(ui->ti_groupName->text().size() < 4) {
        QMessageBox mb(this);
        mb.setWindowTitle("Error group name");
        mb.setText("Group name should be longer!\n (Minimum 4 chars)");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        return;
    }

    if(ui->ti_path->text().size() < 5) {
        QMessageBox mb(this);
        mb.setWindowTitle("Error");
        mb.setText("Path too short!");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        return;
    }

    const QFileInfo outputDir(ui->ti_path->text());
    if(!checkDirExists(outputDir)) {
        QDir().mkpath(ui->ti_path->text());

        if(!checkDirExists(outputDir)) {
            QMessageBox mb(this);
            mb.setWindowTitle("Error");
            mb.setText("Can't create directory!");
            mb.setIcon(QMessageBox::Warning);
            mb.exec();
            return;
        }
    }

    //TODO directory not empty
    //TODO not valid path pass (if insert random text)

    QDialog::accept();
}

bool AddNewMainGroup::checkDirExists(const QFileInfo &dir) {
    if ((!dir.exists()) || (!dir.isDir()) || (!dir.isWritable())) {
        return false;
    }
    return true;
}

QString AddNewMainGroup::getGroupParent() {
    return ui->dd_parent->currentText() == "root" ? "" : ui->dd_parent->currentText();
}

QString AddNewMainGroup::getGroupName() {
    return ui->ti_groupName->text();
}

QString AddNewMainGroup::getGroupPath() {
    return ui->ti_path->text();
}

void AddNewMainGroup::setValues(const QString &groupParent, const QString &groupName, const QString &groupPath) {
    ui->dd_parent->setEditText(groupParent);
    ui->ti_groupName->setText(groupName);
    ui->ti_path->setText(groupPath);
}
