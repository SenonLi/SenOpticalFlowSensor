#include "SenOpticalFlowSensorsMainWindow.h"
#include "ui_SenOpticalFlowSensorsMainWindow.h"

SenOpticalFlowSensorsMainWindow::SenOpticalFlowSensorsMainWindow(QWidget *parent, QString OF_SensorID) :
  QMainWindow(parent),metaImageWidget(NULL),firstImageWidegt(NULL),secondImageWidget(NULL)
  ,metaSerialObject(NULL),myFirstSerialObject(NULL), mySecondSerialObject(NULL), senStopPreparationTime(50)
  ,metaDongleDecodingObject(NULL), firstDglDecObject(NULL), secondDglDecObject(NULL)
  ,lastCommandNotComplete(false) ,bandPassInputMode(false) ,bandPassLow(0) ,bandPassHigh(0) ,bandPassJudge(0) ,bandPassSensorID(0)
  ,myMainWindow_ui(new Ui::SenOpticalFlowSensorsMainWindow)
{
    myMainWindow_ui->setupUi(this);
    QRect screenRect = QApplication::desktop()->screenGeometry() ;

    int maxWidth = screenRect.width();
    mySensorID = OF_SensorID;

    dongleProtocol = new SenBleDongleTalkFundamentals;

////
    qRegisterMetaType<SerialInfoGroup>();// For SerialSettingsInfo Structure store
////
    mySenSettingsDialog = new SenSerialSettingDialog;
    mySettingInfo = mySenSettingsDialog->settings();

    myMainWindow_ui->actionConnect->setEnabled(true);
    myMainWindow_ui->actionDisconnect->setEnabled(false);
    myMainWindow_ui->actionQuit->setEnabled(true);
    myMainWindow_ui->actionConfigure->setEnabled(true);

// to Set centralWidget
    // Motion Tracking Widget
    myMotionTrackingWidget = new SenMotionTrackingWidget;
    myMotionTrackingWidget->setWindowTitle(OF_SensorID + QString("Sen Motion Tracking"));

    // SenConsole
    mySenConsole = new SenConsole;
    mySenConsole->setEnabled(false);

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(mySenConsole, 0,0,1,1);
    centralLayout->addWidget(myMotionTrackingWidget, 0,1,1,1);
    mySenConsole->resize(825,512);
    myMotionTrackingWidget->setFixedSize(1512,512);

    QWidget *myCentralWidget = new QWidget;
    myCentralWidget->setLayout(centralLayout);
    setCentralWidget(myCentralWidget);

// Serial Objects from the first to the fourth
    myFirstSerialObject = new SenSerialObject(0, 1);
    mySecondSerialObject = new SenSerialObject(0, 2);

// Image Burst Widget (from first to the fourth)
    firstImageWidegt = new SenImageBurstWidget(0, 1);
    firstImageWidegt->setWindowTitle(QString("1st Sen Pixels-Burst"));
    firstImageWidegt->move(0,0);
    firstImageWidegt->show();

    secondImageWidget = new SenImageBurstWidget(0, 2);
    secondImageWidget->setWindowTitle(QString("2nd Sen Pixels-Burst"));
    secondImageWidget->move(maxWidth - 470,0);
//    secondImageWidget->show();

    firstDglDecObject = new SenDecodeCySmtProtocolPackageObject(0, 1);
    secondDglDecObject = new SenDecodeCySmtProtocolPackageObject(0, 2);

    /************  Macro Initial Function *************/
    multiSerialsInitialAndOpen();

//  DonglePackageDecoding     Thread
    donglePackageDecodingThread = new QThread();
    firstDglDecObject->moveToThread(donglePackageDecodingThread);
    secondDglDecObject->moveToThread(donglePackageDecodingThread);
    donglePackageDecodingThread->start();

}

SenOpticalFlowSensorsMainWindow::~SenOpticalFlowSensorsMainWindow()
{
    SL_GapCMD_TerminateConnection();
    SenDelay(4);
    sendCmd_Header();
    sendGapCmd_StopScan();
    SenDelay(2);

    delete myFirstSerialObject;
    delete mySecondSerialObject;
//    delete metaSerialObject;

    donglePackageDecodingThread->quit();
    while(donglePackageDecodingThread->isRunning()){
        qApp->processEvents();
    }
    delete donglePackageDecodingThread;
    delete firstDglDecObject;
    delete secondDglDecObject;

    delete mySenSettingsDialog;
    delete dongleProtocol;
    delete myMainWindow_ui;
}

