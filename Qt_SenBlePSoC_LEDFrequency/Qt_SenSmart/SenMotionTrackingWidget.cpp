#include "SenMotionTrackingWidget.h"

SenMotionTrackingWidget::SenMotionTrackingWidget(QWidget *parent) : QWidget(parent)
  ,firstSensorPointF(0,0), secondSensorPointF(-10,-10)
  , thirdSensorPointF(-10,10), fourthSensorPointF(10,10)
  , railUpdateElapsedTimer(NULL)

//  /****  For Transfer Start  *********************/
////  ,metaImageWidget(NULL),firstImageWidegt(NULL),secondImageWidget(NULL)
//  ,metaSerialObject(NULL),myFirstSerialObject(NULL), mySecondSerialObject(NULL)//, senStopPreparationTime(50)
//  ,metaDongleDecodingObject(NULL), firstDglDecObject(NULL), secondDglDecObject(NULL)
//  ,bandPassInputMode(false) ,bandPassLow(0) ,bandPassHigh(0) ,bandPassJudge(0) ,bandPassSensorID(0)//  ,lastCommandNotComplete(false)
//  /****  For Transfer End  ************************/

{
    firstSensorPointPen = QPen(Qt::green);
    firstSensorPointPen.setWidth(8);

    secondSensorPointPen = QPen(QColor(255,255,0));
    secondSensorPointPen.setWidth(0);

    thirdSensorPointPen = QPen(QColor(196,10,254));
    thirdSensorPointPen.setWidth(0);

    fourthSensorPointPen = QPen(Qt::red);
    fourthSensorPointPen.setWidth(0);

    linePen = QPen(Qt::white);
    linePen.setWidth(0.1);

    horizontalLine.setLine(-1512, 0, 1512, 0);
    verticalLine.setLine(0, -1512, 0, 1512);


    railUpdateElapsedTimer = new QElapsedTimer;
    railUpdateElapsedTimer->start();

    this->resize(512,512);




//    /****  For Transfer Start  *********************************************************************************************/

//    dongleProtocol = new SenBleDongleTalkFundamentals;
//    qRegisterMetaType<SerialInfoGroup>();// For SerialSettingsInfo Structure store

//    mySenSettingsDialog = new SenSerialSettingDialog;
//    mySettingInfo = mySenSettingsDialog->settings();

//        //    myMainWindow_ui->actionConnect->setEnabled(true);
//        //    myMainWindow_ui->actionDisconnect->setEnabled(false);
//        //    myMainWindow_ui->actionQuit->setEnabled(true);

//    // Serial Objects from the first to the fourth
//    myFirstSerialObject = new SenSerialObject(0, 1);
//    mySecondSerialObject = new SenSerialObject(0, 2);

//        //// Image Burst Widget (from first to the fourth)
//        //    firstImageWidegt = new SenImageBurstWidget(0, 1);
//        //    firstImageWidegt->setWindowTitle(QString("1st Sen Pixels-Burst"));
//        //    firstImageWidegt->move(0,0);
//        //    firstImageWidegt->show();

//        //    secondImageWidget = new SenImageBurstWidget(0, 2);
//        //    secondImageWidget->setWindowTitle(QString("2nd Sen Pixels-Burst"));
//        //    secondImageWidget->move(maxWidth - 470,0);
//        ////    secondImageWidget->show();

//    firstDglDecObject = new SenDecodeCySmtProtocolPackageObject(0, 1);
//    secondDglDecObject = new SenDecodeCySmtProtocolPackageObject(0, 2);

//    /************  Macro Initial Function *************/
////    multiSerialsInitialAndOpen();

//    //  DonglePackageDecoding Thread
//    donglePackageDecodingThread = new QThread();
//    firstDglDecObject->moveToThread(donglePackageDecodingThread);
//    secondDglDecObject->moveToThread(donglePackageDecodingThread);
//    donglePackageDecodingThread->start();



//    // CREATE A CONTEXT MENU FOR TOGGLING TEXTURE
//    serialDialogMenu = new QMenu();
//    QAction *action = serialDialogMenu->addAction(QString("Serial Configure"));
//    connect(action, SIGNAL(triggered()), mySenSettingsDialog, SLOT(appear()));

//    openAssignedSerialPortIfChanged();
//    /****  For Transfer End  ******************************************************************************************/
}


