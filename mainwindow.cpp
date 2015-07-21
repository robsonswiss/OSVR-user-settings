/** @file
    @brief Header

    @date 2014

    @author
    Razer, Inc.
    <http://www.razerzone.com>
*/

// Copyright 2015 Razer, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QProcess>
#include <QDir>

#include "json/json.h"

// Helper function for validating numerical input
class myValidator : public QDoubleValidator
{
    public:
    myValidator(double bottom, double top, int decimals, QObject * parent) :
    QDoubleValidator(bottom, top, decimals, parent)
    {
    }

    QValidator::State validate(QString &s, int &i) const
    {
        if (s.isEmpty() || s == "-") {
            return QValidator::Intermediate;
        }

        QLocale locale;

        QChar decimalPoint = locale.decimalPoint();
        int charsAfterPoint = s.length() - s.indexOf(decimalPoint) -1;

        if (charsAfterPoint > decimals() && s.indexOf(decimalPoint) != -1) {
            return QValidator::Invalid;
        }

        bool ok;
        double d = locale.toDouble(s, &ok);

        if (ok && d >= bottom() && d <= top()) {
            return QValidator::Acceptable;
        } else {
            return QValidator::Invalid;
        }
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
// Connect button signal to appropriate slot
//     connect(ui->ODdominant, SIGNAL (released()), this, SLOT (on_ODdominantButton_clicked();));
//     connect(ui->OSdominant, SIGNAL (released()), this, SLOT (on_OSdominantButton_clicked();));
//    connect(ui->resetButton, SIGNAL (released()), this, SLOT (on_resetButton_clicked();));
    ui->setupUi(this);

    ui->standingHeight->setValidator( new myValidator(0, 300, 2, this) );
    ui->seatedHeight->setValidator( new myValidator(0, 300, 2, this) );
    ui->ipd->setValidator(new myValidator(0,100,2,0));

    ui->dOsSpherical->setValidator( new myValidator(-100, 100, 2, this) );
    ui->dOsCylindrical->setValidator( new myValidator(-100, 100, 2, this) );
    ui->dOsAxis->setValidator( new myValidator(-100, 300, 2, this) );
    ui->nOsAdd->setValidator( new myValidator(-100, 100, 2, this) );

    ui->dOdSpherical->setValidator( new myValidator(-100, 100, 2, this) );
    ui->dOdCylindrical->setValidator( new myValidator(-100, 100, 2, this) );
    ui->dOdAxis->setValidator( new myValidator(-100, 300, 2, this) );
    ui->nOdAdd->setValidator( new myValidator(-100, 100, 2, this) );

    ui->tabWidget->setCurrentIndex(0);

    osvrUserConfigFilename = QString("OSVR_user_config.json");
    loadConfigFile(osvrUserConfigFilename);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadConfigFile(QString filename)
{
    char* cstr;
    std::string fname = filename.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );

    std::ifstream file_id;
    file_id.open(cstr);

    Json::Reader reader;
    Json::Value value;
    if (!reader.parse (file_id, value)){
         qWarning("Couldn't open save file, creating file.");
        // new file just has default values
        saveConfigFile(filename);
    }else{
        osvrUser.read(value);
    }
    updateFormValues();
    return true;
}

void MainWindow::updateFormValues(){
    if (!QString::compare("Male",osvrUser.gender()))
        ui->gender->setCurrentIndex(0);
    else
        ui->gender->setCurrentIndex(1);
    ui->standingHeight->setText(QString::number(osvrUser.standingEyeHeight()));
    ui->seatedHeight->setText(QString::number(osvrUser.seatedEyeHeight()));
    ui->ipd->setText(QString::number(osvrUser.pupilDistance(OS)+osvrUser.pupilDistance(OD)));

    ui->dOsSpherical->setText(QString::number(osvrUser.spherical(OS)));
    ui->dOsCylindrical->setText(QString::number(osvrUser.cylindrical(OS)));
    ui->dOsAxis->setText(QString::number(osvrUser.axis(OS)));
    ui->nOsAdd->setText(QString::number(osvrUser.addNear(OS)));
    ui->OSdominant->setChecked(osvrUser.dominant(OS));

    ui->dOdSpherical->setText(QString::number(osvrUser.spherical(OD)));
    ui->dOdCylindrical->setText(QString::number(osvrUser.cylindrical(OD)));
    ui->dOdAxis->setText(QString::number(osvrUser.axis(OD)));
    ui->nOdAdd->setText(QString::number(osvrUser.addNear(OD)));
    ui->ODdominant->setChecked(osvrUser.dominant(OD));
}

void MainWindow::on_ODdominantButton_clicked(){
    ui->ODdominant->setChecked(true);
    ui->OSdominant->setChecked(false);
}

void MainWindow::on_OSdominantButton_clicked(){
    ui->OSdominant->setChecked(true);
    ui->ODdominant->setChecked(false);
}


void MainWindow::on_resetButton_clicked()
{
    updateFormValues();
}

void MainWindow::on_aboutButton_clicked()
{
    QMessageBox::information(0, QString("OSVR Configuration Utility"), QString("Simple utility for helping you set up and configure your personal OSVR settings... For more information, visit www.osvr.com."), QMessageBox::Ok);
    // loadConfigFile(QString("start.json"));
}

void MainWindow::loadValuesFromForm(OSVRUser *oo)
{
    double ipd;

    oo->setGender(ui->gender->currentText());

    // IPD settings
    if (!ui->ipd->text().isEmpty()){
        ipd = ui->ipd->text().toDouble();
        oo->setPupilDistance(OS,ipd/2);
        oo->setPupilDistance(OD,ipd/2);
    }

    //Left eye settings
    if (!ui->dOsSpherical->text().isEmpty())
        oo->setSpherical(OS,ui->dOsSpherical->text().toDouble());
    if (!ui->dOsCylindrical->text().isEmpty())
        oo->setCylindrical(OS,ui->dOsCylindrical->text().toDouble());
    if (!ui->dOsAxis->text().isEmpty())
        oo->setAxis(OS,ui->dOsAxis->text().toDouble());
    if (!ui->nOsAdd->text().isEmpty())
        oo->setAddNear(OS,ui->nOsAdd->text().toDouble());

    // right eye settings
    if (!ui->dOdSpherical->text().isEmpty())
        oo->setSpherical(OD,ui->dOdSpherical->text().toDouble());
    if (!ui->dOdCylindrical->text().isEmpty())
        oo->setCylindrical(OD,ui->dOdCylindrical->text().toDouble());
    if (!ui->dOdAxis->text().isEmpty())
        oo->setAxis(OD,ui->dOdAxis->text().toDouble());
    if (!ui->nOdAdd->text().isEmpty())
        oo->setAddNear(OD,ui->nOdAdd->text().toDouble());

    if(ui->ODdominant->isChecked())
        oo->setDominant(OD);
    else
        oo->setDominant(OS);

    // anthropometric settings
    if (!ui->standingHeight->text().isEmpty())
        oo->setStandingEyeHeight(ui->standingHeight->text().toDouble());
    if (!ui->seatedHeight->text().isEmpty())
        oo->setSeatedEyeHeight(ui->seatedHeight->text().toDouble());
}

void MainWindow::saveConfigFile(QString filename)
{
    char* cstr;
    std::string fname = filename.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );

    Json::Value ooo;
    osvrUser.write(ooo);

    std::ofstream out_file;
    out_file.open(cstr);
    Json::StyledWriter styledWriter;
    out_file <<styledWriter.write(ooo);
    out_file.close();
}

void MainWindow::on_saveButton_clicked()
{
    loadValuesFromForm(&osvrUser);
    saveConfigFile(osvrUserConfigFilename);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_demoButton_clicked()
{
    QProcess *process = new QProcess(this);
    QString file = "spherical.exe -adapter 1 -screen-quality good -single-instance";
    process->start(file);
}

void MainWindow::on_resetYawButton_clicked()
{
    QProcess *process = new QProcess(this);
    QString file = "reset_yaw.bat";
    process->start(file);
}
