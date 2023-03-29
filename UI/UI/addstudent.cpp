//
// Created by Filip on 31.10.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddStudent.h" resolved

#include "addstudent.h"
#include "ui_AddStudent.h"


AddStudent::AddStudent(QWidget *parent) :
        QWidget(parent), ui(new Ui::AddStudent) {
    ui->setupUi(this);
}

AddStudent::~AddStudent() {
    delete ui;
}
