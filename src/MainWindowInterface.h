//
// Created by Filip on 01.09.2022.
//

#ifndef CHECKER_MAINWINDOWINTERFACE_H
#define CHECKER_MAINWINDOWINTERFACE_H
#include "QSettings"
#include "Settings.h"
#include <QMessageBox>
#include <QApplication>
#include <QErrorMessage>
#include <QPushButton>

class MainWindowInterface {
public:
    MainWindowInterface() = default;

    virtual ~MainWindowInterface() = default;

    virtual void showWindow() = 0;

    /**
     * Show critical message box with options continue or exit app
     * @param parent
     * @param title title of messagebox
     * @param text content of messagebox
     */
    void criticalMessageBoxExit(QWidget *parent, const QString &title, const QString &text) {
        QMessageBox errorDialog(parent);
        errorDialog.setText(text);
        errorDialog.setIcon(QMessageBox::Critical);
        errorDialog.setWindowTitle(title);
        auto closeButton = errorDialog.addButton("Close app", QMessageBox::ButtonRole::NoRole);
        errorDialog.addButton("Continue", QMessageBox::ButtonRole::YesRole);
        errorDialog.exec();
        if (errorDialog.clickedButton() == closeButton) {
            QApplication::exit(1);
            exit(1);
        }
    }

    void warningMessageBox(QWidget *parent, const QString &title, const QString &text) {
        QMessageBox warning(parent);
        warning.setWindowTitle(title);
        warning.setText(text);
        warning.setIcon(QMessageBox::Warning);
        warning.exec();
    }

    bool warningConfirmBox(QWidget *parent, const QString &title, const QString &text) {
        QMessageBox confirm(parent);
        confirm.setWindowTitle(title);
        confirm.setText(text);
        confirm.setIcon(QMessageBox::Warning);
        confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        return confirm.exec() == QMessageBox::Yes;
    }
};

#endif //CHECKER_MAINWINDOWINTERFACE_H