void SenOpticalFlowSensorsMainWindow::autoTimerAskDeviceID()
{
    QByteArray fakeKeyPress = ((QString)FAKE_KEY_ASK_DEVICE_ID).toLatin1(); // start imageBurst
    writeDataToMultiSerialPorts(fakeKeyPress);
}

void SenOpticalFlowSensorsMainWindow::initUI_Connections()
{
//    connect(myMainWindow_ui->actionConnect, SIGNAL(triggered()), this, SLOT(openMultiSerialObjectsPorts()));

    connect(myMainWindow_ui->actionConnect, SIGNAL(triggered()), this, SLOT(openAssignedSerialPortIfChanged()));


    connect(myMainWindow_ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeMultiSerialObjectsPorts()), Qt::QueuedConnection);
    connect(mySenConsole, SIGNAL(getData(QByteArray)), this, SLOT(writeDataToMultiSerialPorts(QByteArray)));

    connect(myMainWindow_ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(myMainWindow_ui->actionConfigure, SIGNAL(triggered()), mySenSettingsDialog, SLOT(appear()));

    connect(myMainWindow_ui->actionClear, SIGNAL(triggered()), mySenConsole, SLOT(clear()));
    connect(myMainWindow_ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(myMainWindow_ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void SenOpticalFlowSensorsMainWindow::initMetaSerialConnections()
{
    connect(metaSerialObject, SIGNAL(emitSerialOpenedSetting()), this, SLOT(setSerialOpenedStatus()), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitSerialOpenErrorSetting(QString)), this, SLOT(setSerialOpenErrorStatus(QString)), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitSerialCloseSetting()), this, SLOT(closeMultiSerialObjectsPorts()), Qt::QueuedConnection);

    connect(metaSerialObject, SIGNAL(emitSerialDataToConsole(QByteArray, int)), this, SLOT(showDataInConsole(QByteArray, int)), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitMotionTrackingStart(int)), this, SLOT(initialMotionFileRecordObjectFeature(int)), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitMotionTrackingStop(qint64, int)), this, SLOT(reportMotionFileRecordObjectFeature(qint64, int)), Qt::QueuedConnection);

    connect(metaSerialObject, SIGNAL(emitAllSerialImageBurstCommand()), this, SLOT(startAllSerialImageBurstCommand()), Qt::QueuedConnection);

    // metaSerialObject
    connect(metaSerialObject, SIGNAL(emitMotionUpdateSignal(QByteArray,int, int)), myMotionTrackingWidget, SLOT(SenMotionUpdate(QByteArray,int,int)), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitArduinoInitialization()), myMotionTrackingWidget, SLOT(initializationSlot()), Qt::QueuedConnection);

    // metaImageWidegt
    connect(metaSerialObject, SIGNAL(emitBurstDataQByteArray(QByteArray, int)), metaImageWidget, SLOT(SenPixelsDataProcess(QByteArray, int)), Qt::QueuedConnection);
    connect(metaImageWidget, SIGNAL(emitBurstBegunFalse()), metaSerialObject, SLOT(setBurstBegunFalse()));
    connect(metaImageWidget, SIGNAL(emitBurstStopForGoodNext()), metaSerialObject, SLOT(stopForGoodNextActive()));

    metaSerialObject->SenMotionByteArray.clear();

    // metaDongleDecodingObject
    connect(metaSerialObject, SIGNAL(emitToShowHexDataFromDongleOnly(QByteArray)), metaDongleDecodingObject, SLOT(slotShowHexDataToConsole(QByteArray)), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitToShowDongleBasicResponseFormat(QByteArray)), metaDongleDecodingObject, SLOT(showBasicResponseFormat(QByteArray)), Qt::QueuedConnection);
    connect(metaSerialObject, SIGNAL(emitToDecodeDonglePackage(QByteArray)), metaDongleDecodingObject, SLOT(decodeDongle(QByteArray)), Qt::QueuedConnection);

    connect(metaDongleDecodingObject, SIGNAL(emitDataToConsole(QByteArray, int)), this, SLOT(showDataInConsole(QByteArray, int)), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitSendCmdInitBleStack()), this, SLOT(slotBleSendCmdInitBleStack()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitCmdComplete(uchar)), this, SLOT(processCmdCompleteSignal(uchar)), Qt::QueuedConnection);

    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdSetConnectionParameters()), this, SLOT(slotSendGapCmd_SetConnectionParameters()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdSetLocalDeviceSecurity()), this, SLOT(slotSendGapCmd_SetLocalDeviceSecurity()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdStartScan()), this, SLOT(slotSendGapCmd_StartScan()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdStopScan()), this, SLOT(slotSendGapCmd_StopScan()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGapCmd_ResolveAndSetPeerAddr()), this, SLOT(slotSendGapCmd_ResolveAndSetPeerAddr()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGapCmd_EstablishConnection()), this, SLOT(slotSendGapCmd_EstablishConnection()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGattCmd_ExchangeGatt_MTU_Size()), this, SLOT(slotSendGattCmd_ExchangeGatt_MTU_Size()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGattCmd_DiscoverAllPrimaryServices()), this, SLOT(slotSendGattCmd_DiscoverAllPrimaryServices()), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int, int)), this, SLOT(slotSendGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int, int)), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emitGapCmd_UpdateConnectionParamResp(QByteArray, QByteArray)), this, SLOT(slotSendGapCmd_UpdateConnectionParamResp(QByteArray, QByteArray)), Qt::QueuedConnection);

    connect(metaDongleDecodingObject, SIGNAL(emitJumpMotionUpdateSignal(QByteArray,int, int)), myMotionTrackingWidget, SLOT(SenMotionUpdate(QByteArray,int,int)), Qt::QueuedConnection);
    connect(metaDongleDecodingObject, SIGNAL(emit300PixelsShowUp(QByteArray,int)), metaImageWidget, SLOT(SenPixelsDataProcess(QByteArray, int)), Qt::QueuedConnection);

    connect(metaDongleDecodingObject, SIGNAL(emitGattCMD_MotionServiceActive()), this, SLOT(slotSL_GattCMD_MotionServiceActive()), Qt::QueuedConnection);


    connect(metaDongleDecodingObject, SIGNAL(emitBandPassModeBegin(uchar, uchar, uchar, int)), this, SLOT(slotSerialBandPassModeBigin(uchar, uchar, uchar, int)), Qt::QueuedConnection);

}

