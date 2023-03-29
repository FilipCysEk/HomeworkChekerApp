//
// Created by Filip on 21.09.2022.
//

#ifndef CHECKER_ASSIGMENTMW_H
#define CHECKER_ASSIGMENTMW_H


#include <QMainWindow>
#include <QInputDialog>
#include "MainWindowInterface.h"
#include "GroupSettings.h"
#include <QStringListModel>
#include <TreeModel.h>
#include <regex>
#include <QFileDialog>
#include <CSVlib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class AssigmentsMW; }
QT_END_NAMESPACE

class AssigmentMW: public QMainWindow, public MainWindowInterface  {
    Q_OBJECT
    Ui::AssigmentsMW *ui;
    const int groupId;
    const std::string path;
    GroupSettings gSettings;
    std::unique_ptr<TreeModel> groupList;
    std::unique_ptr<TreeModel> studentsList;
    std::unique_ptr<TreeModel> assigmentsList;

    void refreshGroupList();
    void refreshStudentsList(int groupId = -1);

    void refreshAssigmentsList();

    TreeItem *getSelectedItem(QModelIndexList selected, TreeModel *treeModel);

    /**
     * Return id of selected group
     * @return -1 if not selected, or index of selected element
     */
    int getIdSelectedGroup();

    void restoreStudentsTableState();
    void saveStudentsTableState();

    void restoreWindowGeometry();
    void saveWindowGeometry();

    void closeEvent(QCloseEvent *event) override;

    void createNewGroup();

    void deleteGroup();

    void editGroup();

    void importStudentsFromCsv();

    void studentsCsvLoad(const std::string& path);

public slots:
    void on_l_groupsList_clicked();

    void on_menuGroup_triggered(QAction *action);

    void on_menuStudents_triggered(QAction *action);

public:
    explicit AssigmentMW(int groupId, const std::string &path, QWidget *parent = nullptr);

    ~AssigmentMW() override;

    void showWindow() override;

};


#endif //CHECKER_ASSIGMENTMW_H
