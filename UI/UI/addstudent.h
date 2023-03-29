//
// Created by Filip on 31.10.2022.
//

#ifndef CHECKER_ADDSTUDENT_H
#define CHECKER_ADDSTUDENT_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class AddStudent; }
QT_END_NAMESPACE

class AddStudent : public QWidget {
Q_OBJECT

public:
    explicit AddStudent(QWidget *parent = nullptr);

    ~AddStudent() override;

private:
    Ui::AddStudent *ui;
};


#endif //CHECKER_ADDSTUDENT_H