void SenOpticalFlowSensorsMainWindow::processCmdCompleteSignal(uchar event_ID)
{
    lastCommandNotComplete = false;
}

void SenOpticalFlowSensorsMainWindow::SenBleSendCmdInitBleStack()
{
    while (lastCommandNotComplete){
        qApp->processEvents();
    }
    sendCmd_Header();
    sendCmd_InitBleStack();
}

void SenOpticalFlowSensorsMainWindow::multiSerialsInitialAndOpen()
{
    initUI_Connections();
    metaSerialObject = myFirstSerialObject;
    metaImageWidget = firstImageWidegt;
    metaDongleDecodingObject = firstDglDecObject;
    initMetaSerialConnections();//Difference between initialSerialOpen and SerialOpen
    openMetaSerialPort((dongleProtocol->getFirstSerialInfo()).name);

    metaSerialObject = mySecondSerialObject;
    metaImageWidget = secondImageWidget;
    metaDongleDecodingObject = secondDglDecObject;
    initMetaSerialConnections();//Difference between initialSerialOpen and SerialOpen
//    openMetaSerialPort((dongleProtocol->getSecondSerialInfo()).name);

    autoTimerAskDeviceID();
}

void SenOpticalFlowSensorsMainWindow::openMultiSerialObjectsPorts()
{
    metaSerialObject = myFirstSerialObject;
    openMetaSerialPort((dongleProtocol->getFirstSerialInfo()).name);

    metaSerialObject = mySecondSerialObject;
//    openMetaSerialPort((dongleProtocol->getSecondSerialInfo()).name);

    autoTimerAskDeviceID();
}

