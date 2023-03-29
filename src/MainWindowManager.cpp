//
// Created by Filip on 21.09.2022.
//

#include "MainWindowManager.h"

void MainWindowManager::openWelcomeWindow() {
    openedWindow = std::make_unique<WelcomeWindow>();
    openedWindow->showWindow();

    QObject::connect(dynamic_cast<WelcomeWindow*>(openedWindow.get()), &WelcomeWindow::sig_openAssigment,
                     this, &MainWindowManager::openAssigmentMW);
}

void MainWindowManager::openAssigmentMW(int groupId, const std::string &path) {
    openedWindow = std::make_unique<AssigmentMW>(groupId, path);
    openedWindow->showWindow();
}
