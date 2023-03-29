//
// Created by Filip on 01.09.2022.
//

#ifndef CHECKER_WELCOMEWINDOW_H
#define CHECKER_WELCOMEWINDOW_H

#include <QWidget>
#include "Settings.h"
#include "MainWindowInterface.h"
#include "AddNewMainGroup.h"
#include "MainGroupSettings.h"
#include <QKeyEvent>
#include "TreeModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WelcomeWindow; }
QT_END_NAMESPACE

class WelcomeWindow : public QWidget, public MainWindowInterface  {
Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

    ~WelcomeWindow() override;

    void showWindow() override;

private slots:

    void on_b_addGroup_clicked();

    void on_b_editGroup_clicked();

    void on_b_deleteGroup_clicked();

    void on_b_openGroup_clicked();

    void on_tv_groupsList_collapsed();
    void on_tv_groupsList_expanded();
    void on_tv_groupsList_doubleClicked();

    void keyPressEvent(QKeyEvent *event) override;

signals:
    void sig_openAssigment(int groupId, const std::string &path);

private:
    MainGroupSettings mainGroupSettings;
    std::unique_ptr<TreeModel> treeModel;
    QStringList parentsList;

    Ui::WelcomeWindow *ui;

    void loadSettings();

    void loadSettingsReadGroups(const json &groupsData);

    void reloadGroups();

    void warningNotSelectedGroup();
    bool warningDeleteGroup(const QString &groupName);
    void showLoadSettingsErrorDialog(QWidget *parent, const QString &what);

    TreeItem * getSelectedGroup();

    void closeEvent(QCloseEvent *event) override;

    void saveExpandedCollapsed();

    void restoreExpanded();

    void addToParentList(const QString &element);

    inline void openAssigment() {
        auto elem = getSelectedGroup();
        if(elem == nullptr)
            return;

        emit sig_openAssigment(treeModel->getColumn("id", elem).toInt(),
                               treeModel->getColumn("Path", elem).toString().toStdString());
    }
};


#endif //CHECKER_WELCOMEWINDOW_H