void SenOpticalFlowSensorsMainWindow::closeMultiSerialObjectsPorts()
{
    SL_GapCMD_TerminateConnection();
    SenDelay(2);
    sendCmd_Header();
    sendGapCmd_StopScan();
    SenDelay(2);
    qDebug() << "Quit triggered!";
    // First
    metaSerialObject = myFirstSerialObject;
    setSerialCloseStatus();
    qDebug() << "myFirstSerialObject Serial Close ed!";
    //  Second
    metaSerialObject = mySecondSerialObject;
    setSerialCloseStatus();
    qDebug() << "mySecondSerialObject Serial Close ed!";

    myFirstSerialObject->stopPrepare();
    qDebug() << "myFirstSerialObject stopPrepare ed!";
    mySecondSerialObject->stopPrepare();
    qDebug() << "mySecondSerialObject stopPrepare ed!";

    SenDelay(senStopPreparationTime);

    firstImageWidegt->senStopPrepare();
    qDebug() << "firstImageWidegt stopPrepare ed!";
    secondImageWidget->senStopPrepare();
    qDebug() << "secondImageWidget stopPrepare ed!";

    myMotionTrackingWidget->initialization();

    ////
//    mySenConsole->setEnabled(false);
    myMainWindow_ui->actionConnect->setEnabled(true);
    myMainWindow_ui->actionDisconnect->setEnabled(false);
    myMainWindow_ui->actionConfigure->setEnabled(true);
    myMainWindow_ui->statusBar->showMessage(tr("Disconnected"));
}

void SenOpticalFlowSensorsMainWindow::openMetaSerialPort(QString metaSerialInfoName)
{
    metaSerialObject->mySerialPort->setPortName(metaSerialInfoName);

    metaSerialObject->mySerialPort->setBaudRate((dongleProtocol->getDefaultSerialInfo()).baudRate);
    metaSerialObject->mySerialPort->setDataBits((dongleProtocol->getDefaultSerialInfo()).dataBits);
    metaSerialObject->mySerialPort->setParity((dongleProtocol->getDefaultSerialInfo()).parity);
    metaSerialObject->mySerialPort->setStopBits((dongleProtocol->getDefaultSerialInfo()).stopBits);
    metaSerialObject->mySerialPort->setFlowControl((dongleProtocol->getDefaultSerialInfo()).flowControl);

    if (metaSerialObject->mySerialPort->open(QIODevice::ReadWrite)) {
        setSerialOpenedStatus();
    } else {
            QMessageBox::critical(this, tr("Error"), metaSerialObject->mySerialPort->errorString());
            myMainWindow_ui->statusBar->showMessage(tr("Open error"));
    }
}
QByteArray SenOpticalFlowSensorsMainWindow::getMiddlePacket_LastReceivedHandleInfo()
{
    QByteArray lastReceivedHandleInfo;
    metaDongleDecodingObject = firstDglDecObject;//Need further coding!
    lastReceivedHandleInfo[0] = metaDongleDecodingObject->peerBD_Handle[metaDongleDecodingObject->establishedConnectionNum - 1].bdHandle;
    lastReceivedHandleInfo[1] = metaDongleDecodingObject->peerBD_Handle[metaDongleDecodingObject->establishedConnectionNum - 1].attId;

    return lastReceivedHandleInfo;
}

QByteArray SenOpticalFlowSensorsMainWindow::getTestPacket_InputServiceRange()
{
    QByteArray testServiceRange;

    testServiceRange[0] = 0x01;
    testServiceRange[1] = 0x00;
    testServiceRange[2] = 0x07;
    testServiceRange[3] = 0x00;// Just Test, Not useful

    return testServiceRange;
}

