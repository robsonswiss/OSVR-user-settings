#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "osvruser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void saveConfigFile(QString filename);
    bool loadConfigFile(QString filename);
    void updateFormValues(void);
    void loadValuesFromForm(OSVRUser *oo);

    void on_resetButton_clicked();
    void on_saveButton_clicked();
    void on_exitButton_clicked();
    void on_aboutButton_clicked();
    void on_ODdominantButton_clicked();
    void on_OSdominantButton_clicked();
    void on_demoButton_clicked();
    void on_resetYawButton_clicked();

private:
    Ui::MainWindow *ui;

    QString osvrUserConfigFilename;
    OSVRUser osvrUser;
};

#endif // MAINWINDOW_H
