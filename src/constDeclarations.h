//
// Created by Filip on 01.09.2022.
//

#ifndef CHECKER_CONSTDECLARATIONS_H
#define CHECKER_CONSTDECLARATIONS_H

#include <QStandardPaths>

#define CV_ORGANIZATION "FilipCysEk"
#define CV_APP_NAME "Checker"

const QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + CV_APP_NAME + "/";

#endif //CHECKER_CONSTDECLARATIONS_H
