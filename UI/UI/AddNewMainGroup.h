//
// Created by Filip on 02.09.2022.
//

#ifndef CHECKER_ADDNEWMAINGROUP_H
#define CHECKER_ADDNEWMAINGROUP_H


#include <QDialog>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class DialogAddNewBaseGroup; }
QT_END_NAMESPACE

class AddNewMainGroup : public QDialog {
    Q_OBJECT

public:
    explicit AddNewMainGroup(const QStringList &parentsElements, QWidget *parent = nullptr);

    ~AddNewMainGroup() override;

    void accept() override;

    QString getGroupParent();
    QString getGroupName();
    QString getGroupPath();

    void setValues(const QString & groupParent, const QString &groupName, const QString &groupPath);

private slots:
    void on_bb_footer_accepted();
    void on_b_openDirChooser_clicked();

private:
    Ui::DialogAddNewBaseGroup *ui;
    const QStringList &parents;

    void updateParentsDropdown();

    bool checkDirExists(const QFileInfo &dir);
};


#endif //CHECKER_ADDNEWMAINGROUP_H