///****  For Transfer Start  *********************/

//void SenMotionTrackingWidget::onSerialConfigureTrigered()
//{
//    qDebug() << "Hello! I am triggered!!!  ";
//}

//void SenMotionTrackingWidget::multiSerialsInitialAndOpen()
//{
//    metaSerialObject = myFirstSerialObject;
////    metaImageWidget = firstImageWidegt;
//    metaDongleDecodingObject = firstDglDecObject;
//    initMetaSerialConnections();//Difference between initialSerialOpen and SerialOpen
//    openMetaSerialPort((dongleProtocol->getFirstSerialInfo()).name);

//    metaSerialObject = mySecondSerialObject;
////    metaImageWidget = secondImageWidget;
//    metaDongleDecodingObject = secondDglDecObject;
//    initMetaSerialConnections();//Difference between initialSerialOpen and SerialOpen
////    openMetaSerialPort((dongleProtocol->getSecondSerialInfo()).name);

//    autoTimerAskDeviceID();
//}

//void SenMotionTrackingWidget::openMetaSerialPort(QString metaSerialInfoName)
//{
//    metaSerialObject->mySerialPort->setPortName(metaSerialInfoName);

//    metaSerialObject->mySerialPort->setBaudRate((dongleProtocol->getDefaultSerialInfo()).baudRate);
//    metaSerialObject->mySerialPort->setDataBits((dongleProtocol->getDefaultSerialInfo()).dataBits);
//    metaSerialObject->mySerialPort->setParity((dongleProtocol->getDefaultSerialInfo()).parity);
//    metaSerialObject->mySerialPort->setStopBits((dongleProtocol->getDefaultSerialInfo()).stopBits);
//    metaSerialObject->mySerialPort->setFlowControl((dongleProtocol->getDefaultSerialInfo()).flowControl);

//    if (metaSerialObject->mySerialPort->open(QIODevice::ReadWrite)) {
////        setSerialOpenedStatus();
//    } else {
//            QMessageBox::critical(this, tr("Error"), metaSerialObject->mySerialPort->errorString());
//    }
//}

//void SenMotionTrackingWidget::initMetaSerialConnections()
//{
//    connect(metaSerialObject, SIGNAL(emitSerialOpenErrorSetting(QString)), this, SLOT(setSerialOpenErrorStatus(QString)), Qt::QueuedConnection);
//    connect(metaSerialObject, SIGNAL(emitSerialCloseSetting()), this, SLOT(closeMultiSerialObjectsPorts()), Qt::QueuedConnection);

//    connect(metaSerialObject, SIGNAL(emitMotionTrackingStart(int)), this, SLOT(initialMotionFileRecordObjectFeature(int)), Qt::QueuedConnection);
//    connect(metaSerialObject, SIGNAL(emitMotionTrackingStop(qint64, int)), this, SLOT(reportMotionFileRecordObjectFeature(qint64, int)), Qt::QueuedConnection);

//    // metaSerialObject
//    connect(metaSerialObject, SIGNAL(emitMotionUpdateSignal(QByteArray,int, int)), this, SLOT(SenMotionUpdate(QByteArray,int,int)), Qt::QueuedConnection);
//    connect(metaSerialObject, SIGNAL(emitArduinoInitialization()), this, SLOT(initializationSlot()), Qt::QueuedConnection);

////    // metaImageWidegt
////    connect(metaSerialObject, SIGNAL(emitAllSerialImageBurstCommand()), this, SLOT(startAllSerialImageBurstCommand()), Qt::QueuedConnection);
////    connect(metaSerialObject, SIGNAL(emitBurstDataQByteArray(QByteArray, int)), metaImageWidget, SLOT(SenPixelsDataProcess(QByteArray, int)), Qt::QueuedConnection);
////    connect(metaImageWidget, SIGNAL(emitBurstBegunFalse()), metaSerialObject, SLOT(setBurstBegunFalse()));
////    connect(metaImageWidget, SIGNAL(emitBurstStopForGoodNext()), metaSerialObject, SLOT(stopForGoodNextActive()));

//    metaSerialObject->SenMotionByteArray.clear();

