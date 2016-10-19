#include "SenSerialObject.h"

SenSerialObject::SenSerialObject(QObject *parent, int givenSerialNum)
    : QObject(parent)   ,pixelsBurstBegun(false), motionReadBegun(false), initialPixelsBurst(false)
  ,emitCount(0), linePixelsDataQString(""), syncNumMotionsCount(0)
  ,stopForGoodNext(false),serialReadPixelsBurstMax(900),pixelsBurstSum(0), myElapsedTimer(NULL)
  ,debugError(false), pixelsUpdatePerTime(180)
  ,evtHeaderByteSize(2), evtLenInfoOccupation(2)
{
    serialObject_ID = givenSerialNum;
    mySerialPort = new QSerialPort();
    myElapsedTimer = new QElapsedTimer;

    motionReadBytes = 2 * motionReadSyncNum + 1;
    motionReadPerTime = 2 * motionReadSyncNum + 1;

    dongleProtocol = new SenBleDongleTalkFundamentals;
    dongleEvtHeader_Low  = dongleProtocol->getHeaderFromArray_Low(DONGLE_EVENT_HEADER);
    dongleEvtHeader_High = dongleProtocol->getHeaderFromArray_High(DONGLE_EVENT_HEADER);

    connect(mySerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(mySerialPort, SIGNAL(readyRead()), this, SLOT(readBleDongle()));

    SenPixelsBurstByteArray.clear();//
    SenLineByteArray.clear();//
    dongleByteArrary.clear();
    SenLineString = "";
}

SenSerialObject::~SenSerialObject()
{
    delete dongleProtocol;
    delete mySerialPort;
    delete myElapsedTimer;
}


void SenSerialObject::openSerialPort(SerialInfoGroup mySettingInfo)
{
    mySerialPort->setPortName(mySettingInfo.name);
    mySerialPort->setBaudRate(mySettingInfo.baudRate);
    mySerialPort->setDataBits(mySettingInfo.dataBits);
    mySerialPort->setParity(mySettingInfo.parity);
    mySerialPort->setStopBits(mySettingInfo.stopBits);
    mySerialPort->setFlowControl(mySettingInfo.flowControl);

    if (mySerialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Error 0 means no error and the SerialPort was openned successfully!";
        emit emitSerialOpenedSetting();
        syncNumMotionsCount = 0;
        SenMotionByteArray.clear();
        debugError = false;
    } else {
        emit emitSerialOpenErrorSetting(mySerialPort->errorString());
    }
}

void SenSerialObject::closeSerialPort()
{
//    if (mySerialPort->isOpen())
//        mySerialPort->close();
//    SenDelay(1000);
    QByteArray fakeKeyPress = ((QString)"q").toLatin1();
    mySerialPort->write(fakeKeyPress);

//    emit emitSerialCloseSetting();
}

void SenSerialObject::readBleDongle()
{
    QByteArray donglePackage = mySerialPort->readAll();
//    emit emitToShowHexDataFromDongleOnly(donglePackage);
    emit emitToDecodeDonglePackage(donglePackage);


//    if(debugError)  {
//        QByteArray data = mySerialPort->readAll();
//        emit emitSerialDataToConsole(data, 0);// int: 0(Red) for the Error status
//    }else   {
//        showBasicResponseFormat();
////        dongleDecode();
//    }
}


void SenSerialObject::handleError(QSerialPort::SerialPortError error)
{
    qDebug() << "error: " << error << " and errorString: " << mySerialPort->errorString();
    if (error == QSerialPort::ResourceError) {
        emit emitSerialCriticalErrorSetting(mySerialPort->errorString());
        closeSerialPort();
    }
}

void SenSerialObject::SenDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents,msec); // 100 milliseconds
}

void SenSerialObject::SenDeadDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
     qDebug() << "I am dead, please wait........................"; // 100 milliseconds
}