QByteArray SenOpticalFlowSensorsMainWindow::getFinalChosenPacket_InputServiceRange(int inputServiceIndex, int sensorID)
{
    QByteArray inputServiceRange;
    uchar deviceID;
//    CYBLE_DISC_SRVC_INFO_T metaDeviceServicesList;
    if (sensorID = 1)   metaDongleDecodingObject = firstDglDecObject;
    else if (sensorID = 2)  metaDongleDecodingObject = secondDglDecObject;
//    else if (sensorID = 3)  metaDongleDecodingObject = ;
//    else if (sensorID = 4)  metaDongleDecodingObject = ;
    else qDebug() <<"Wrong sensorID passing ! Check it out!";


    deviceID = metaDongleDecodingObject->peerBD_Handle[metaDongleDecodingObject->establishedConnectionNum - 1].bdHandle;

    switch(deviceID)    {
        case (uchar)Sen_First_Bluetooth_Device_ID:  {
            metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->firstDeviceServicesList;
        }break;
        case (uchar)Sen_Second_Bluetooth_Device_ID: {   metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->secondDeviceServicesList;   }break;
        case (uchar)Sen_Third_Bluetooth_Device_ID:  {   metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->thirdDeviceServicesList;    }break;
        case (uchar)Sen_Fourth_Bluetooth_Device_ID: {   metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->fourthDeviceServicesList;   }break;
    }

    inputServiceRange[0] = dongleProtocol->get_L8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.startHandle));
    inputServiceRange[1] = dongleProtocol->get_H8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.startHandle));
    inputServiceRange[2] = dongleProtocol->get_L8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.endHandle));
    inputServiceRange[3] = dongleProtocol->get_H8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.endHandle));

    return inputServiceRange;
}

void SenOpticalFlowSensorsMainWindow::writeDataToMetaSerialPort(const QByteArray &data)
{
//    metaSerialObject->mySerialPort->write(data);
    if (bandPassInputMode)  {
        switch(bandPassJudge)   {
            case BandPassJudge_AfterGotAllServices: {
                if (data[0] >= (bandPassLow + Sen_Ascii_Zero) && data[0] <= (bandPassHigh + Sen_Ascii_Zero))     {   // Constructor trigger
                    SL_GattCMD_DiscoverAllCharacteristics_ByChoosenService(data[0] - Sen_Ascii_Zero, bandPassSensorID);
                    bandPassInputMode = false;
                    bandPassJudge = 0; bandPassSensorID = 0;
                }else if (data[0] == 'q') {
                    bandPassInputMode = false;
                    bandPassJudge = 0; bandPassSensorID = 0;
                    mySenConsole->putData((QString("\rQuit Band Pass Mode\r")).toLatin1(), 1);
                }else   {
                    mySenConsole->putData((QString("\r\rWrong Input ! Please input between    ")).toLatin1(), 1);
                    mySenConsole->putData(QString::number(bandPassLow).toLatin1(), 1);
                    mySenConsole->putData((QString("   and   ")).toLatin1(), 1);
                    mySenConsole->putData(QString::number(bandPassHigh).toLatin1(), 1);
                    mySenConsole->putData((QString("\n\n")).toLatin1(), 1);
                }
            }break;

            default:    {
                mySenConsole->putData((QString("\rWrong bandPassJudge, check it out! \r")).toLatin1(), 1);
                bandPassInputMode = false;
                bandPassJudge = 0;  bandPassSensorID = 0;
            }break;
        }

    }else   {
        if (data.length() != 0)   {

            //******** BLE Added on Nov 30, 2015
            if (data[0] == '~')     {   // Constructor trigger
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                sendCmd_Header();   sendCmd_GetDeviceId();
            }else if (data[0] == '*') { // Initial Dongle
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                sendCmd_Header();   sendCmd_InitBleStack();
            }else if (data[0] == '0') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                sendCmd_Header();   sendGapCmd_StartScan();
            }else if (data[0] == '8') {
                while (lastCommandNotComplete)  {
                    qApp->processEvents();
                }
                SL_GapCMD_TerminateConnection();
            }else if (data[0] == '9') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                sendCmd_Header();   sendGapCmd_StopScan();

            }else if (data[0] == '1') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                SL_GattCMD_MotionServiceActive();
            }else if (data[0] == '4') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                SL_GattCMD_MotionServiceStop();
            }else if (data[0] == '2') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                SL_GattCMD_ImageBurstServiceActive();
            }else if (data[0] == '5') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                SL_GattCMD_ImageBurstServiceStop();
            }

            // Useless, just for testing
            else if (data[0] == '@') {
                while (lastCommandNotComplete)  { qApp->processEvents(); }
                sendCmd_Header();   sendCmd_GetRSSI();
            }else {
                metaSerialObject->mySerialPort->write(data);
            }
            // BLE Added on Nov 30, 2015 ********
        }
    }


    if(metaSerialObject->motionReadBegun)   {
        qint64 stopAt = metaSerialObject->myElapsedTimer->elapsed();
        qDebug() << "Sensor" << mySensorID <<"Stop Request Sent at " << stopAt << "milliseconds";
    }
}

