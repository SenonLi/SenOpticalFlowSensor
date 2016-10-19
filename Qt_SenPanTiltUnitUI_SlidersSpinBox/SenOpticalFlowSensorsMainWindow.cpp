#include "SenOpticalFlowSensorsMainWindow.h"
#include "ui_SenOpticalFlowSensorsMainWindow.h"

SenOpticalFlowSensorsMainWindow::SenOpticalFlowSensorsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    myMainWindow_ui(new Ui::SenOpticalFlowSensorsMainWindow)
{
    myMainWindow_ui->setupUi(this);
    mySenConsole = new SenConsole;
    mySenConsole->setEnabled(false);



    horizontalQLabel = new QLabel(tr("Pan(H) Axis: "));

    horizontalQSlider = new QSlider(Qt::Horizontal);
    horizontalQSlider->setFocusPolicy(Qt::StrongFocus);
    horizontalQSlider->setTickPosition(QSlider::NoTicks);
//    horizontalQSlider->setTickInterval(500);
    horizontalQSlider->setSingleStep(1);

    horizontalQSlider->setValue(0);
    horizontalQSlider->setRange(-3000, 3000);

    horizontalQSpinBox = new QSpinBox;
    horizontalQSpinBox->setRange(-3000, 3000);
    horizontalQSpinBox->setSingleStep(1);
    horizontalQSpinBox->setValue(0);

    verticalQLabel = new QLabel(tr("Tilt(V) Axis: "));

    verticalQSlider = new QSlider(Qt::Horizontal);
    verticalQSlider->setFocusPolicy(Qt::StrongFocus);
    verticalQSlider->setTickPosition(QSlider::NoTicks);
//    verticalQSlider->setTickInterval(100);
    verticalQSlider->setSingleStep(1);

    verticalQSlider->setValue(0);
    verticalQSlider->setRange(-600, 600);

    verticalQSpinBox = new QSpinBox;
    verticalQSpinBox->setRange(-600, 600);
    verticalQSpinBox->setSingleStep(1);
    verticalQSpinBox->setValue(0);
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);

    QGridLayout *slidersLayout = new QGridLayout;
    slidersLayout->addWidget(horizontalQLabel, 0, 0, 1, 1);
    slidersLayout->addWidget(verticalQLabel, 1, 0, 1, 1);
    slidersLayout->addWidget(horizontalQSlider, 0, 1, 1, 10);
    slidersLayout->addWidget(verticalQSlider, 1, 1, 1, 10);
    slidersLayout->addWidget(horizontalQSpinBox, 0, 11, 1, 2);
    slidersLayout->addWidget(verticalQSpinBox, 1, 11, 1, 2);

    layout->addSpacing(10);

    layout->addLayout(slidersLayout);
    layout->addSpacing(20);
    layout->addWidget(mySenConsole);

    QWidget *sliderConsoleWidget = new QWidget();
    sliderConsoleWidget->setLayout(layout);

    setCentralWidget(sliderConsoleWidget);

    initSliderSpinBoxConnections();

    this->resize(1000,800);
////
    myDefaulSerialObject = new SenSerialControlAlgorithmObject();
////

    mySenSettingsDialog = new SenSettingsDialog;
    serialInfoGroupHardWrie();

    myMainWindow_ui->actionConnect->setEnabled(true);
    myMainWindow_ui->actionDisconnect->setEnabled(false);
    myMainWindow_ui->actionQuit->setEnabled(true);
    myMainWindow_ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    myMainWindow_ui->mainToolBar->hide();
    emit emitOpenSerialOrder(mySettingInfo);
}
void SenOpticalFlowSensorsMainWindow::initSliderSpinBoxConnections()
{
    connect(verticalQSlider, SIGNAL(valueChanged(int)),
            verticalQSpinBox, SLOT(setValue(int)));
    connect(verticalQSpinBox, SIGNAL(valueChanged(int)),
            verticalQSlider, SLOT(setValue(int)));

    connect(horizontalQSlider, SIGNAL(valueChanged(int)),
            horizontalQSpinBox, SLOT(setValue(int)));
    connect(horizontalQSpinBox, SIGNAL(valueChanged(int)),
            horizontalQSlider, SLOT(setValue(int)));

    connect(horizontalQSlider, SIGNAL(valueChanged(int)),
            this, SLOT(setHorizontalPosition(int)));
    connect(verticalQSlider, SIGNAL(valueChanged(int)),
            this, SLOT(setVerticalPosition(int)));
}

void SenOpticalFlowSensorsMainWindow::serialInfoGroupHardWrie()
{
    mySettingInfo.name = "COM3";
    mySettingInfo.baudRate = 9600;
    mySettingInfo.dataBits = (QSerialPort::DataBits)8;
    mySettingInfo.parity = (QSerialPort::Parity)0;
    mySettingInfo.stopBits = (QSerialPort::StopBits)1;
    mySettingInfo.flowControl = (QSerialPort::FlowControl)0;
    mySettingInfo.localEchoEnabled = false;
}

