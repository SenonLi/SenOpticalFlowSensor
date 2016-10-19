#ifndef SENSERIALSETTINGDIALOG_H
#define SENSERIALSETTINGDIALOG_H

#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QCoreApplication>

#include <QDialog>
#include <QDialogButtonBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QThread>

#include "SenBleDongleTalkFundamentals.h"

class SenSerialSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SenSerialSettingWidget(QWidget *parent = 0);
    ~SenSerialSettingWidget();

    inline SerialInfoGroup getSerialSettings() const { return(currentSettings); }
    inline bool checkCustomerActive() const { return(customerControlActive); }

    void apply();

private slots:
    void showPortInfo(int idx);
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    SerialInfoGroup currentSettings;
    QIntValidator *intValidator;

    bool customerControlActive;
    QLabel *descriptionLabel, *manufacturerLabel, *baudRateLabel;
    QComboBox *serialPortInfoListBox, *baudRateBox;
    QGroupBox *serialPortsSelectGroupBox, *parametersSelectGroupBox;
};


class SenSerialSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SenSerialSettingDialog(QDialog *parent = 0);
    inline bool checkCustomerActive() const { return(widget->checkCustomerActive()); }

    SerialInfoGroup settings() const;

private slots:
    inline void apply() { widget->apply(); hide(); }
    inline void appear()    {
        this->show();
    }
private:
    SenSerialSettingWidget *widget;
};

#endif // SENSERIALSETTINGDIALOG_H