//    // metaDongleDecodingObject
//    connect(metaSerialObject, SIGNAL(emitToShowHexDataFromDongleOnly(QByteArray)), metaDongleDecodingObject, SLOT(slotShowHexDataToConsole(QByteArray)), Qt::QueuedConnection);
//    connect(metaSerialObject, SIGNAL(emitToShowDongleBasicResponseFormat(QByteArray)), metaDongleDecodingObject, SLOT(showBasicResponseFormat(QByteArray)), Qt::QueuedConnection);
//    connect(metaSerialObject, SIGNAL(emitToDecodeDonglePackage(QByteArray)), metaDongleDecodingObject, SLOT(decodeDongle(QByteArray)), Qt::QueuedConnection);

//    connect(metaDongleDecodingObject, SIGNAL(emitSendCmdInitBleStack()), this, SLOT(slotBleSendCmdInitBleStack()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitCmdComplete(uchar)), this, SLOT(processCmdCompleteSignal(uchar)), Qt::QueuedConnection);

//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdSetConnectionParameters()), this, SLOT(slotSendGapCmd_SetConnectionParameters()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdSetLocalDeviceSecurity()), this, SLOT(slotSendGapCmd_SetLocalDeviceSecurity()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdStartScan()), this, SLOT(slotSendGapCmd_StartScan()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmdStopScan()), this, SLOT(slotSendGapCmd_StopScan()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmd_ResolveAndSetPeerAddr()), this, SLOT(slotSendGapCmd_ResolveAndSetPeerAddr()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmd_EstablishConnection()), this, SLOT(slotSendGapCmd_EstablishConnection()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGattCmd_ExchangeGatt_MTU_Size()), this, SLOT(slotSendGattCmd_ExchangeGatt_MTU_Size()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGattCmd_DiscoverAllPrimaryServices()), this, SLOT(slotSendGattCmd_DiscoverAllPrimaryServices()), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int, int)), this, SLOT(slotSendGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int, int)), Qt::QueuedConnection);
//    connect(metaDongleDecodingObject, SIGNAL(emitGapCmd_UpdateConnectionParamResp(QByteArray, QByteArray)), this, SLOT(slotSendGapCmd_UpdateConnectionParamResp(QByteArray, QByteArray)), Qt::QueuedConnection);

//    connect(metaDongleDecodingObject, SIGNAL(emitJumpMotionUpdateSignal(QByteArray,int, int)), this, SLOT(SenMotionUpdate(QByteArray,int,int)), Qt::QueuedConnection);
////    connect(metaDongleDecodingObject, SIGNAL(emit300PixelsShowUp(QByteArray,int)), metaImageWidget, SLOT(SenPixelsDataProcess(QByteArray, int)), Qt::QueuedConnection);

//    connect(metaDongleDecodingObject, SIGNAL(emitGattCMD_MotionServiceActive()), this, SLOT(slotSL_GattCMD_MotionServiceActive()), Qt::QueuedConnection);

//    connect(metaDongleDecodingObject, SIGNAL(emitBandPassModeBegin(uchar, uchar, uchar, int)), this, SLOT(slotSerialBandPassModeBigin(uchar, uchar, uchar, int)), Qt::QueuedConnection);

//}


//void SenMotionTrackingWidget::writeDataToMultiSerialPorts(const QByteArray &data)
//{
//    metaSerialObject = myFirstSerialObject;
//    mySensorID = "1";
//    writeDataToMetaSerialPort(data);

//    metaSerialObject = mySecondSerialObject;
//    if (metaSerialObject->mySerialPort->isOpen())   {
//        mySensorID = "2";
//        writeDataToMetaSerialPort(data);
//    }else   metaSerialObject = myFirstSerialObject;

//}

//void SenMotionTrackingWidget::writeDataToMetaSerialPort(const QByteArray &data)
//{
////    metaSerialObject->mySerialPort->write(data);
//    if (bandPassInputMode)  {
//        switch(bandPassJudge)   {
//            case BandPassJudge_AfterGotAllServices: {
//                if (data[0] >= (bandPassLow + Sen_Ascii_Zero) && data[0] <= (bandPassHigh + Sen_Ascii_Zero))     {   // Constructor trigger
//                    SL_GattCMD_DiscoverAllCharacteristics_ByChoosenService(data[0] - Sen_Ascii_Zero, bandPassSensorID);
//                    bandPassInputMode = false;
//                    bandPassJudge = 0; bandPassSensorID = 0;
//                }else if (data[0] == 'q') {
//                    bandPassInputMode = false;
//                    bandPassJudge = 0; bandPassSensorID = 0;
//                }else   {
//                }
//            }break;

