//
// Created by Filip on 01.09.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddGroup.h" resolved

#include "addgroup.h"
#include "ui_AddGroup.h"


AddGroup::AddGroup(QWidget *parent) :
        QWidget(parent), ui(new Ui::AddGroup) {
    ui->setupUi(this);
}

AddGroup::~AddGroup() {
    delete ui;
}
