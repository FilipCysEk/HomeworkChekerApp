#include <QApplication>
#include "MainWindowManager.h"
#include "Settings.h"

QSettings AppSettings::appSettings = QSettings(appDataDir + "settings.ini", QSettings::IniFormat);

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setOrganizationName(CV_ORGANIZATION);
    QApplication::setApplicationName(CV_APP_NAME);

    MainWindowManager mainWindow;
    mainWindow.openWelcomeWindow();
    return QApplication::exec();
}