//            default:    {
//                bandPassInputMode = false;
//                bandPassJudge = 0;  bandPassSensorID = 0;
//            }break;
//        }

//    }else   {
//        if (data.length() != 0)   {

//            //******** BLE Added on Nov 30, 2015
//            if (data[0] == '~')     {   // Constructor trigger
//                sendCmd_Header();   sendCmd_GetDeviceId();
//            }else if (data[0] == '*') { // Initial Dongle
//                sendCmd_Header();   sendCmd_InitBleStack();
//            }else if (data[0] == '0') {
//                sendCmd_Header();   sendGapCmd_StartScan();
//            }else if (data[0] == '8') {
//                SL_GapCMD_TerminateConnection();
//            }else if (data[0] == '9') {
//                sendCmd_Header();   sendGapCmd_StopScan();

//            }else if (data[0] == '1') {
//                SL_GattCMD_MotionServiceActive();
//            }else if (data[0] == '4') {
//                SL_GattCMD_MotionServiceStop();
//            }else if (data[0] == '2') {
//                SL_GattCMD_ImageBurstServiceActive();
//            }else if (data[0] == '5') {
//                SL_GattCMD_ImageBurstServiceStop();
//            }

//            // Useless, just for testing
//            else if (data[0] == '@') {
//                sendCmd_Header();   sendCmd_GetRSSI();
//            }else {
//                metaSerialObject->mySerialPort->write(data);
//            }
//            // BLE Added on Nov 30, 2015 ********
//        }
//    }


//    if(metaSerialObject->motionReadBegun)   {
//        qint64 stopAt = metaSerialObject->myElapsedTimer->elapsed();
//        qDebug() << "Sensor" << mySensorID <<"Stop Request Sent at " << stopAt << "milliseconds";
//    }
//}


//void SenMotionTrackingWidget::autoTimerAskDeviceID()
//{
//    QByteArray fakeKeyPress = ((QString)FAKE_KEY_ASK_DEVICE_ID).toLatin1(); // start imageBurst
//    writeDataToMultiSerialPorts(fakeKeyPress);
//}

//void SenMotionTrackingWidget::initialMotionFileRecordObjectFeature(int serialObject_ID)
//{
//    if (serialObject_ID == 1)   {
//        metaSerialObject = myFirstSerialObject;
//    }else if (serialObject_ID == 2)   {
//        metaSerialObject = mySecondSerialObject;
//    }else   qDebug() <<"There must be something wrong with motionRecord serialObject_ID";
//    metaSerialObject->myElapsedTimer->restart();
//}

//void SenMotionTrackingWidget::reportMotionFileRecordObjectFeature(qint64 elapsedTime, int serialObject_ID)
//{
//    if (serialObject_ID == 1)   {
//        metaSerialObject = myFirstSerialObject;
//    }else if (serialObject_ID == 2)   {
//        metaSerialObject = mySecondSerialObject;
//    }
//}


//QByteArray SenMotionTrackingWidget::getMiddlePacket_LastReceivedHandleInfo()
//{
//    QByteArray lastReceivedHandleInfo;
//    metaDongleDecodingObject = firstDglDecObject;//Need further coding!
//    lastReceivedHandleInfo[0] = metaDongleDecodingObject->peerBD_Handle[metaDongleDecodingObject->establishedConnectionNum - 1].bdHandle;
//    lastReceivedHandleInfo[1] = metaDongleDecodingObject->peerBD_Handle[metaDongleDecodingObject->establishedConnectionNum - 1].attId;

//    return lastReceivedHandleInfo;
//}

//QByteArray SenMotionTrackingWidget::getTestPacket_InputServiceRange()
//{
//    QByteArray testServiceRange;

//    testServiceRange[0] = 0x01;
//    testServiceRange[1] = 0x00;
//    testServiceRange[2] = 0x07;
//    testServiceRange[3] = 0x00;// Just Test, Not useful

//    return testServiceRange;
//}

