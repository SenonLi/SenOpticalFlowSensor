#ifndef SENOPTICALFLOWSENSORSMAINWINDOW_H
#define SENOPTICALFLOWSENSORSMAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

#include "SenConsole.h"
#include "SenSettingsDialog.h"

#include <QSlider>
#include <QBoxLayout>
#include <QSpinBox>
#include <QLabel>
////
#include "SenSerialControlAlgorithmObject.h"
///

namespace Ui {
class SenOpticalFlowSensorsMainWindow;
}

class SenOpticalFlowSensorsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SenOpticalFlowSensorsMainWindow(QWidget *parent = 0);
    ~SenOpticalFlowSensorsMainWindow();

private slots:
    void about();
////

    void stepUp();
    void stepDown();
    void stepLeft();
    void stepRight();

    void step10Up();
    void step10Down();
    void step10Left();
    void step10Right();

    void setHorizontalPosition(int panValue);
    void setVerticalPosition(int tiltValue);


    void writeData(const QByteArray &data);
    void setSerialCloseStatus();
    void setSerialOpenedStatus();
    void setSerialOpenErrorStatus(QString serialOpenErrorString);
    void setSerialCriticalErrorStatus(QString serialCriticalErrorString);
    void openSerialPortPrepare();
    void showDataInConsole(QByteArray data);

signals:
    void emitOpenSerialOrder(SenSettingsDialog::Settings);
////

private:
    void initSliderSpinBoxConnections();
    void initActionsConnections();
    void serialInfoGroupHardWrie();

    Ui::SenOpticalFlowSensorsMainWindow *myMainWindow_ui;
    SenConsole *mySenConsole;
    SenSettingsDialog *mySenSettingsDialog;

    QSlider *horizontalQSlider, *verticalQSlider;
    QSpinBox *horizontalQSpinBox, *verticalQSpinBox;
    QLabel *horizontalQLabel, *verticalQLabel;
////
    SenSettingsDialog::Settings mySettingInfo;
    SenSerialControlAlgorithmObject *myDefaulSerialObject;
////
};

#endif // SENOPTICALFLOWSENSORSMAINWINDOW_H