SenOpticalFlowSensorsMainWindow::~SenOpticalFlowSensorsMainWindow()
{
    delete mySenSettingsDialog;
    delete myMainWindow_ui;
}

void SenOpticalFlowSensorsMainWindow::about()
{
    QMessageBox::about(this, tr("About Sen Serial Terminal"),
                       tr("The <b>Sen Serial Terminal</b>"));
}

void SenOpticalFlowSensorsMainWindow::initActionsConnections()
{
////

    connect(mySenConsole, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
    connect(mySenConsole, SIGNAL(emitStepUp()), this, SLOT(stepUp()));
    connect(mySenConsole, SIGNAL(emitStepDown()), this, SLOT(stepDown()));
    connect(mySenConsole, SIGNAL(emitStepLeft()), this, SLOT(stepLeft()));
    connect(mySenConsole, SIGNAL(emitStepRight()), this, SLOT(stepRight()));

    connect(mySenConsole, SIGNAL(emitStep10Up()), this, SLOT(step10Up()));
    connect(mySenConsole, SIGNAL(emitStep10Down()), this, SLOT(step10Down()));
    connect(mySenConsole, SIGNAL(emitStep10Left()), this, SLOT(step10Left()));
    connect(mySenConsole, SIGNAL(emitStep10Right()), this, SLOT(step10Right()));

    connect(myMainWindow_ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPortPrepare()));
    connect(this, SIGNAL(emitOpenSerialOrder(SenSettingsDialog::Settings)), myDefaulSerialObject, SLOT(openSerialPort(SenSettingsDialog::Settings)));
    connect(myDefaulSerialObject, SIGNAL(emitSerialOpenedSetting()), this, SLOT(setSerialOpenedStatus()));
    connect(myDefaulSerialObject, SIGNAL(emitSerialOpenErrorSetting(QString)), this, SLOT(setSerialOpenErrorStatus(QString)));

    connect(myMainWindow_ui->actionDisconnect, SIGNAL(triggered()), myDefaulSerialObject, SLOT(closeSerialPort()));
    connect(myDefaulSerialObject, SIGNAL(emitSerialCloseSetting()), this, SLOT(setSerialCloseStatus()));

    connect(myDefaulSerialObject, SIGNAL(emitSerialDataToConsole(QByteArray)), this, SLOT(showDataInConsole(QByteArray)));
////
    connect(myMainWindow_ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(myMainWindow_ui->actionConfigure, SIGNAL(triggered()), mySenSettingsDialog, SLOT(show()));
    connect(myMainWindow_ui->actionClear, SIGNAL(triggered()), mySenConsole, SLOT(clear()));
    connect(myMainWindow_ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(myMainWindow_ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


//// All Below Added
void SenOpticalFlowSensorsMainWindow::writeData(const QByteArray &data)
{
    myDefaulSerialObject->mySerialPort->write(data);
}
void SenOpticalFlowSensorsMainWindow::setHorizontalPosition(int panValue)
{
    QByteArray setPanPosition;

    setPanPosition[0] = 'P';
    setPanPosition[1] = 'P';
    setPanPosition += QByteArray::number(-panValue, 10);
    setPanPosition += ' ';

    myDefaulSerialObject->mySerialPort->write(setPanPosition);
}

void SenOpticalFlowSensorsMainWindow::setVerticalPosition(int tiltValue)
{
    QByteArray setTiltPosition;
    setTiltPosition[0] = 'T';
    setTiltPosition[1] = 'P';
    setTiltPosition += QByteArray::number(tiltValue, 10);
    setTiltPosition += ' ';

    myDefaulSerialObject->mySerialPort->write(setTiltPosition);
}

void SenOpticalFlowSensorsMainWindow::stepUp()
{
    verticalQSpinBox->setValue(verticalQSpinBox->value() + 1);

    QByteArray stepUp;
    stepUp[0] = 'T';
    stepUp[1] = 'O';
    stepUp[2] = '1';
    stepUp[3] = ' ';

    myDefaulSerialObject->mySerialPort->write(stepUp);
}
void SenOpticalFlowSensorsMainWindow::stepDown()
{
    verticalQSpinBox->setValue(verticalQSpinBox->value() - 1);

    QByteArray stepDown;
    stepDown[0] = 'T';
    stepDown[1] = 'O';
    stepDown[2] = '-';
    stepDown[3] = '1';
    stepDown[4] = ' ';

    myDefaulSerialObject->mySerialPort->write(stepDown);
}
void SenOpticalFlowSensorsMainWindow::stepLeft()
{
    horizontalQSpinBox->setValue(horizontalQSpinBox->value() - 1);

    QByteArray stepLeft;
    stepLeft[0] = 'P';
    stepLeft[1] = 'O';
    stepLeft[2] = '1';
    stepLeft[3] = ' ';

    myDefaulSerialObject->mySerialPort->write(stepLeft);
}
void SenOpticalFlowSensorsMainWindow::stepRight()
{
    horizontalQSpinBox->setValue(horizontalQSpinBox->value() + 1);

    QByteArray stepRight;
    stepRight[0] = 'P';
    stepRight[1] = 'O';
    stepRight[2] = '-';
    stepRight[3] = '1';
    stepRight[4] = ' ';

    myDefaulSerialObject->mySerialPort->write(stepRight);
}
void SenOpticalFlowSensorsMainWindow::step10Up()
{
    verticalQSpinBox->setValue(verticalQSpinBox->value() + 10);

    QByteArray step10Up;
    step10Up[0] = 'T';
    step10Up[1] = 'O';
    step10Up[2] = '1';
    step10Up[3] = '0';
    step10Up[4] = ' ';

    myDefaulSerialObject->mySerialPort->write(step10Up);
}
void SenOpticalFlowSensorsMainWindow::step10Down()
{
    verticalQSpinBox->setValue(verticalQSpinBox->value() - 10);

    QByteArray step10Down;
    step10Down[0] = 'T';
    step10Down[1] = 'O';
    step10Down[2] = '-';
    step10Down[3] = '1';
    step10Down[4] = '0';
    step10Down[5] = ' ';

    myDefaulSerialObject->mySerialPort->write(step10Down);
}
void SenOpticalFlowSensorsMainWindow::step10Left()
{
    horizontalQSpinBox->setValue(horizontalQSpinBox->value() - 10);

    QByteArray step10Left;
    step10Left[0] = 'P';
    step10Left[1] = 'O';
    step10Left[2] = '1';
    step10Left[3] = '0';
    step10Left[4] = ' ';

    myDefaulSerialObject->mySerialPort->write(step10Left);
}
void SenOpticalFlowSensorsMainWindow::step10Right()
{
    horizontalQSpinBox->setValue(horizontalQSpinBox->value() + 10);

    QByteArray step10Right;
    step10Right[0] = 'P';
    step10Right[1] = 'O';
    step10Right[2] = '-';
    step10Right[3] = '1';
    step10Right[4] = '0';
    step10Right[5] = ' ';

    myDefaulSerialObject->mySerialPort->write(step10Right);
}

void SenOpticalFlowSensorsMainWindow::setSerialCloseStatus()
{
    mySenConsole->setEnabled(false);
    myMainWindow_ui->actionConnect->setEnabled(true);
    myMainWindow_ui->actionDisconnect->setEnabled(false);
    myMainWindow_ui->actionConfigure->setEnabled(true);
    myMainWindow_ui->statusBar->showMessage(tr("Disconnected"));
}

void SenOpticalFlowSensorsMainWindow::setSerialOpenedStatus()
{
    QByteArray fakeKeyCMDQuary = ((QString)"?").toLatin1();
    QByteArray fakeKeyResetInitial = ((QString)"r\n").toLatin1();

    QByteArray fakeKeyEnter = ((QString)"\n").toLatin1();

    myDefaulSerialObject->mySerialPort->write(fakeKeyEnter);

    mySenConsole->setEnabled(true);
    mySenConsole->setLocalEchoEnabled(mySettingInfo.localEchoEnabled);
    myMainWindow_ui->actionConnect->setEnabled(false);
    myMainWindow_ui->actionDisconnect->setEnabled(true);
    myMainWindow_ui->actionConfigure->setEnabled(false);
    myMainWindow_ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                               .arg(mySettingInfo.name).arg(mySettingInfo.stringBaudRate).arg(mySettingInfo.stringDataBits)
                               .arg(mySettingInfo.stringParity).arg(mySettingInfo.stringStopBits).arg(mySettingInfo.stringFlowControl));

//    myDefaulSerialObject->mySerialPort->write(fakeKeyResetInitial);
}

void SenOpticalFlowSensorsMainWindow::setSerialOpenErrorStatus(QString serialOpenErrorString)
{
    QMessageBox::critical(this, tr("Error"), serialOpenErrorString);
    myMainWindow_ui->statusBar->showMessage(tr("Open error"));
}

void SenOpticalFlowSensorsMainWindow::setSerialCriticalErrorStatus(QString serialCriticalErrorString)
{
    QMessageBox::critical(this, tr("Critical Error"), serialCriticalErrorString);
    myMainWindow_ui->statusBar->showMessage(tr("Critical Error"));
}

void SenOpticalFlowSensorsMainWindow::openSerialPortPrepare()
{
    mySettingInfo = mySenSettingsDialog->settings();

    emit emitOpenSerialOrder(mySettingInfo);
}

void SenOpticalFlowSensorsMainWindow::showDataInConsole(QByteArray data)
{
    mySenConsole->putData(data);
}