//QByteArray SenMotionTrackingWidget::getFinalChosenPacket_InputServiceRange(int inputServiceIndex, int sensorID)
//{
//    QByteArray inputServiceRange;
//    uchar deviceID;
////    CYBLE_DISC_SRVC_INFO_T metaDeviceServicesList;
//    if (sensorID = 1)   metaDongleDecodingObject = firstDglDecObject;
//    else if (sensorID = 2)  metaDongleDecodingObject = secondDglDecObject;
////    else if (sensorID = 3)  metaDongleDecodingObject = ;
////    else if (sensorID = 4)  metaDongleDecodingObject = ;
//    else qDebug() <<"Wrong sensorID passing ! Check it out!";


//    deviceID = metaDongleDecodingObject->peerBD_Handle[metaDongleDecodingObject->establishedConnectionNum - 1].bdHandle;

//    switch(deviceID)    {
//        case (uchar)Sen_First_Bluetooth_Device_ID:  {
//            metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->firstDeviceServicesList;
//        }break;
//        case (uchar)Sen_Second_Bluetooth_Device_ID: {   metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->secondDeviceServicesList;   }break;
//        case (uchar)Sen_Third_Bluetooth_Device_ID:  {   metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->thirdDeviceServicesList;    }break;
//        case (uchar)Sen_Fourth_Bluetooth_Device_ID: {   metaDongleDecodingObject->metaDeviceServicesList = metaDongleDecodingObject->fourthDeviceServicesList;   }break;
//    }

//    inputServiceRange[0] = dongleProtocol->get_L8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.startHandle));
//    inputServiceRange[1] = dongleProtocol->get_H8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.startHandle));
//    inputServiceRange[2] = dongleProtocol->get_L8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.endHandle));
//    inputServiceRange[3] = dongleProtocol->get_H8((uchar*)&(metaDongleDecodingObject->metaDeviceServicesList[inputServiceIndex - 1].range.endHandle));

//    return inputServiceRange;
//}

//void SenMotionTrackingWidget::setSerialOpenErrorStatus(QString serialOpenErrorString)
//{
//    QMessageBox::critical(this, tr("Error"), serialOpenErrorString);
//}


//void SenMotionTrackingWidget::closeMultiSerialObjectsPorts()
//{
//    SL_GapCMD_TerminateConnection();
//    SenDelay(2);
//    sendCmd_Header();
//    sendGapCmd_StopScan();
//    SenDelay(2);
//    qDebug() << "Quit triggered!";
//    // First
//    metaSerialObject = myFirstSerialObject;
//    setSerialCloseStatus();
//    qDebug() << "myFirstSerialObject Serial Close ed!";
//    //  Second
//    metaSerialObject = mySecondSerialObject;
//    setSerialCloseStatus();
//    qDebug() << "mySecondSerialObject Serial Close ed!";

//    myFirstSerialObject->stopPrepare();
//    qDebug() << "myFirstSerialObject stopPrepare ed!";
//    mySecondSerialObject->stopPrepare();
//    qDebug() << "mySecondSerialObject stopPrepare ed!";

////    SenDelay(senStopPreparationTime);
////    firstImageWidegt->senStopPrepare();
////    qDebug() << "firstImageWidegt stopPrepare ed!";
////    secondImageWidget->senStopPrepare();
////    qDebug() << "secondImageWidget stopPrepare ed!";

//    this->initialization();

//    ////
////    myMainWindow_ui->actionConnect->setEnabled(true);
////    myMainWindow_ui->actionDisconnect->setEnabled(false);
////    myMainWindow_ui->actionConfigure->setEnabled(true);
////    myMainWindow_ui->statusBar->showMessage(tr("Disconnected"));
//}

//void SenMotionTrackingWidget::setSerialCloseStatus()
//{
//    if (metaSerialObject->mySerialPort->isOpen())
//        metaSerialObject->mySerialPort->close();
//}

//void SenMotionTrackingWidget::SenDelay(int msec)
//{
//    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
//    while( QTime::currentTime() < dieTime )
//        qApp->processEvents(QEventLoop::AllEvents,msec);// 100 milliseconds
//}

//void SenMotionTrackingWidget::openAssignedSerialPortIfChanged()
//{
//    if (mySenSettingsDialog->checkCustomerActive())  {
//        metaSerialObject = myFirstSerialObject;
//        openMetaSerialPort((mySenSettingsDialog->settings()).name);//Open Unique Assigned Serial Port
//        autoTimerAskDeviceID();
//    }else   {
//        openMultiSerialObjectsPorts();
//    }
//}