void SenSerialObject::readData()
{
    if(debugError)  {
        QByteArray data = mySerialPort->readAll();
        emit emitSerialDataToConsole(data, 0);// int: 0(Red) for the Error status
    }else   {

    QString SenImageBurstStartString = "image data";
    QString SenImageBurstEndString = "----------";
    QString SenMotionBurstStartString = "SenMotionSprouting:";
    QString SenInitializationString = "Welcome to Sen PSoC delete me";
    QString SenInitialPixelsBurstString = "3, Sen Motion Burst.";
    QString SenStopRequestReceivedString = "I'm Sen Stop Request!";

    while(mySerialPort->bytesAvailable()){
//    while(!mySerialPort->atEnd()){
//        QByteArray SenPixelsBurstByteArray;// = mySerialPort->read(serialReadPixelsBurstMax);
        SenPixelsBurstByteArray.clear();
        if (pixelsBurstBegun) {
            if (serialReadPixelsBurstMax >= pixelsUpdatePerTime) {
                SenPixelsBurstByteArray = mySerialPort->read(pixelsUpdatePerTime);
            }else   {
                SenPixelsBurstByteArray = mySerialPort->read(serialReadPixelsBurstMax);
            }

            serialReadPixelsBurstMax = serialReadPixelsBurstMax - SenPixelsBurstByteArray.length();

            if(serialReadPixelsBurstMax < 0)    {
                qDebug() << "Error!!! serialReadPixelsBurstMax Calculation Wrong!!! \n\r";
                while(1)    {
                    ;
                }
            }

            emit emitBurstDataQByteArray(SenPixelsBurstByteArray, ++emitCount);


            if(serialReadPixelsBurstMax == 0)    {
                serialReadPixelsBurstMax = 900;
                pixelsBurstBegun = false;
                emitCount = 0;
            }

//            if (mySenConsole->lineDebugActive) {
//                qDebug() << "SenPixelsBurstByteArray.length() = " << SenPixelsBurstByteArray.length() << "emitCount = " << emitCount;
//                pixelsBurstSum += SenPixelsBurstByteArray.length();
//                qDebug() << "pixelsBurstSum = " << pixelsBurstSum;
//            }
            ;
        }else if (motionReadBegun) {
                QByteArray thisMotionByteArray = mySerialPort->read(motionReadBytes);
                SenMotionByteArray += thisMotionByteArray;

                if (SenMotionByteArray.length() == motionReadPerTime)   {

                    unsigned char flag = SenMotionByteArray[SenMotionByteArray.length() - 1];
                    if (flag == 85) {
                        ;// Continue, motionReadBegun == true;
                    }else if (flag == 170)  {
//                    }else if (flag == 88)  {// for test
                        qint64 elapsedTime = myElapsedTimer->elapsed();
                        emit emitMotionTrackingStop(elapsedTime, serialObject_ID);
                        motionReadBegun = false;
                    }else   {
                        emit emitMotionUpdateSignal(SenMotionByteArray, SenMotionByteArray.length() - 1, false, serialObject_ID);
                        qDebug() << "sync Byte = "<< flag ;
                        qDebug() << "Failed At syncNum. = "<< syncNumMotionsCount + 1;
                        qDebug() << "motionReadBytes = "<< motionReadBytes ;
                        qDebug() << "thisTime.length() = " << thisMotionByteArray.length();
                        qDebug() << "All.length() = " << SenMotionByteArray.length();
                        qDebug() << "elapsedTime = "<< myElapsedTimer->elapsed() ;

                        SenMotionByteArray.clear();
                        while(!mySerialPort->atEnd())   {
                            for (int j=0; j<18;j++)    {
                                //                            int flagInt = ((QByteArray)mySerialPort->read(1))[0];
                                //                            QString flagString = QString::number(flagInt) + " ";
                                //                            emit emitSerialDataToConsole(flagString.toLatin1(),serialObject_ID);
                                QByteArray thisMotionByteArray = mySerialPort->read(motionReadBytes);
                                SenMotionByteArray += thisMotionByteArray;

                                if (SenMotionByteArray.length() == motionReadPerTime)   {
                                    unsigned char flag = SenMotionByteArray[SenMotionByteArray.length() - 1];
                                    qDebug() << "sync Byte = "<< flag ;
                                    emit emitMotionUpdateSignal(SenMotionByteArray, SenMotionByteArray.length() - 1, false, serialObject_ID);
                                    syncNumMotionsCount++;
                                    motionReadBytes = motionReadPerTime;
                                    SenMotionByteArray.clear();
                                }else if (SenMotionByteArray.length() < motionReadPerTime)  {
                                    qDebug() << "Splitted at syncNum. = " << syncNumMotionsCount + 1
                                             << "motionReadBytes = " << motionReadBytes
                                             << "thisMotionByteArray.length() = " << thisMotionByteArray.length()
                                             << "  and SenMotionByteArray.length() = " << SenMotionByteArray.length();
                                    motionReadBytes = motionReadPerTime - SenMotionByteArray.length();
                                }else   {
                                    qDebug() << "SenMotionByteArray.length() = " << SenMotionByteArray.length();
                                    qDebug() << "motionReadPerTime = " << motionReadPerTime;
                                    qDebug() << "SenMotionByteArray.length() - motionReadPerTime = " << SenMotionByteArray.length() - motionReadPerTime;
                                    while(1)    {
                                        ;
                                    }
                                }

                            }
                        }

//                        while(1)    {
                            motionReadBegun = false;
                            SenDelay(1000);
                            debugError = true;
//                        }
                    }


                    emit emitMotionUpdateSignal(SenMotionByteArray, SenMotionByteArray.length() - 1, serialObject_ID);
                    syncNumMotionsCount++;

                    motionReadBytes = motionReadPerTime;
                    SenMotionByteArray.clear();
                }else if (SenMotionByteArray.length() < motionReadPerTime)  {
//                    qDebug() << "Splitted at syncNum. = " << syncNumMotionsCount + 1
//                             << "thisMotionByteArray.length() = " << thisMotionByteArray.length()
//                             << "  and SenMotionByteArray.length() = " << SenMotionByteArray.length();
                    motionReadBytes = motionReadPerTime - SenMotionByteArray.length();

                }else   {
                    qDebug() << "Error: Motion Read have a big problem !!!";
                    while(1)    {
                        ;
                    }
                }

        }else   {
//             QByteArray SenLineByteArray = mySerialPort->readLine();
//             QString SenLineString = SenLineByteArray;
            SenLineByteArray = mySerialPort->readLine();
            SenLineString = SenLineByteArray;   SenLineByteArray.clear();

            linePixelsDataQString += SenLineString;

            if(linePixelsDataQString.length() != 0) {

                if(linePixelsDataQString[linePixelsDataQString.length() - 1] == '\n')  {
                    if (linePixelsDataQString.contains(SenImageBurstStartString, Qt::CaseSensitive))       {
                        emit emitSerialDataToConsole((QString("\rBurst Start -->\r")).toLatin1(), serialObject_ID);
                        pixelsBurstBegun = true; pixelsBurstSum = 0;
                    }else if (linePixelsDataQString.contains(SenImageBurstEndString, Qt::CaseSensitive))   {
                        emit emitSerialDataToConsole((QString("\r<-- Burst End \r")).toLatin1(), serialObject_ID);
                        if(stopForGoodNext)    {
                            emit emitSerialDataToConsole((QString("\rPrepare to Stop for a Good Next Frame.9 \r")).toLatin1(), serialObject_ID);
                            qDebug() << "Stop for Next good one." << emitCount;
                            QByteArray fakeKeyPress = ((QString)"q").toLatin1();
                            mySerialPort->write(fakeKeyPress);
                            stopForGoodNext = false;
                        }
                        pixelsBurstBegun = false; initialPixelsBurst = false;
                        emitCount = 0; pixelsBurstSum = 0;
                        //            qDebug() << "<--Burst End,   pixelsBurstBegun = " << pixelsBurstBegun << ",   emitCount = " << emitCount;
                    }else if (linePixelsDataQString.contains(SenMotionBurstStartString, Qt::CaseSensitive))   {
                        emit emitMotionTrackingStart(serialObject_ID);
                        motionReadBegun = true;
                    }else if (linePixelsDataQString.contains(SenInitializationString, Qt::CaseSensitive))   {
                        emit emitArduinoInitialization();
                        emit emitSerialDataToConsole(linePixelsDataQString.toLatin1(), 2);
                        qDebug() << "Ardiono Initialed";
                        initialPixelsBurst = true;
                    }else if (linePixelsDataQString.contains(SenInitialPixelsBurstString, Qt::CaseSensitive))   {
                        emit emitSerialDataToConsole(linePixelsDataQString.toLatin1(), serialObject_ID);
                        if (initialPixelsBurst)    {
                            qDebug() << "Pixels Burst Beginning...";
                            //                         QByteArray fakeKeyPress = ((QString)"0").toLatin1(); // start imageBurst
                            //                         mySerialPort->write(fakeKeyPress);
                            emit emitAllSerialImageBurstCommand();
                        }
                    }else if (linePixelsDataQString.contains(SenStopRequestReceivedString, Qt::CaseSensitive))   {
                        emit emitSerialCloseSetting();
                        emit emitSerialDataToConsole(linePixelsDataQString.toLatin1(), serialObject_ID);
                        //  qDebug() << "Arduino Initiated !";
                    }else   {
                        emit emitSerialDataToConsole(linePixelsDataQString.toLatin1(), serialObject_ID);
                    }
                    linePixelsDataQString = "";
                }
            }

        }// End of none-BurstBegun
    }// End of while(!mySerialPort->atEnd())

    }
}


void SenSerialObject::dongleDecode()
{
}
void SenSerialObject::generalEventHandler(ushort evtType, QByteArray evtPackageArray)
{

}
void SenSerialObject::showHexDataToConsole(QByteArray anyUnsignedData)
{

}
void SenSerialObject::showBasicResponseFormat()
{
}
void SenSerialObject::hexDataImmeResponseFromDongleToConsole(QByteArray rawData)
{
}