void SenOpticalFlowSensorsMainWindow::writeDataToMultiSerialPorts(const QByteArray &data)
{
    metaSerialObject = myFirstSerialObject;
    mySensorID = "1";
    writeDataToMetaSerialPort(data);

    metaSerialObject = mySecondSerialObject;
    if (metaSerialObject->mySerialPort->isOpen())   {
        mySensorID = "2";
        writeDataToMetaSerialPort(data);
    }else   metaSerialObject = myFirstSerialObject;

}

void SenOpticalFlowSensorsMainWindow::startAllSerialImageBurstCommand()
{
    QByteArray fakeKeyPress = ((QString)FAKE_KEY_IMAGE_BUTST_START).toLatin1(); // start imageBurst
    writeDataToMultiSerialPorts(fakeKeyPress);
}

void SenOpticalFlowSensorsMainWindow::setSerialCloseStatus()
{
    if (metaSerialObject->mySerialPort->isOpen())
        metaSerialObject->mySerialPort->close();
}

void SenOpticalFlowSensorsMainWindow::initialMotionFileRecordObjectFeature(int serialObject_ID)
{
    if (serialObject_ID == 1)   {
        mySenConsole->putData((QString("\r1st Motion track Start -->\r")).toLatin1(), serialObject_ID);
        metaSerialObject = myFirstSerialObject;
    }else if (serialObject_ID == 2)   {
        mySenConsole->putData((QString("\r2nd Motion Read Start -->\r")).toLatin1(), serialObject_ID);
        metaSerialObject = mySecondSerialObject;
    }else   qDebug() <<"There must be something wrong with motionRecord serialObject_ID";
    metaSerialObject->myElapsedTimer->restart();
}

void SenOpticalFlowSensorsMainWindow::reportMotionFileRecordObjectFeature(qint64 elapsedTime, int serialObject_ID)
{
    if (serialObject_ID == 1)   {
        mySenConsole->putData((QString("\r<-- 1st Motion Burst Stop\r")).toLatin1(), serialObject_ID);
        metaSerialObject = myFirstSerialObject;
    }else if (serialObject_ID == 2)   {
        mySenConsole->putData((QString("\r<-- 2nd Motion Burst Stop\r")).toLatin1(), serialObject_ID);
        metaSerialObject = mySecondSerialObject;
    }
}

void SenOpticalFlowSensorsMainWindow::setSerialOpenedStatus()
{
    mySenConsole->setEnabled(true);
    mySenConsole->setLocalEchoEnabled(mySettingInfo.localEchoEnabled);
    myMainWindow_ui->actionConnect->setEnabled(false);
    myMainWindow_ui->actionDisconnect->setEnabled(true);
    myMainWindow_ui->actionConfigure->setEnabled(true);
    myMainWindow_ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                               .arg(mySettingInfo.name).arg(mySettingInfo.stringBaudRate).arg(mySettingInfo.stringDataBits)
                               .arg(mySettingInfo.stringParity).arg(mySettingInfo.stringStopBits).arg(mySettingInfo.stringFlowControl));
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

void SenOpticalFlowSensorsMainWindow::showDataInConsole(QByteArray data, int colorNumber)
{
    mySenConsole->putData(data, colorNumber);
}

void SenOpticalFlowSensorsMainWindow::about()
{
    QMessageBox::about(this, tr("About Sen Serial Terminal"),
                       tr("The <b>Sen Serial Terminal</b>"));
}

void SenOpticalFlowSensorsMainWindow::SenDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
        qApp->processEvents(QEventLoop::AllEvents,msec);// 100 milliseconds
}

void SenOpticalFlowSensorsMainWindow::openAssignedSerialPortIfChanged()
{
    if (mySenSettingsDialog->checkCustomerActive())  {
        metaSerialObject = myFirstSerialObject;
        openMetaSerialPort((mySenSettingsDialog->settings()).name);//Open Unique Assigned Serial Port
        autoTimerAskDeviceID();
    }else   {
        openMultiSerialObjectsPorts();
    }
}