//void SenMotionTrackingWidget::openMultiSerialObjectsPorts()
//{
//    metaSerialObject = myFirstSerialObject;
//    openMetaSerialPort((dongleProtocol->getFirstSerialInfo()).name);

//    metaSerialObject = mySecondSerialObject;
////    openMetaSerialPort((dongleProtocol->getSecondSerialInfo()).name);

//    autoTimerAskDeviceID();
//}


///****  For Transfer End  ************************/

SenMotionTrackingWidget::~SenMotionTrackingWidget()
{
//    /****  For Transfer Start  ************************/

//    closeMultiSerialObjectsPorts();

//    SL_GapCMD_TerminateConnection();
//    SenDelay(4);
//    sendCmd_Header();
//    sendGapCmd_StopScan();
//    SenDelay(2);

//    delete myFirstSerialObject;
//    delete mySecondSerialObject;
////    delete metaSerialObject;

//    donglePackageDecodingThread->quit();
//    while(donglePackageDecodingThread->isRunning()){
//        qApp->processEvents();
//    }
//    delete donglePackageDecodingThread;
//    delete firstDglDecObject;
//    delete secondDglDecObject;

//    delete mySenSettingsDialog;
//    delete dongleProtocol;

//    /****  For Transfer End  ************************/


//    delete SenWriteFile;
//    delete SenReadFile;
    delete railUpdateElapsedTimer;
}


void SenMotionTrackingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());

    painter.fillRect(event->rect(), QColor(50, 50, 50));
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 768.0, side / 768.0);


    painter.setPen(linePen);
    painter.drawLine(verticalLine);
    painter.drawLine(horizontalLine);

    // from the first to the fourth
    painter.setPen(firstSensorPointPen);
    painter.setBrush(QBrush(Qt::green));
    painter.drawEllipse(firstSensorPointF, 1.5, 1.5);

    painter.end();
}

void SenMotionTrackingWidget::SenMotionUpdate(QByteArray syncSamplesByteArray, int byteArrayLength, int serialObject_ID)
{

    int accumulate_X = 0, accumulate_Y = 0;
    qint64 railUpdateTime = railUpdateElapsedTimer->elapsed();
    bool railUpdate = false;
    if (railUpdateTime >= 100) {
        railUpdate = true;
        railUpdateElapsedTimer->restart();
    }
    for (int i=0; i<byteArrayLength; )   {
                accumulate_Y += syncSamplesByteArray[i++];
                accumulate_X += syncSamplesByteArray[i++];
    }
//    qDebug() << "accumulate_X = " << accumulate_X << "accumulate_Y = " << accumulate_Y;

    // from the first to the fourth
    if (serialObject_ID == 1)   {
        firstSensorPointF.setX(firstSensorPointF.x() - accumulate_X);
        firstSensorPointF.setY(firstSensorPointF.y() + accumulate_Y);
    }else if (serialObject_ID == 2)    {
        secondSensorPointF.setX(secondSensorPointF.x() - accumulate_X);
        secondSensorPointF.setY(secondSensorPointF.y() + accumulate_Y);
    }else if (serialObject_ID == 3)    {
        thirdSensorPointF.setX(thirdSensorPointF.x() - accumulate_X);
        thirdSensorPointF.setY(thirdSensorPointF.y() + accumulate_Y);
    }else if (serialObject_ID == 4)    {
        fourthSensorPointF.setX(fourthSensorPointF.x() - accumulate_X);
        fourthSensorPointF.setY(fourthSensorPointF.y() + accumulate_Y);
    }else   qDebug() <<"There is something wrong with MotionUpdate serialObject_ID";

    if (railUpdate == true) {

        qDebug() << "Sensor 1 " << "position   X = " << firstSensorPointF.x() << ",   Y = " << firstSensorPointF.y();
//        qDebug() << " at railUpdateTime = " << railUpdateTime;
        railUpdate = false;
    }

    update();
}












