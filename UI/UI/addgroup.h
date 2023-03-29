//
// Created by Filip on 01.09.2022.
//

#ifndef CHECKER_ADDGROUP_H
#define CHECKER_ADDGROUP_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class AddGroup; }
QT_END_NAMESPACE

class AddGroup : public QWidget {
Q_OBJECT

public:
    explicit AddGroup(QWidget *parent = nullptr);

    ~AddGroup() override;

private:
    Ui::AddGroup *ui;
};


#endif //CHECKER_ADDGROUP_H
