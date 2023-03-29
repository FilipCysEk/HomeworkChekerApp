//
// Created by Filip on 21.09.2022.
//

#ifndef CHECKER_MAINWINDOWMANAGER_H
#define CHECKER_MAINWINDOWMANAGER_H

#include "MainWindowInterface.h"
#include "UI/WelcomeWindow.h"
#include "UI/AssigmentMW.h"
#include <QObject>


class MainWindowManager: public QObject {
    Q_OBJECT

private:
    std::unique_ptr<MainWindowInterface> openedWindow;

public slots:
    void openWelcomeWindow();

    void openAssigmentMW(int groupId, const std::string &path);

};


#endif //CHECKER_MAINWINDOWMANAGER_H