//SenMotionTrackingWidget::SenMotionTrackingWidget(QWidget *parent) : QWidget(parent)
//  ,firstSensorPointF(10,-10), secondSensorPointF(-10,-10)
//  , thirdSensorPointF(-10,10), fourthSensorPointF(10,10)
//  , railUpdateElapsedTimer(NULL)
//{
//    firstSensorPointPen = QPen(Qt::green);
//    firstSensorPointPen.setWidth(4);

//    secondSensorPointPen = QPen(QColor(255,255,0));
//    secondSensorPointPen.setWidth(4);

//    thirdSensorPointPen = QPen(QColor(196,10,254));
//    thirdSensorPointPen.setWidth(4);

//    fourthSensorPointPen = QPen(Qt::red);
//    fourthSensorPointPen.setWidth(4);

//    railUpdateElapsedTimer = new QElapsedTimer;
//    railUpdateElapsedTimer->start();

//    this->resize(512,512);
//}

//SenMotionTrackingWidget::~SenMotionTrackingWidget()
//{
////    delete SenWriteFile;
////    delete SenReadFile;
//    delete railUpdateElapsedTimer;
//}


//void SenMotionTrackingWidget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter;
//    painter.begin(this);
//    painter.setRenderHint(QPainter::Antialiasing);

//    int side = qMin(width(), height());

//    painter.fillRect(event->rect(), QColor(50, 50, 50));
//    painter.translate(width() / 2, height() / 2);
//    painter.scale(side / 512.0, side / 512.0);

//    // from the first to the fourth
//    painter.setPen(firstSensorPointPen);
//    painter.setBrush(QBrush(Qt::green));
//    painter.drawEllipse(firstSensorPointF, 1.5, 1.5);

//    painter.setPen(secondSensorPointPen);
//    painter.setBrush(QBrush(QColor(255,255,0)));
//    painter.drawEllipse(secondSensorPointF, 1.5, 1.5);

//    painter.setPen(thirdSensorPointPen);
//    painter.setBrush(QBrush(QColor(255,255,0)));
//    painter.drawEllipse(thirdSensorPointF, 1.5, 1.5);

//    painter.setPen(fourthSensorPointPen);
//    painter.setBrush(QBrush(QColor(255,255,0)));
//    painter.drawEllipse(fourthSensorPointF, 1.5, 1.5);

//    painter.end();
//}

//void SenMotionTrackingWidget::SenMotionUpdate(QByteArray syncSamplesByteArray, int byteArrayLength, int serialObject_ID)
//{

//    int accumulate_X = 0, accumulate_Y = 0;
//    qint64 railUpdateTime = railUpdateElapsedTimer->elapsed();
//    bool railUpdate = false;
//    if (railUpdateTime >= 100) {
//        railUpdate = true;
//        railUpdateElapsedTimer->restart();
//    }
//    for (int i=0; i<byteArrayLength; )   {
//        accumulate_X += syncSamplesByteArray[i++];
//        accumulate_Y += syncSamplesByteArray[i++];
//    }
//    qDebug() << "accumulate_X = " << accumulate_X << "accumulate_Y = " << accumulate_Y;

//    // from the first to the fourth
//    if (serialObject_ID == 1)   {
//        firstSensorPointF.setX(firstSensorPointF.x() - accumulate_X);
//        firstSensorPointF.setY(firstSensorPointF.y() + accumulate_Y);
//    }else if (serialObject_ID == 2)    {
//        secondSensorPointF.setX(secondSensorPointF.x() - accumulate_X);
//        secondSensorPointF.setY(secondSensorPointF.y() + accumulate_Y);
//    }else if (serialObject_ID == 3)    {
//        thirdSensorPointF.setX(thirdSensorPointF.x() - accumulate_X);
//        thirdSensorPointF.setY(thirdSensorPointF.y() + accumulate_Y);
//    }else if (serialObject_ID == 4)    {
//        fourthSensorPointF.setX(fourthSensorPointF.x() - accumulate_X);
//        fourthSensorPointF.setY(fourthSensorPointF.y() + accumulate_Y);
//    }else   qDebug() <<"There is something wrong with MotionUpdate serialObject_ID";

//    if (railUpdate == true) {

////        qDebug() << "Sensor 1 " << "position   X = " << firstSensorPointF.x() << ",   Y = " << firstSensorPointF.y();
////        qDebug() << " at railUpdateTime = " << railUpdateTime;
//        railUpdate = false;
//    }

//    update();
//}





