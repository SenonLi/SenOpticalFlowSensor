#include "SenDecodeCySmtProtocolPackageObject.h"

SenDecodeCySmtProtocolPackageObject::SenDecodeCySmtProtocolPackageObject(QObject *parent, int OF_SensorID) : QObject(parent)
  ,nextRawDataPriority(PRIORITY_HEADER)
  ,resolveSetPeerAddAutoAfterScanStop(false) ,resolve_SetPeerAddGetGoodResponse(false)
  ,establishedConnectionNum(0) ,establishConnectionSuccess(false) ,chosenServiceIndex(-1)
  ,updateConnctionParaRequestSent(false), updateConnctionParaRequestSuccess(false), connectionParameterSuccessfullyUpdated(false)
  ,firstDeviceServicesCount(0), secondDeviceServicesCount(0), thirdDeviceServicesCount(0), fourthDeviceServicesCount(0)
{
    mySensorID = OF_SensorID;

    dongleProtocol = new SenBleDongleTalkFundamentals;
    dongleEvtHeader_Low  = dongleProtocol->getHeaderFromArray_Low(DONGLE_EVENT_HEADER);
    dongleEvtHeader_High = dongleProtocol->getHeaderFromArray_High(DONGLE_EVENT_HEADER);

    for (int i=0; i<Sen_BluetoothDevice_Maximum_Num; i++)    {
        peerBD_Handle[i].bdHandle = 0;
        peerBD_Handle[i].attId = 0;
    }

    initialDeviceServicesList();

    decodeRawBuffer.clear();
    nextRawNeededSize = EVT_HEADER_BYTE_SIZE;
}

SenDecodeCySmtProtocolPackageObject::~SenDecodeCySmtProtocolPackageObject()
{
}

void SenDecodeCySmtProtocolPackageObject::initialMetaDevaceServicesList()
{
    for (int i=0; i<Sen_Max_Services_Number; i++)   {
        metaDeviceServicesList[i].charactersCount = 0;
        metaDeviceServicesList[i].descriptorsTotalCount = 0;
        metaDeviceServicesList[i].range.startHandle = 0x0000;
        metaDeviceServicesList[i].range.endHandle = 0x0000;
        metaDeviceServicesList[i].uuid = 0x0000;
        metaDeviceServicesList[i].uuid128 = false;
        metaDeviceServicesList[i].uuid128_first32   = 0x00000000;
        metaDeviceServicesList[i].uuid128_32To64    = 0x00000000;
        metaDeviceServicesList[i].uuid128_64To96    = 0x00000000;
        metaDeviceServicesList[i].uuid128_96To128   = 0x00000000;

        for(int j=0; j<Sen_Max_Characteristics_Number; j++) {
            metaDeviceServicesList[i].charactersList[j].attHandle = 0x0000;
            metaDeviceServicesList[i].charactersList[j].characterProperties = 0x00;
            metaDeviceServicesList[i].charactersList[j].characterValueHandle = 0x0000;
            metaDeviceServicesList[i].charactersList[j].characterValueUuid = 0x0000;
            metaDeviceServicesList[i].charactersList[j].valueDescriptorsCount = 0;
            metaDeviceServicesList[i].charactersList[j].characterValueUuid_128 = false;
            metaDeviceServicesList[i].charactersList[j].charaValueUuid128_first32   = 0x00000000;
            metaDeviceServicesList[i].charactersList[j].charaValueUuid128_32To64    = 0x00000000;
            metaDeviceServicesList[i].charactersList[j].charaValueUuid128_64To96    = 0x00000000;
            metaDeviceServicesList[i].charactersList[j].charaValueUuid128_96To128   = 0x00000000;

            for (int k=0; k<Sen_Max_Descriptors_Number; k++)    {
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorHandle = 0x0000;
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorUuid = 0x0000;
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorUuid_128 = false;
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorUuid128_first32   = 0x00000000;
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorUuid128_32To64    = 0x00000000;
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorUuid128_64To96    = 0x00000000;
                metaDeviceServicesList[i].charactersList[j].valueDescriptorsList[k].descriptorUuid128_96To128   = 0x00000000;

            }
        }
    }
}

void SenDecodeCySmtProtocolPackageObject::initialDeviceServicesList()
{
    metaDeviceServicesList = firstDeviceServicesList;
    initialMetaDevaceServicesList();
    metaDeviceServicesList = secondDeviceServicesList;
    initialMetaDevaceServicesList();
    metaDeviceServicesList = thirdDeviceServicesList;
    initialMetaDevaceServicesList();
    metaDeviceServicesList = fourthDeviceServicesList;
    initialMetaDevaceServicesList();
//    metaDeviceServicesCount = &firstDeviceServicesCount;
}

void SenDecodeCySmtProtocolPackageObject::decodeDongle(QByteArray rawDonglePackage)
{
        /*
         * Set up macro DongleReadSize with dataReading Priority system, codes in this are divided into sections by priority,
         * every sentence belongs to only one section with one priority; newReadData with a higher priority could pass smaller sections to get deeper
         *  and get into his sections to determine how many bytes to read every time, and where to use the new read bytes!!!
         *
         * %%%%. Most important thing is to extract correct num data from rawDonglePackage
         *       based on nextRawNeededSize and nextRawDataPriority
         * %%
      %%% Details: Codes after every data extraction from rawDonglePackage, need to be
         *      a. find out length of leftDonglePackge
         *      b. determine new nextRawNeededSize and extractStartPoint
         *      c. process extacted decodeRawBuffer based on nextRawDataPriority, determine clear or not after the processing
         *      d. determine new nextRawNeededSize and extractStartPoint again, then determine nextRawDataPriority
         */
        switch(nextRawDataPriority) {
            case PRIORITY_HEADER:   {
                priorityHeaderHandler(rawDonglePackage);
            }break;

            case PRIORITY_LENGTH_INFO:   {
                priorityLengthInfoHandler(rawDonglePackage);
            }break;

            case PRIORITY_EVENT_WITH_PARAMETERS:   {
                priorityEvtWithParametersHandler(rawDonglePackage);
            }break;

            default:    {
                qDebug() << "Wrong Priority, Fix Me!!";
            }break;
        }
}

void SenDecodeCySmtProtocolPackageObject::priorityHeaderHandler(QByteArray rawHeaderPackage)
{
//    check_rawHeaderPackage.clear();
//    check_rawHeaderPackage = rawHeaderPackage;// delete me after finish
//    qDebug() << "I am in priorityHeaderHandler";

    int extractStartPoint = 0;
    if (rawHeaderPackage.length() >= nextRawNeededSize)
    {
        QByteArray wholeHeaderStartPackage = decodeRawBuffer + rawHeaderPackage;//Important!
        decodeRawBuffer += extract_Y_Bytes_Since_X_From(rawHeaderPackage, EVT_HEADER_BYTE_SIZE, extractStartPoint);

        if ((uchar)decodeRawBuffer[DATA_LOW_8BIT] == dongleEvtHeader_Low
                && (uchar)decodeRawBuffer[DATA_HIGH_8BIT] == dongleEvtHeader_High  )
        {
            decodeRawBuffer.clear();
            nextRawNeededSize = EVT_LENGTH_INFO_OCCUPATION;
            nextRawDataPriority = PRIORITY_LENGTH_INFO;

            QByteArray rawLengthInfoPackage = wholeHeaderStartPackage.remove(0, EVT_HEADER_BYTE_SIZE);
            priorityLengthInfoHandler(rawLengthInfoPackage);

        }// Header Priority Data Decoded
        else        // raw data wrong, show it to console, find out its cause and fix it!!
        {
            qDebug() << "Header Priority, raw data wrong, show it to console, find out its cause and fix it!!";
            showHexDataToConsole(rawHeaderPackage, rawHeaderPackage.length());// Send to console it not 0xA7BD Header
            decodeRawBuffer.clear();
            nextRawNeededSize = EVT_HEADER_BYTE_SIZE;
            nextRawDataPriority = PRIORITY_HEADER;
        }

    }else   {   // Header raw data not enough for EVT_HEADER_BYTE_SIZE
        decodeRawBuffer += extract_Y_Bytes_Since_X_From(rawHeaderPackage, rawHeaderPackage.length(), extractStartPoint);
        nextRawNeededSize = nextRawNeededSize - rawHeaderPackage.length();
        nextRawDataPriority = PRIORITY_HEADER;
    }
}

void SenDecodeCySmtProtocolPackageObject::priorityLengthInfoHandler(QByteArray rawLengthInfoPackage)
{
//    check_rawLengthInfoPackage.clear();
//    check_rawLengthInfoPackage = rawLengthInfoPackage; // delete me after finish
//    qDebug() << "I am in priorityLengthInfoHandler";

    int extractStartPoint = 0;
    evtParamterLenSize = 0;
    if (rawLengthInfoPackage.length() >= nextRawNeededSize)
    {
        QByteArray wholeLengthInfoStartPackage = decodeRawBuffer + rawLengthInfoPackage;//Important!
        decodeRawBuffer += extract_Y_Bytes_Since_X_From(rawLengthInfoPackage, EVT_LENGTH_INFO_OCCUPATION, extractStartPoint);

        evtParamterLenSize = (uchar)decodeRawBuffer[DATA_LOW_8BIT] + 16*16*(uchar)decodeRawBuffer[DATA_HIGH_8BIT];

        if (evtParamterLenSize >= sizeof(ushort))// > sizeof(evtType)
        {
            decodeRawBuffer.clear();
            nextRawNeededSize = evtParamterLenSize;
            nextRawDataPriority = PRIORITY_EVENT_WITH_PARAMETERS;

            QByteArray rawEvtWithParamPackage = wholeLengthInfoStartPackage.remove(0, EVT_LENGTH_INFO_OCCUPATION);
            priorityEvtWithParametersHandler(rawEvtWithParamPackage);

//            check_after_rawLengthInfoPackage = rawLengthInfoPackage;// delete me after finish

        }// Length Info Priority Data Decoded
        else        // Length Info raw data wrong, show it to console, find out its cause and fix it!!
        {
            qDebug() << "Length Info Priority, raw data wrong, show it to console, find out its cause and fix it!!";
            showHexDataToConsole(rawLengthInfoPackage, rawLengthInfoPackage.length());
            decodeRawBuffer.clear();
            nextRawNeededSize = EVT_HEADER_BYTE_SIZE;
            nextRawDataPriority = PRIORITY_HEADER;
        }

    }else   {   // raw data not enough for EVT_LENGTH_INFO_OCCUPATION
        decodeRawBuffer += extract_Y_Bytes_Since_X_From(rawLengthInfoPackage, rawLengthInfoPackage.length(), extractStartPoint);
        nextRawNeededSize = nextRawNeededSize - rawLengthInfoPackage.length();
        nextRawDataPriority = PRIORITY_LENGTH_INFO;
    }
}

void SenDecodeCySmtProtocolPackageObject::priorityEvtWithParametersHandler(QByteArray rawEvtWithParamPackage)
{

    int extractStartPoint = 0;
    if ( evtParamterLenSize <= 0)   // evtParamterLenSize has to be none zero if this priorityEvtWithParametersHandler function is called
        qDebug() << "evtParamterLenSize wrong, somewhere clear wrong or forgot to assign it !!";
//    check_rawEvtWithParamPackage.clear();
//    check_rawEvtWithParamPackage = rawEvtWithParamPackage;// delete me after finish

    if (rawEvtWithParamPackage.length() >= nextRawNeededSize)
    {
        QByteArray wholeEvtWithParamStartPackage = decodeRawBuffer + rawEvtWithParamPackage;//Important!
        decodeRawBuffer += extract_Y_Bytes_Since_X_From(rawEvtWithParamPackage, evtParamterLenSize, extractStartPoint);
//        qDebug() << "I am Num = 0, rawEvtWithParamPackage.length() = " << rawEvtWithParamPackage.length() << "\n\t nextRawNeededSize = " << nextRawNeededSize;

        /*** 0. Decode evt_High_Types/ evt_Low_Types before switch into Macro Event handler ******/
        uchar evtType_H = (uchar)decodeRawBuffer[DATA_HIGH_8BIT];
        uchar evtType_L = (uchar)decodeRawBuffer[DATA_LOW_8BIT];
        uchar evtType_L_GapOrGatt = evtType_L & Sen_EVT_TYPE_LOW_8BIT_MASK;
        ushort evtType = evtType_L + 16*16*evtType_H;

        uchar cmdOpCode_L = (uchar)decodeRawBuffer[DATA_LOW_8BIT + (int)sizeof(ushort)];
        uchar cmdOpCode_H = ((uchar)decodeRawBuffer[DATA_HIGH_8BIT + (int)sizeof(ushort)] & Sen_CMD_ID_HIGH_8BIT_MASK);
        ushort cmdOpCode = cmdOpCode_L + 16*16*cmdOpCode_H;

        switch(evtType_H)    {
            case (uchar)General_Events:   {
                generalEventHandler(evtType, decodeRawBuffer, evtParamterLenSize);
//                qDebug() << "I am Num = 0, rawEvtWithParamPackage.length() = " << rawEvtWithParamPackage.length() << "\n\t nextRawNeededSize = " << nextRawNeededSize;
//                check_after_rawEvtWithParamPackage.clear();
//                check_after_rawEvtWithParamPackage = decodeRawBuffer;

            }break;

            case (uchar)L2CAP_Events:     {
                uchar cmdOpCodeID = cmdOpCode & Sen_CMD_ID_LOW_8BIT_MASK;
                L2CAP_EventHandler(evtType, cmdOpCodeID, decodeRawBuffer, evtParamterLenSize);
            }break;

            case (uchar)GATT_GAP_Events:  {

                uchar cmdOpCodeID = cmdOpCode & Sen_CMD_ID_LOW_8BIT_MASK;

                if (evtType_L_GapOrGatt == Sen_EVT_GAP_OUTOF_GAPGATT)
                        gapEventHandler(evtType, cmdOpCodeID, decodeRawBuffer, evtParamterLenSize);
                else    gattEventHandler(evtType, cmdOpCodeID, decodeRawBuffer, evtParamterLenSize);

            }break;

            default:    {
                dumpCheck = decodeRawBuffer;
                qDebug() << "evtType_H got wrong raw data, show rawEvtWithParamPackage to console, find out its cause and fix it!!";
                showHexDataToConsole(rawEvtWithParamPackage, rawEvtWithParamPackage.length());
            }break;
        }

        decodeRawBuffer.clear();
        nextRawNeededSize = EVT_HEADER_BYTE_SIZE;
        nextRawDataPriority = PRIORITY_HEADER;

        QByteArray rawHeaderPackage = wholeEvtWithParamStartPackage.remove(0, evtParamterLenSize);

//        check_wholeEvtWithParamStartPackage.clear();
//        check_wholeEvtWithParamStartPackage = wholeEvtWithParamStartPackage;// delete me
//        qDebug() << "evtParamterLenSize reset 0.";

        evtParamterLenSize = 0;// This step should be before next transfer, this is very important!!!

        priorityHeaderHandler(rawHeaderPackage);

    }else   {   // raw data not enough for evtParamterLenSize
        decodeRawBuffer += extract_Y_Bytes_Since_X_From(rawEvtWithParamPackage, rawEvtWithParamPackage.length(), extractStartPoint);
        nextRawNeededSize = nextRawNeededSize - rawEvtWithParamPackage.length();
        nextRawDataPriority = PRIORITY_EVENT_WITH_PARAMETERS;
    }
}

void SenDecodeCySmtProtocolPackageObject::gattEventHandler(ushort evtType, uchar cmdOpCodeID, QByteArray evtPackageArray, int packageLength)
{
    if (evtPackageArray.length() < packageLength)
        qDebug() << "evtPackageArray.length() = " << evtPackageArray.length() << ", packageLength = " << packageLength;
    evtPackageArray = extract_Y_Bytes_Since_X_From(evtPackageArray, packageLength, 0);

    int parameterSize = evtPackageArray.length() - (sizeof(evtType) + sizeof(ushort)); // whole cmdOpCode
    if (parameterSize < 0)  {
        qDebug() << "Something has no parameters    or   parameterSize Algorithm Wrong !";
    }
    if (evtType == (ushort)EVT_CURRENT_CONNECTION_PARAMETERS_NOTIFICATION
            || evtType == (ushort)EVT_CHARACTERISTIC_VALUE_NOTIFICATION)  {//cmd was put 0
            evtPackageArray = evtPackageArray.remove(0, sizeof(evtType));
    }else   evtPackageArray = evtPackageArray.remove(0, (sizeof(evtType) + sizeof(ushort))); // whole cmdOpCode


    switch(evtType)    {
        case (ushort)EVT_EXCHANGE_GATT_MTU_SIZE_RESPONSE:   {
            QByteArray evtFeedBackToConsole;
            int index = 0;
            uchar deviceID;
            deviceID = evtPackageArray[index++];
            evtFeedBackToConsole += "    'Exchange Gatt MTU Size' Response  ( Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGattCmdStringFromArray((int)cmdOpCodeID);
            evtFeedBackToConsole += " )\n\t\t\t         Device ID:\t";
            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(&deviceID, 1);
            index++;// skip attID, useless
            evtFeedBackToConsole += "\t MTU:   0x";
            peerBD_Handle[Sen_BluetoothDevice_Maximum_Num - deviceID].MTU_Low = (uchar)evtPackageArray[index++];
            peerBD_Handle[Sen_BluetoothDevice_Maximum_Num - deviceID].MTU_High = (uchar)evtPackageArray[index];
            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(peerBD_Handle[Sen_BluetoothDevice_Maximum_Num - deviceID].MTU_High), 1);
            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(peerBD_Handle[Sen_BluetoothDevice_Maximum_Num - deviceID].MTU_Low), 1);
            evtFeedBackToConsole += "\n";

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }break;
        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_DISCOVER_ALL_PRIMARY_SERVICES_RESULT_PROGRESS:   {
            QByteArray evtFeedBackToConsole;
            int index = 0;
            uchar deviceID;
            deviceID = evtPackageArray[index++];
            switch(deviceID)    {
                case (uchar)Sen_First_Bluetooth_Device_ID:  {
                metaDeviceServicesList = firstDeviceServicesList;
                metaDeviceServicesCount = &firstDeviceServicesCount;
            }break;
                case (uchar)Sen_Second_Bluetooth_Device_ID: {   metaDeviceServicesList = secondDeviceServicesList;  metaDeviceServicesCount = &secondDeviceServicesCount;    }break;
                case (uchar)Sen_Third_Bluetooth_Device_ID:  {   metaDeviceServicesList = thirdDeviceServicesList;   metaDeviceServicesCount = &thirdDeviceServicesCount;    }break;
                case (uchar)Sen_Fourth_Bluetooth_Device_ID: {   metaDeviceServicesList = fourthDeviceServicesList;  metaDeviceServicesCount = &fourthDeviceServicesCount;    }break;
            }
//            *metaDeviceServicesCount = 0;

            evtFeedBackToConsole += "==========================================\n";
            evtFeedBackToConsole += "    'Discover All Primary Services' Result Progress (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
            evtFeedBackToConsole += " )\tDevice ID:\t";
            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(&deviceID, 1);
            evtFeedBackToConsole += "\n------------------------\n";

            index++;// skip attID, useless

            for (; index<evtPackageArray.length();) {
                (*metaDeviceServicesCount)++;

                evtFeedBackToConsole += "\tPrimary Service ";
                evtFeedBackToConsole += QString::number(*metaDeviceServicesCount, 10);
                evtFeedBackToConsole += ":\n\t\t Start Handle:\t0x";
                metaDeviceServicesList[(*metaDeviceServicesCount) - 1].range.startHandle = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index+1]);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].range.startHandle) + 1, 1);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].range.startHandle), 1);
                index += sizeof(short);//startHandle
                evtFeedBackToConsole += ":\n\t\t End Handle:\t0x";
                metaDeviceServicesList[(*metaDeviceServicesCount) - 1].range.endHandle = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index+1]);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].range.endHandle) + 1, 1);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].range.endHandle), 1);
                index += sizeof(short);//endHandle

                if ((uchar)evtPackageArray[index] == CYBLE_GATT_16_BIT_UUID_FORMAT)     {//uuidType
                    index++;
                    evtFeedBackToConsole += ":\n\t\t UUID:\t0x";
                    metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index+1]);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid), 1);
                    index += sizeof(short);//UUID

                }else if ((uchar)evtPackageArray[index] == CYBLE_GATT_128_BIT_UUID_FORMAT)  {
                    index++;
                    metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128 = true;

                    metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_first32 =
                            (uchar)evtPackageArray[index] + (16*16)*((uchar)evtPackageArray[index+1])
                        + (16*16)*(16*16)*(uchar)evtPackageArray[index+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[index+3]);
                    index += sizeof(uint);//uuid128_first32

                    metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_32To64 =
                            (uchar)evtPackageArray[index] + (16*16)*((uchar)evtPackageArray[index+1])
                        + (16*16)*(16*16)*(uchar)evtPackageArray[index+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[index+3]);
                    index += sizeof(uint);//uuid128_32To64

                    metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_64To96 =
                            (uchar)evtPackageArray[index] + (16*16)*((uchar)evtPackageArray[index+1])
                        + (16*16)*(16*16)*(uchar)evtPackageArray[index+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[index+3]);
                    index += sizeof(uint);//uuid128_64To96

                    metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_96To128 =
                            (uchar)evtPackageArray[index] + (16*16)*((uchar)evtPackageArray[index+1])
                        + (16*16)*(16*16)*(uchar)evtPackageArray[index+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[index+3]);
                    index += sizeof(uint);//uuid128_96To128

                    evtFeedBackToConsole += ":\n\t\t UUID_128: \t0x ";
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_96To128) + 3, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_96To128) + 2, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_96To128) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_96To128) + 0, 1);

                    evtFeedBackToConsole += ",";
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_64To96) + 3, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_64To96) + 2, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_64To96) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_64To96) + 0, 1);

                    evtFeedBackToConsole += ",";
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_32To64) + 3, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_32To64) + 2, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_32To64) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_32To64) + 0, 1);

                    evtFeedBackToConsole += ",";
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_first32) + 3, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_first32) + 2, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_first32) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[(*metaDeviceServicesCount) - 1].uuid128_first32) + 0, 1);

                    qDebug() << "Service 128 Bit UUID";
                }else   {   qDebug() << " UUID Type Wrong! further decoding needed!";   }

                evtFeedBackToConsole += "\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            }

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

            bandPassModeInputHint.clear();
            if((*metaDeviceServicesCount)>0)
            {
                emit emitBandPassModeBegin(DATA_HIGH_8BIT, *metaDeviceServicesCount, BandPassJudge_AfterGotAllServices, mySensorID);

                bandPassModeInputHint += "Please Input:";
                for (int i=0; i < (*metaDeviceServicesCount); i++)  {
                    bandPassModeInputHint += "\n ";
                    bandPassModeInputHint += QString::number(i+1).toLatin1();
                    bandPassModeInputHint += " ,   to Discover All Characteristics in Service   ";
                    bandPassModeInputHint += QString::number(i+1).toLatin1();
                }
                bandPassModeInputHint += "\n";
            }

        }break;

        /*******************************************************************************************************************************************************************/

        case (ushort)EVT_DISCOVER_ALL_CHARACTERISTICS_RESULT_PROGRESS:   {
            QByteArray evtFeedBackToConsole;
            int packageArrayIndex = 0;
            int serviceIndex = -1;
            uchar deviceID;
            ushort attHandle;//(characteristic declaration ID)only one attribute in one service
            deviceID = evtPackageArray[packageArrayIndex++];
            switch(deviceID)    {
                case (uchar)Sen_First_Bluetooth_Device_ID:  {
                metaDeviceServicesList = firstDeviceServicesList;
            }break;
                case (uchar)Sen_Second_Bluetooth_Device_ID: {   metaDeviceServicesList = secondDeviceServicesList;    }break;
                case (uchar)Sen_Third_Bluetooth_Device_ID:  {   metaDeviceServicesList = thirdDeviceServicesList;     }break;
                case (uchar)Sen_Fourth_Bluetooth_Device_ID: {   metaDeviceServicesList = fourthDeviceServicesList;    }break;
            }

            evtFeedBackToConsole += "==========================================\n";
            evtFeedBackToConsole += "    'Discover All Characteristics' Result Progress (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
            evtFeedBackToConsole += " )\tDevice ID:\t";
            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(&deviceID, 1);
            evtFeedBackToConsole += "\n------------------------\n";
            packageArrayIndex++;// skip attID, useless

            for (int characterCount=0; packageArrayIndex < evtPackageArray.length();) {
                characterCount++;
                attHandle =  (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);


                for(int servicesListIndex = 0; servicesListIndex < Sen_Max_Services_Number; servicesListIndex++)    {
                    if (attHandle >= metaDeviceServicesList[servicesListIndex].range.startHandle && attHandle <= metaDeviceServicesList[servicesListIndex].range.endHandle)
                        serviceIndex = servicesListIndex;
                }

                chosenServiceIndex= serviceIndex;
                if  (serviceIndex != -1)    {
                    evtFeedBackToConsole += " Service ";
                    evtFeedBackToConsole += QString::number(serviceIndex + 1, 10);
                    evtFeedBackToConsole += "\tCharacteristic  ";
                    metaDeviceServicesList[serviceIndex].charactersCount = characterCount;
                    evtFeedBackToConsole += QString::number(metaDeviceServicesList[serviceIndex].charactersCount, 10);
                    evtFeedBackToConsole += ":\n\t\t Attribute Handle:\t0x";

                    metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].attHandle = (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].attHandle) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].attHandle), 1);
                    packageArrayIndex += sizeof(short);// Attribute Handle

                    metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterProperties = evtPackageArray[packageArrayIndex++];

                    evtFeedBackToConsole += "\n\t   Characteristic Value Handle:\t0x";
                    metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueHandle = (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueHandle) + 1, 1);
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueHandle), 1);
                    packageArrayIndex += sizeof(short);// Characteristic Value Handle

                    if ((uchar)evtPackageArray[packageArrayIndex] == CYBLE_GATT_16_BIT_UUID_FORMAT)     {//uuidType
                        packageArrayIndex++;
                        evtFeedBackToConsole += "\n  \t    Characteristic Value UUID:\t0x";
                        metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueUuid = (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueUuid) + 1, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueUuid), 1);
                        packageArrayIndex += sizeof(short);//Characteristic Value UUID

                    }else if ((uchar)evtPackageArray[packageArrayIndex] == CYBLE_GATT_128_BIT_UUID_FORMAT)  {
                        packageArrayIndex++;
                        metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterValueUuid_128 = true;

                        metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_first32 =
                                (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                              + (16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+2]) + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                        packageArrayIndex += sizeof(uint);//charaValueUuid128_first32

                        metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_32To64 =
                                (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                              + (16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+2]) + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                        packageArrayIndex += sizeof(uint);//charaValueUuid128_32To64

                        metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_64To96 =
                                (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                              + (16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+2]) + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                        packageArrayIndex += sizeof(uint);//charaValueUuid128_64To96

                        metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_96To128 =
                                (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                              + (16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+2]) + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                        packageArrayIndex += sizeof(uint);//charaValueUuid128_96To128



                        evtFeedBackToConsole += "\n  \t Characteristic Value UUID_128:\t0x ";
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_96To128) + 3, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_96To128) + 2, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_96To128) + 1, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_96To128) + 0, 1);

                        evtFeedBackToConsole += ",";
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_64To96) + 3, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_64To96) + 2, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_64To96) + 1, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_64To96) + 0, 1);

                        evtFeedBackToConsole += ",";
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_32To64) + 3, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_32To64) + 2, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_32To64) + 1, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_32To64) + 0, 1);

                        evtFeedBackToConsole += ",";
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_first32) + 3, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_first32) + 2, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_first32) + 1, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].charaValueUuid128_first32) + 0, 1);

                        qDebug() << "Chracteristic 128 Bit UUID";
                    }else   {   qDebug() << " UUID Type Wrong! further decoding needed!";   }

                    evtFeedBackToConsole += "\n\t     Characteristic Properties:\t0x";
                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterCount-1].characterProperties), 1);

                    evtFeedBackToConsole += "\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                }else if (serviceIndex = -1)    {   qDebug() << "serviceIndex = -1, attHandle Wrong! ";
                }else   { qDebug() << "serviceIndex Algorithm Wrong, Check it out! "; }
            }


            /*********** Echo Test Start ***********/
//            evtFeedBackToConsole += "\n  Now Echo Test from Memory !!! \n\n";
//            for (int j=0; j < metaDeviceServicesList[serviceIndex].charactersCount; j++) {

//                    evtFeedBackToConsole += " Service ";
//                    evtFeedBackToConsole += QString::number(serviceIndex + 1, 10);
//                    evtFeedBackToConsole += "\tCharacteristic  ";
//                    evtFeedBackToConsole += QString::number(j+1, 10);
//                    evtFeedBackToConsole += "\n\t\t Attribute Handle:\t0x";

//                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].attHandle) + 1, 1);
//                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].attHandle), 1);

//                    evtFeedBackToConsole += "\n\t   Characteristic Value Handle:\t0x";
//                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].characterValueHandle) + 1, 1);
//                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].characterValueHandle), 1);

//                        evtFeedBackToConsole += "\n  \t    Characteristic Value UUID:\t0x";
//                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].characterValueUuid) + 1, 1);
//                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].characterValueUuid), 1);


//                    evtFeedBackToConsole += "\n\t     Characteristic Properties:\t0x";
//                    evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[j].characterProperties), 1);

//                    evtFeedBackToConsole += "\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

//            }

            /********** Echo Test End  ********************/





            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

        }break;

        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_DISCOVER_ALL_CHARACTERISTIC_DESCRIPTORS_RESULT_PROGRESS:   {
//            showHexDataToConsole(evtPackageArray, evtPackageArray.length());
            QByteArray evtFeedBackToConsole;
            int packageArrayIndex = 0;
            int serviceIndex = -1, characterIndex = -1, descriptorInOneCharatIndex = -1;
            uchar deviceID;
            ushort descriptorHandle;//(Could be any handle: Service Handle, Character Handle, or Descriptor Handle)only one attribute in one service
            deviceID = evtPackageArray[packageArrayIndex++];
            switch(deviceID)    {
                case (uchar)Sen_First_Bluetooth_Device_ID:  {
                metaDeviceServicesList = firstDeviceServicesList;
            }break;
                case (uchar)Sen_Second_Bluetooth_Device_ID: {   metaDeviceServicesList = secondDeviceServicesList;    }break;
                case (uchar)Sen_Third_Bluetooth_Device_ID:  {   metaDeviceServicesList = thirdDeviceServicesList;     }break;
                case (uchar)Sen_Fourth_Bluetooth_Device_ID: {   metaDeviceServicesList = fourthDeviceServicesList;    }break;
            }

            evtFeedBackToConsole += "==========================================\n";
            evtFeedBackToConsole += "    'Discover All Characteristics Descriptor' Result Progress (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
            evtFeedBackToConsole += " )\tDevice ID:\t";
            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(&deviceID, 1);

            packageArrayIndex++;// skip attID, useless

            descriptorHandle =  (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);

            // Find out serviceIndex
            for(int servicesListIndex = 0; servicesListIndex < Sen_Max_Services_Number; servicesListIndex++)    {
                if (descriptorHandle >= metaDeviceServicesList[servicesListIndex].range.startHandle && descriptorHandle <= metaDeviceServicesList[servicesListIndex].range.endHandle)
                {
                    serviceIndex = servicesListIndex;

                    break;
                }
            }

            evtFeedBackToConsole += "\n Service ";
            evtFeedBackToConsole += QString::number(serviceIndex + 1, 10);
            evtFeedBackToConsole += "\n------------------------\n";

            if  (serviceIndex != -1)    {
                // classify descriptors into characteristics list
                for (int descriptorTotalCount=0; packageArrayIndex < evtPackageArray.length();) {
                    descriptorTotalCount++;
                    characterIndex = -1;
                    // Find out characterIndex
                    // And set descriptorsTotalCount +1, charactersList[characterIndex].valueDescriptorsCount +1
                    // And set descriptorInOneCharatIndex
                    descriptorHandle =  (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);
                    if (descriptorHandle == metaDeviceServicesList[serviceIndex].range.startHandle)    {   // Service Handle
                        characterIndex = 0; // Put Service Handle Descriptor in the first Character List member
                        (metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount)++;
                        descriptorInOneCharatIndex = metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount - 1;
                        (metaDeviceServicesList[serviceIndex].descriptorsTotalCount)++;
                    }else   {
                        int charactListInex=0;
                        for (; charactListInex < metaDeviceServicesList[serviceIndex].charactersCount; charactListInex++)  {
                            if (    (descriptorHandle >= metaDeviceServicesList[serviceIndex].charactersList[charactListInex].attHandle
                                    &&  descriptorHandle < metaDeviceServicesList[serviceIndex].charactersList[charactListInex + 1].attHandle)  )
                            {
                                characterIndex = charactListInex;

                                (metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount)++;
                                descriptorInOneCharatIndex = metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount - 1;

                                (metaDeviceServicesList[serviceIndex].descriptorsTotalCount)++;

                                break;
                            }
                        }
                        if (characterIndex == -1)   {
                            charactListInex = metaDeviceServicesList[serviceIndex].charactersCount - 1;
                            if(descriptorHandle >= metaDeviceServicesList[serviceIndex].charactersList[charactListInex].attHandle
                                    &&  descriptorHandle <= metaDeviceServicesList[serviceIndex].range.endHandle)
                            {
                                characterIndex = charactListInex;
                                (metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount)++;
                                descriptorInOneCharatIndex = metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount - 1;

                                (metaDeviceServicesList[serviceIndex].descriptorsTotalCount)++;
                            }
                        }
                    }

                    if (characterIndex != -1)   {
                        chosenServiceIndex= serviceIndex;

                        evtFeedBackToConsole += " Characteristic ";
                        evtFeedBackToConsole += QString::number(characterIndex + 1, 10);
                        evtFeedBackToConsole += "  Descriptor ";
                        evtFeedBackToConsole += QString::number((metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsCount), 10);
                        evtFeedBackToConsole += ":\tAttribute Handle:  0x";

                        metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorHandle = (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorHandle) + 1, 1);
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorHandle), 1);
                        packageArrayIndex += sizeof(short);// Attribute Handle

                        if ((uchar)evtPackageArray[packageArrayIndex] == CYBLE_GATT_16_BIT_UUID_FORMAT)     {//uuidType
                            packageArrayIndex++;
                            evtFeedBackToConsole += "\tUUID:  0x";
                            metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid = (uchar)evtPackageArray[packageArrayIndex] + 16*16*((uchar)evtPackageArray[packageArrayIndex+1]);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid) + 1, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid), 1);
                            packageArrayIndex += sizeof(short);//Characteristic Value UUID

                        }else if ((uchar)evtPackageArray[packageArrayIndex] == CYBLE_GATT_128_BIT_UUID_FORMAT)  {
                            packageArrayIndex++;

                            metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid_128 = true;

                            metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_first32 =
                                                    (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                                            + (16*16)*(16*16)*(uchar)evtPackageArray[packageArrayIndex+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                            packageArrayIndex += sizeof(uint);//descriptorUuid128_first32

                            metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_32To64 =
                                                    (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                                            + (16*16)*(16*16)*(uchar)evtPackageArray[packageArrayIndex+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                            packageArrayIndex += sizeof(uint);//descriptorUuid128_32To64

                            metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_64To96 =
                                                    (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                                            + (16*16)*(16*16)*(uchar)evtPackageArray[packageArrayIndex+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                            packageArrayIndex += sizeof(uint);//descriptorUuid128_64To96

                            metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_96To128 =
                                                    (uchar)evtPackageArray[packageArrayIndex] + (16*16)*((uchar)evtPackageArray[packageArrayIndex+1])
                                            + (16*16)*(16*16)*(uchar)evtPackageArray[packageArrayIndex+2] + (16*16)*(16*16)*(16*16)*((uchar)evtPackageArray[packageArrayIndex+3]);
                            packageArrayIndex += sizeof(uint);//descriptorUuid128_96To128


                            evtFeedBackToConsole += "\tUUID_128:\t0x ";
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_96To128) + 3, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_96To128) + 2, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_96To128) + 1, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_96To128) + 0, 1);

                            evtFeedBackToConsole += ",";
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_64To96) + 3, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_64To96) + 2, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_64To96) + 1, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_64To96) + 0, 1);

                            evtFeedBackToConsole += ",";
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_32To64) + 3, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_32To64) + 2, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_32To64) + 1, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_32To64) + 0, 1);

                            evtFeedBackToConsole += ",";
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_first32) + 3, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_first32) + 2, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_first32) + 1, 1);
                            evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(metaDeviceServicesList[serviceIndex].charactersList[characterIndex].valueDescriptorsList[descriptorInOneCharatIndex].descriptorUuid128_first32) + 0, 1);

                            qDebug() << "128 Bit UUID";
                        }else   {   qDebug() << " UUID Type Wrong! further decoding needed!";   }

                        evtFeedBackToConsole += "\n";//\t\t\t~~~~~~~~~~~~~~~~~~~~~~\n";

                    }else if (characterIndex == -1)    {   qDebug() << "characterIndex = -1, descriptorHandle Wrong! ";
                    }else   { qDebug() << "characterIndex Algorithm Wrong, Check it out! "; }
                }
            }else if (serviceIndex == -1)    {   qDebug() << "serviceIndex = -1, descriptorHandle Wrong! ";
            }else   { qDebug() << "serviceIndex Algorithm Wrong, Check it out! "; }



            /*********** Start ***********/

            /********** End  ********************/

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }break;
        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_CHARACTERISTIC_VALUE_NOTIFICATION:   {
            if (evtPackageArray.length() == 8)  {   // Motion Burst
                // For Slider Read after Actived
                QByteArray evtFeedBackToConsole, jumpMotionUpdateArray;
                int index = 0;
                uchar deviceID, valueHandle_Low, valueHandle_High, value_Low, value_High;
                deviceID = (uchar)evtPackageArray[index++];
                evtFeedBackToConsole += "    Motion Burst: ";
                evtFeedBackToConsole += "\tDevice ID\t";
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(&deviceID, 1);
                index++;// skip attID, useless
                evtFeedBackToConsole += "\tValue Handle   0x";
                valueHandle_Low = evtPackageArray[index++];
                valueHandle_High = evtPackageArray[index++];

                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(valueHandle_High), 1);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(valueHandle_Low), 1);

                index += sizeof(ushort);// skip value length, useless

                evtFeedBackToConsole += "\t Value   0x  ";

                value_Low = evtPackageArray[index++];
                value_High = evtPackageArray[index++];
                jumpMotionUpdateArray[0] = value_Low;
                jumpMotionUpdateArray[1] = value_High;

                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(value_High), 1);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(value_Low), 1);

                evtFeedBackToConsole += "\n";

                emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

                emit emitJumpMotionUpdateSignal(jumpMotionUpdateArray, jumpMotionUpdateArray.length(), mySensorID);

            }else if (evtPackageArray.length() == 307)  {    // Image Burst
                // For Slider Read after Actived
                QByteArray evtFeedBackToConsole, pixels300Array;
                int index = 0;
                uchar deviceID, valueHandle_Low, valueHandle_High, pixels300SectionIndex;
                deviceID = (uchar)evtPackageArray[index++];
                evtFeedBackToConsole += "    Image Pixels: ";
                evtFeedBackToConsole += "\tDevice ID\t";
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(&deviceID, 1);
                index++;// skip attID, useless
                evtFeedBackToConsole += "\tValue Handle   0x";
                valueHandle_Low = evtPackageArray[index++];
                valueHandle_High = evtPackageArray[index++];

                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(valueHandle_High), 1);
                evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(valueHandle_Low), 1);

                index += sizeof(ushort);// skip value length, useless

                pixels300SectionIndex = evtPackageArray[index++];
                for(int m=0; m<300; m++) {
                    pixels300Array[m] = evtPackageArray[index + m];
                }

                evtFeedBackToConsole += "\t Value:   ";
//                evtFeedBackToConsole += getAddFeedBackStringFromAddStartPointer(pixels18Array, 19);

                evtFeedBackToConsole += "\n";

                emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

                emit emit300PixelsShowUp(pixels300Array, pixels300SectionIndex);

            }else   {
                showHexDataToConsole(evtPackageArray, evtPackageArray.length());
                qDebug() << "something Wrong, value is not correctly read !";

            }

        }break;

        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_DISCOVER_PRIMARY_SERVICES_BY_UUID_RESULT_PROGRESS:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_DISCOVER_PRIMARY_SERVICES_BY_UUID_RESULT_PROGRESS !";   }break;
        case (ushort)EVT_FIND_INDLUDED_SERVICES_RESULT_PROGRESS:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_FIND_INDLUDED_SERVICES_RESULT_PROGRESS !";   }break;
        case (ushort)EVT_DISCOVER_CHARACTERISTICS_BY_UUID_RESULT_PROGRESS:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_DISCOVER_CHARACTERISTICS_BY_UUID_RESULT_PROGRESS !";   }break;

        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_READ_CHARACTERISTIC_VALUE_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_READ_CHARACTERISTIC_VALUE_RESPONSE !";   }break;
        case (ushort)EVT_READ_USING_CHARACTERISTIC_UUID_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_READ_USING_CHARACTERISTIC_UUID_RESPONSE !";   }break;
        case (ushort)EVT_READ_LONG_CHARACTERISTIC_VALUE_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_READ_LONG_CHARACTERISTIC_VALUE_RESPONSE !";   }break;
        case (ushort)EVT_READ_MULTIPLE_CHARACTERISTIC_VALUES_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_READ_MULTIPLE_CHARACTERISTIC_VALUES_RESPONSE !";   }break;

        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_READ_CHARACTERISTIC_DESCRIPTOR_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_READ_CHARACTERISTIC_DESCRIPTOR_RESPONSE !";   }break;
        case (ushort)EVT_READ_LONG_CHARACTERISTIC_DESCRIPTOR_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_READ_LONG_CHARACTERISTIC_DESCRIPTOR_RESPONSE !";   }break;
        case (ushort)EVT_CHARACTERISTIC_VALUE_INDICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_CHARACTERISTIC_VALUE_INDICATION !";   }break;
        case (ushort)EVT_GATT_ERROR_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GATT_ERROR_NOTIFICATION !";   }break;

        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_GATT_STOP_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GATT_STOP_NOTIFICATION !";   }break;
        case (ushort)EVT_GATT_TIMEOUT_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GATT_TIMEOUT_NOTIFICATION !";   }break;
        /*******************************************************************************************************************************************************************/

        default:    {
            qDebug() << "GATT Events Handler ! Something Wrong with me !";
            showHexDataToConsole(evtPackageArray, evtPackageArray.length());
        }break;
    }


}

void SenDecodeCySmtProtocolPackageObject::gapEventHandler(ushort evtType, uchar cmdOpCodeID, QByteArray evtPackageArray, int packageLength)
{
    if (evtPackageArray.length() < packageLength)
        qDebug() << "evtPackageArray.length() = " << evtPackageArray.length() << ", packageLength = " << packageLength;
    evtPackageArray = extract_Y_Bytes_Since_X_From(evtPackageArray, packageLength, 0);

    int parameterSize = evtPackageArray.length() - (sizeof(evtType) + sizeof(ushort)); // whole cmdOpCode
    if (parameterSize < 0)  {
        qDebug() << "EVT_SCAN_STOPPED_NOTIFICATION             or          parameterSize Algorithm Wrong !";
    }
    if (evtType == (ushort)EVT_CURRENT_CONNECTION_PARAMETERS_NOTIFICATION
            || evtType == (ushort)EVT_CONNECTION_TERMINATED_NOTIFICATION
            || evtType == (ushort)EVT_UPDATE_CONNECTION_PARAMETERS_NOTIFICATION)  {//cmd was put 0
            evtPackageArray = evtPackageArray.remove(0, sizeof(evtType));
    }else   evtPackageArray = evtPackageArray.remove(0, (sizeof(evtType) + sizeof(ushort))); // whole cmdOpCode

    switch(evtType)    {
        case (ushort)EVT_SCAN_STOPPED_NOTIFICATION:   {
            QByteArray evtFeedBackToConsole;
            if (packageLength == sizeof(evtType))   evtFeedBackToConsole += "\tEvent: Scan Stopped !\n";
            else    {
                evtFeedBackToConsole += "  Wrong Event Data happened, check it out!\n";
                qDebug() << "  Wrong Event Data happened, check it out!";
            }
            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }break;

        case (ushort)EVT_GET_WHITE_LIST_RESPONSE:   {
            QByteArray evtFeedBackToConsole;
            evtFeedBackToConsole += "    'Get White List' Response  (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
            if (parameterSize == 0)    evtFeedBackToConsole += ") :    No device in White List!\n";
            else  { evtFeedBackToConsole += "):    Got White List, Extro decoding Needed !\n";  showHexDataToConsole(evtPackageArray, evtPackageArray.length());  }

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }break;

        case (ushort)EVT_GET_BONDED_DEVICES_LIST_RESPONSE:  {
            QByteArray evtFeedBackToConsole;
            evtFeedBackToConsole += "    'Get Bonded Devices List' Response  (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
            if (parameterSize == 0)    evtFeedBackToConsole += ") :    No Device in Bonded List!\n";
            else  { evtFeedBackToConsole += "):    Got Bonded Devices List, Extro decoding Needed !\n";  showHexDataToConsole(evtPackageArray, evtPackageArray.length());  }

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }
    /***************************************************************************************************************************************/
        case (ushort)EVT_SCAN_PROGRESS_RESULT:   {
            if (packageLength >= GAPC_ADV_REPORT_T_STRUCKTURE_SIZE)  {
                if ((int)cmdOpCodeID == (uchar)Cmd_Start_Scan_Api)  {
                    QByteArray peerBDAddressArray;

                    int packageIndex = 0;
                    CYBLE_GAPC_ADV_REPORT_T advReport;
                    advReport.eventType = evtPackageArray[packageIndex++];
                    for (;packageIndex <= CYBLE_GAP_BD_ADDR_SIZE; packageIndex++ )  {
                        advReport.peerBdAddr[packageIndex - 1] = evtPackageArray[packageIndex];
                        peerBDAddressArray[packageIndex - 1] = evtPackageArray[packageIndex];
                    }
                    advReport.peerAddrType = evtPackageArray[packageIndex++];
                    advReport.rssi = evtPackageArray[packageIndex++];
                    advReport.dataLen = evtPackageArray[packageIndex++];
                    if (advReport.dataLen == (evtPackageArray.length() - packageIndex))    {
                        for(int i= 0; i<advReport.dataLen; i++)     {
                            advReport.data[i] = evtPackageArray[packageIndex + i];
                        }

                        QByteArray evtFeedBackToConsole;
                        evtFeedBackToConsole += "==========================================\n";
                        evtFeedBackToConsole += "    'Scan Process Evt' Result of Cmd ( ";
                        evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
                        evtFeedBackToConsole += " )\n------------------------\n";

                        evtFeedBackToConsole += " Adv Evt Type: ";
                        if      (advReport.eventType == 0x00)   {  evtFeedBackToConsole += "Connectable undirected advertising (0x00)\n\t   ";  }
                        else if (advReport.eventType == 0x01)   {  evtFeedBackToConsole += "Connectable directed advertising (0x01)\n\t   ";  }
                        else if (advReport.eventType == 0x02)   {  evtFeedBackToConsole += "Scannable undirected advertising (0x02)\n\t   ";  }
                        else if (advReport.eventType == 0x03)   {  evtFeedBackToConsole += "Non connectable undirected advertising (0x03)\n\t   ";  }
                        else if (advReport.eventType == 0x04)   {  evtFeedBackToConsole += "Scan Response (0x04)\n\t   ";  }
                        else                                    {   evtFeedBackToConsole += "advReport.eventType Got Wrong Data! \n\t   ";  }

                        evtFeedBackToConsole += "BD Address Type: ";
                        if (advReport.peerAddrType == 0x00)         evtFeedBackToConsole += "Public Addrss\n\t   ";
                        else if (advReport.peerAddrType == 0x01)    evtFeedBackToConsole += "Public Addrss\n\t   ";
                        else    evtFeedBackToConsole += "advReport.peerAddrType decoding algorithm Wrong! \n\t   ";

                        evtFeedBackToConsole += "BD Address:    ";
                        evtFeedBackToConsole += getAddFeedBackStringFromAddStartPointer(advReport.peerBdAddr, CYBLE_GAP_BD_ADDR_SIZE);
                        evtFeedBackToConsole += "\n\t   Advertisement Event Data: ";
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer(advReport.data, advReport.dataLen);

                        evtFeedBackToConsole += "\n\t   RSSI: ";
                        evtFeedBackToConsole += advReport.rssi;
                        evtFeedBackToConsole += " (";
                        evtFeedBackToConsole += getHexFeedBackStringFromDataStartPointer((uchar*)&(advReport.rssi), 1);
                        evtFeedBackToConsole += ")\n";

                        emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

                        if (advReport.eventType == 0x04 && peerBDAddressArray == dongleProtocol->SenPSoC_BleAddress)
                        {
                            resolveSetPeerAddAutoAfterScanStop = true;
                            emit emitGapCmdStopScan();
                        }
                    }else {
                        qDebug() << "EVT_SCAN_PROGRESS_RESULT event GOT Wrong dataLen and Data, reconsider about this !";
                        showHexDataToConsole(evtPackageArray, evtPackageArray.length());
                    }
                }else   {
                    qDebug() << "EVT_SCAN_PROGRESS_RESULT event right but cmd wrong, reconsider about this !";
                    showHexDataToConsole(evtPackageArray, evtPackageArray.length());
                }
            }else   {
                qDebug() << "EVT_SCAN_PROGRESS_RESULT event GOT Wrong Package Length, reconsider about this !";
                showHexDataToConsole(evtPackageArray, evtPackageArray.length());
            }
        }break;

        /****************************************************************************************************************************************/
        case (ushort)EVT_RESOLVE_AND_SET_PEER_BD_ADDRESS_RESPONSE:   {
            QByteArray evtFeedBackToConsole;
            evtFeedBackToConsole += "    'Resolve & Set Peer Add ' Response  (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
            if ((uchar)evtPackageArray[0] == 0x01)    {
                evtFeedBackToConsole += ") :\tGood Status:  0x01\n";
                resolve_SetPeerAddGetGoodResponse = true;
            }else if ((uchar)evtPackageArray[0] == 0x00)  { resolve_SetPeerAddGetGoodResponse = false; evtFeedBackToConsole += "):    Bonded Devices List Error, Extro decoding Needed !\n";  showHexDataToConsole(evtPackageArray, evtPackageArray.length());  }
            else       { resolve_SetPeerAddGetGoodResponse = false; evtFeedBackToConsole += "):    Cmd R&S Peer Add Error, Extro decoding Needed !\n";  showHexDataToConsole(evtPackageArray, evtPackageArray.length());  }

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }break;


        case (ushort)EVT_ESTABLISH_CONNECTION_RESPONSE:   {
            QByteArray evtFeedBackToConsole, connected_BD_InvertedID;
            connected_BD_InvertedID[0] = evtPackageArray[0];

            evtFeedBackToConsole += "    'Establish Connection ' Response  (Cmd: ";
            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);

            evtFeedBackToConsole += ") : Well Connected!     Connected BD ID: ";
            evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(connected_BD_InvertedID);

            peerBD_Handle[establishedConnectionNum - 1].bdHandle    = (uchar)evtPackageArray[0];
            peerBD_Handle[establishedConnectionNum - 1].attId       = (uchar)evtPackageArray[1];

            if((uchar)evtPackageArray[1] == 0x00)  {   establishConnectionSuccess = false;
            }else evtFeedBackToConsole += "\nAttention! Something Wrong with the att byte value in Establish Conection Response!";

            evtFeedBackToConsole += "\n";

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
        }break;

        /*******************************************************************************************************************************************************************/
        case (ushort)EVT_CURRENT_CONNECTION_PARAMETERS_NOTIFICATION:   {
            if (evtPackageArray.length() == CONNECTION_PARAMETERS_NOTIFICATION_Parameter_Length)    {
                QByteArray evtFeedBackToConsole, connected_BD_InvertedID, attByte, HCI_Status, slaveLatency_Low, slaveLatency_High;
                int index = 0;
                ushort connectionIntvRaw, connectionIntvReal_100ms, supervisionTimeoutRaw, supervisionTimeoutReal;
                connected_BD_InvertedID[0] = evtPackageArray[index++];

                evtFeedBackToConsole += "=================================================\n";
                evtFeedBackToConsole += "      'Connection Parameters Notification' Event :\n";
                evtFeedBackToConsole += "---------------------------\n";
                evtFeedBackToConsole += "\t\tConnected BD IDs: \t";
                evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(connected_BD_InvertedID);
                evtFeedBackToConsole += "  \t(   Inverted Order   )";
                attByte[0] = evtPackageArray[index++];
                if((uchar)attByte[0] != 0x00)   {
                    evtFeedBackToConsole += "\nattByte = 0x";
                    evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(attByte);
                    evtFeedBackToConsole += "\tAttention! Something Wrong with the att byte value in Establish Conection Response!";
                }
                evtFeedBackToConsole += "\n\t\tHCI Status: \t0x";
                HCI_Status[0] = evtPackageArray[index];
                evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(HCI_Status);
                evtFeedBackToConsole += " \t( 0x00 means good )\n";
                if((uchar)evtPackageArray[index++] != 0x00)    evtFeedBackToConsole += "\tAttention! HCI Status is NOT good, check it out! \n";
                evtFeedBackToConsole += "\t\tConnection Interval: \t";

                connectionIntvRaw = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index + 1]);
                connectionIntvReal_100ms = TimeIntv_Calculation_Base + TimeIntv_Calculation_PerIncrease*(connectionIntvRaw - TimeIntv_Calculation_Base_Byte);
                evtFeedBackToConsole += QString::number((uint)(connectionIntvReal_100ms/100),10);
                evtFeedBackToConsole += ".";
                evtFeedBackToConsole += QString::number((uint)(connectionIntvReal_100ms%100),10);
                index += sizeof(ushort);

                evtFeedBackToConsole += " ms\n\t\tSlave Latency: \t0x";
                slaveLatency_Low[0] = evtPackageArray[index++];
                slaveLatency_High[0] = evtPackageArray[index++];
                evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(slaveLatency_High);
                evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(slaveLatency_Low);

                evtFeedBackToConsole += "\n\t\tSupervision Timeout: \t";
                supervisionTimeoutRaw = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index + 1]);
                supervisionTimeoutReal =  supervisionTimeoutRaw * 10;
                evtFeedBackToConsole += QString::number((uint)(supervisionTimeoutReal),10);
                evtFeedBackToConsole += " ms\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

                if (updateConnctionParaRequestSuccess)  {   connectionParameterSuccessfullyUpdated = true;  updateConnctionParaRequestSuccess = false;  }
            }else   {
                showHexDataToConsole(evtPackageArray, evtPackageArray.length());
                qDebug() << "CONNECTION_PARAMETERS Parameter Length Algorithm Wrong, check it out !";
            }

        }break;
        /****************************************************************************************************************************************/
        case (ushort)EVT_UPDATE_CONNECTION_PARAMETERS_NOTIFICATION:   {
            QByteArray evtFeedBackToConsole, connected_BD_InvertedID, attByte, HCI_Status, slaveLatency_Low, slaveLatency_High;
            int index = 0;
            ushort connectionIntvRaw, connectionIntvReal_100ms, supervisionTimeoutRaw, supervisionTimeoutReal;
            connected_BD_InvertedID[0] = evtPackageArray[index++];

            evtFeedBackToConsole += "=================================================\n";
            evtFeedBackToConsole += "      'Update Connection Parameters Notification' Event :\n";
            evtFeedBackToConsole += "---------------------------\n";
            evtFeedBackToConsole += "\t\tConnected BD IDs: \t";
            evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(connected_BD_InvertedID);
            evtFeedBackToConsole += "  \t(   Inverted Order   )";
            attByte[0] = evtPackageArray[index++];
            if((uchar)attByte[0] != 0x00)   {
                evtFeedBackToConsole += "\nattByte = 0x";
                evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(attByte);
                evtFeedBackToConsole += "\tAttention! Something Wrong with the att byte value in Establish Conection Response!\n";
            }

            evtFeedBackToConsole += "\n\t\tConnection Interval Minimum: \t";
            connectionIntvRaw = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index + 1]);
            connectionIntvReal_100ms = TimeIntv_Calculation_Base + TimeIntv_Calculation_PerIncrease*(connectionIntvRaw - TimeIntv_Calculation_Base_Byte);
            evtFeedBackToConsole += QString::number((uint)(connectionIntvReal_100ms/100),10);
            evtFeedBackToConsole += ".";
            evtFeedBackToConsole += QString::number((uint)(connectionIntvReal_100ms%100),10);
            index += sizeof(ushort);

            evtFeedBackToConsole += "ms\n\t\tConnection Interval Maximum: \t";
            connectionIntvRaw = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index + 1]);
            connectionIntvReal_100ms = TimeIntv_Calculation_Base + TimeIntv_Calculation_PerIncrease*(connectionIntvRaw - TimeIntv_Calculation_Base_Byte);
            evtFeedBackToConsole += QString::number((uint)(connectionIntvReal_100ms/100),10);
            evtFeedBackToConsole += ".";
            evtFeedBackToConsole += QString::number((uint)(connectionIntvReal_100ms%100),10);
            index += sizeof(ushort);

            evtFeedBackToConsole += " ms\n\t\tSlave Latency: \t0x";
            slaveLatency_Low[0] = evtPackageArray[index++];
            slaveLatency_High[0] = evtPackageArray[index++];
            evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(slaveLatency_High);
            evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(slaveLatency_Low);

            evtFeedBackToConsole += "\n\t\tSupervision Timeout: \t";
            supervisionTimeoutRaw = (uchar)evtPackageArray[index] + 16*16*((uchar)evtPackageArray[index + 1]);
            supervisionTimeoutReal =  supervisionTimeoutRaw * 10;
            evtFeedBackToConsole += QString::number((uint)(supervisionTimeoutReal),10);
            evtFeedBackToConsole += " ms\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

            ushort accepted = CYBLE_L2CAP_CONN_PARAM_ACCEPTED;
            ushort rejected = CYBLE_L2CAP_CONN_PARAM_REJECTED;
            QByteArray updateHandle, acceptedByteArray, rejectedByteArray;
            updateHandle[0] = connected_BD_InvertedID[0];
            updateHandle[1] = attByte[0];
            acceptedByteArray[0] = dongleProtocol->get_L8((uchar*)&accepted);
            acceptedByteArray[1] = dongleProtocol->get_H8((uchar*)&accepted);
            rejectedByteArray[0] = dongleProtocol->get_L8((uchar*)&rejected);
            rejectedByteArray[1] = dongleProtocol->get_H8((uchar*)&rejected);

            emit emitGapCmd_UpdateConnectionParamResp(updateHandle, acceptedByteArray);
//            emit emitGapCmd_UpdateConnectionParamResp(updateHandle, rejectedByteArray);
            updateConnctionParaRequestSent = true;

        }break;

        /****************************************************************************************************************************************/
        case (ushort)EVT_GET_DEVICE_IO_CAPABILITIES_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_DEVICE_IO_CAPABILITIES_RESPONSE !";   }break;
        case (ushort)EVT_GET_BLUETOOTH_DEVICE_ADDRESS_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_BLUETOOTH_DEVICE_ADDRESS_RESPONSE !";   }break;
        case (ushort)EVT_GET_PEER_BLUETOOTH_DEVICE_ADDRESS_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_PEER_BLUETOOTH_DEVICE_ADDRESS_RESPONSE !";   }break;
        case (ushort)EVT_GET_PEER_DEVICE_HANDLE_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_PEER_DEVICE_HANDLE_RESPONSE !";   }break;

        case (ushort)EVT_GET_CONNECTION_PARAMETERS_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_CONNECTION_PARAMETERS_RESPONSE !";   }break;
        case (ushort)EVT_GET_SCAN_PARAMETERS_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_SCAN_PARAMETERS_RESPONSE !";   }break;
        case (ushort)EVT_GET_LOCAL_DEVICE_SECURITY_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_LOCAL_DEVICE_SECURITY_RESPONSE !";   }break;
        case (ushort)EVT_GET_PEER_DEVICE_SECURITY_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_PEER_DEVICE_SECURITY_RESPONSE !";   }break;
        case (ushort)EVT_GENERATE_BD_ADDR_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GENERATE_BD_ADDR_RESPONSE !";   }break;

        case (ushort)EVT_GENERATE_KEYS_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GENERATE_KEYS_RESPONSE !";   }break;
        case (ushort)EVT_PASSKEY_ENTRY_REQUEST:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_PASSKEY_ENTRY_REQUEST !";   }break;
        case (ushort)EVT_PASSKEY_DISPLAY_REQUEST:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_PASSKEY_DISPLAY_REQUEST !";   }break;
        case (ushort)EVT_CONNECTION_TERMINATED_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_CONNECTION_TERMINATED_NOTIFICATION !";   }break;

        case (ushort)EVT_PAIRING_REQUEST_RECEIVED_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_PAIRING_REQUEST_RECEIVED_NOTIFICATION !";   }break;
        case (ushort)EVT_AUTHENTICATION_ERROR_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_AUTHENTICATION_ERROR_NOTIFICATION !";   }break;
        case (ushort)EVT_CONNECTION_CANCELLED_NOTIFICATION:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_CONNECTION_CANCELLED_NOTIFICATION !";   }break;
        case (ushort)EVT_GET_PEER_DEVICE_SECURITY_KEYS_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_PEER_DEVICE_SECURITY_KEYS_RESPONSE !";   }break;

        /****************************************************************************************************************************************/

        default:    {
            qDebug() << "GAP Events Handler ! Something Wrong with me !";
            showHexDataToConsole(evtPackageArray, evtPackageArray.length());
        }break;
    }

}

void SenDecodeCySmtProtocolPackageObject::generalEventHandler(ushort evtType, QByteArray evtPackageArray, int packageLength)
{
//    qDebug() << "I am in generalEventHandler, evtPackageArray.length() = " << evtPackageArray.length();
    if (evtPackageArray.length() < packageLength)
        qDebug() << "evtPackageArray.length() = " << evtPackageArray.length() << ", packageLength = " << packageLength;

    evtPackageArray = extract_Y_Bytes_Since_X_From(evtPackageArray, packageLength, 0);
//    check_evtPackageArray.clear();
//    check_evtPackageArray = evtPackageArray;//delete me after finish

    QByteArray unDecodePackageData;
    evtPackageArray = evtPackageArray.remove(0, sizeof(evtType));
    unDecodePackageData = evtPackageArray;
    // unReadPackageData contains (cmdOpCode and) parameters, might with additionals. (They are data after evtOpCode = evtType)
    switch(evtType)    {
        case (ushort)EVT_COMMAND_STATUS:   {
            bool isGattCommand = false;
            bool isGapCommand = false;
            uchar cmdOpCode_L = (uchar)evtPackageArray[DATA_LOW_8BIT];
            uchar cmdOpCode_H = ((uchar)evtPackageArray[DATA_HIGH_8BIT] & Sen_CMD_ID_HIGH_8BIT_MASK);
            ushort cmdOpCode = cmdOpCode_L + 16*16*cmdOpCode_H;

            if (evtPackageArray.length() == ((int)sizeof(ushort) + (int)sizeof(ushort))// cmdOpCode + evtStatus
                  && (   cmdOpCode_H  == General_Commands
                      || cmdOpCode_H == L2CAP_Commands
                      || cmdOpCode_H == GATT_GAP_Commands))  {


                uchar cmdOpCodeID = cmdOpCode & Sen_CMD_ID_LOW_8BIT_MASK;
                ushort evtStatus = (uchar)evtPackageArray[DATA_LOW_8BIT + (int)sizeof(ushort)]
                                + 16*16*(uchar)evtPackageArray[DATA_HIGH_8BIT + (int)sizeof(ushort)];
                // Map cmdOpcode, evtStatus
                // Get Cmd_Name string; evtStatus description string
                QByteArray evtFeedBackToConsole;
                evtFeedBackToConsole += "  Cmd Status:\tCommand:\t";

                switch (cmdOpCode_H) {
                    case General_Commands:
                        evtFeedBackToConsole += dongleProtocol->getGeneralCmdStringFromArray((int)cmdOpCodeID);
                        break;

                    case L2CAP_Commands:
                        evtFeedBackToConsole += dongleProtocol->getL2CAP_CmdStringFromArray((int)cmdOpCodeID);
                        break;

                    case GATT_GAP_Commands:
                        if ((cmdOpCode_L & Sen_CMD_GAP_OR_GATT_MASK) == Sen_CMD_GAP_GROUP)  {
                            isGapCommand = true;
                            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
                        }else{   // Sen_CMD_GATT_GROUP
                            isGattCommand = true;
                            evtFeedBackToConsole += dongleProtocol->getGattCmdStringFromArray((int)cmdOpCodeID);
                        }break;

                    default:    {
                        qDebug() << "This is EVT_COMMAND_STATUS. cmdOpCode Algorithm Wrong !!";
                        showHexDataToConsole(evtPackageArray, evtPackageArray.length());
                    }break;
                }

                evtFeedBackToConsole += "\tStatus:    ";
                evtFeedBackToConsole += dongleProtocol->getStatusStringFromArray((int)evtStatus);
                evtFeedBackToConsole += "\n";

                if (isGapCommand == true && cmdOpCodeID == (uchar)Cmd_Establish_Connection_Api && (int)evtStatus == 0)  {
                    establishedConnectionNum++;
                    establishConnectionSuccess = true;
                }

                emit emitDataToConsole(evtFeedBackToConsole, mySensorID);
            }else   {
                qDebug() << "This is EVT_COMMAND_STATUS. Evt Algorithm Wrong or readPackage Size Wrong!!";
                showHexDataToConsole(evtPackageArray, evtPackageArray.length());
            }
        }break;
    /*****************************************************************************************************************/
        case (ushort)EVT_COMMAND_COMPLETE:   {

            bool isGeneralCommand = false;
            bool isGapCommand = false;
            bool isGattCommand = false;
            bool isL2CAP_Command = false;

            uchar cmdOpCode_L = (uchar)evtPackageArray[DATA_LOW_8BIT];
            uchar cmdOpCode_H = ((uchar)evtPackageArray[DATA_HIGH_8BIT] & Sen_CMD_ID_HIGH_8BIT_MASK);
            ushort cmdOpCode = cmdOpCode_L + 16*16*cmdOpCode_H;

            if (evtPackageArray.length() == ((int)sizeof(ushort) + (int)sizeof(ushort)) // cmdOpCode + evtStatus
                    && (   cmdOpCode_H  == (uchar)General_Commands || cmdOpCode_H == (uchar)L2CAP_Commands || cmdOpCode_H == (uchar)GATT_GAP_Commands))  {

                uchar cmdOpCodeID = cmdOpCode & Sen_CMD_ID_LOW_8BIT_MASK;
                ushort evtStatus = (uchar)evtPackageArray[DATA_LOW_8BIT + (int)sizeof(ushort)]
                                + 16*16*(uchar)evtPackageArray[DATA_HIGH_8BIT + (int)sizeof(ushort)];
                QByteArray evtFeedBackToConsole;
                evtFeedBackToConsole += "  Cmd Complete:\tCommand:\t";

                switch (cmdOpCode_H) {
                    case General_Commands:  { isGeneralCommand = true; evtFeedBackToConsole += dongleProtocol->getGeneralCmdStringFromArray((int)cmdOpCodeID);} break;
                    case L2CAP_Commands:    { isL2CAP_Command = true; evtFeedBackToConsole += dongleProtocol->getL2CAP_CmdStringFromArray((int)cmdOpCodeID);} break;
                    case GATT_GAP_Commands: {
                        if ((cmdOpCode_L & Sen_CMD_GAP_OR_GATT_MASK) == Sen_CMD_GAP_GROUP)   {
                            isGapCommand = true;
                            evtFeedBackToConsole += dongleProtocol->getGapCmdStringFromArray((int)cmdOpCodeID);
                        }else   {
                            isGattCommand = true;
                            evtFeedBackToConsole += dongleProtocol->getGattCmdStringFromArray((int)cmdOpCodeID);
                        }

                    }break;// Sen_CMD_GATT_GROUP

                    default:    {
                        qDebug() << "This is EVT_COMMAND_STATUS. cmdOpCode Algorithm Wrong !!";
                        showHexDataToConsole(evtPackageArray, evtPackageArray.length());
                    }break;
                }

                evtFeedBackToConsole += "\tStatus:    ";
                evtFeedBackToConsole += dongleProtocol->getStatusStringFromArray((int)evtStatus);
                evtFeedBackToConsole += "\n\n";

                emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

                /***************************************************************************************************************************************************************/
                if (isGeneralCommand == true && cmdOpCodeID == (uchar)Cmd_Get_Device_Id_Api)   emit emitSendCmdInitBleStack();//emit emitCmdComplete(cmdOpCodeID);
                if (isGeneralCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Init_Ble_Stack_Api && (int)evtStatus == 0)    emit emitGapCmdSetConnectionParameters();
                if (isGapCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Set_Connection_Parameters_Api && (int)evtStatus == 0)    emit emitGapCmdSetLocalDeviceSecurity();
                if (isGapCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Set_Local_Device_Security_Api && (int)evtStatus == 0)    emit emitGapCmdStartScan();
                if (resolveSetPeerAddAutoAfterScanStop == true && isGapCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Stop_Scan_Api && (int)evtStatus == 0)     {
                    emit emitGapCmd_ResolveAndSetPeerAddr();
                    resolveSetPeerAddAutoAfterScanStop = false;
                }
                if (resolve_SetPeerAddGetGoodResponse == true && isGapCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Resolve_Set_Peer_Addr_Api && (int)evtStatus == 0)     {
                    emit emitGapCmd_EstablishConnection();
                    resolve_SetPeerAddGetGoodResponse = false;
                }
                if (isGapCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Establish_Connection_Api && (int)evtStatus == 0)    {    emit emitGattCmd_ExchangeGatt_MTU_Size(); initialDeviceServicesList();  }
//                if (isGapCommand == true && (int)cmdOpCodeID == (uchar)Cmd_UpdateConnectionParam_Resp_Api && (int)evtStatus == 0)    {
//                    if (updateConnctionParaRequestSent) {
//                        updateConnctionParaRequestSuccess = true;
//                        updateConnctionParaRequestSent = false;
//                    }else   if (connectionParameterSuccessfullyUpdated)  {
//                        emit emitGattCmd_ExchangeGatt_MTU_Size();
//                        initialDeviceServicesList();
//                        connectionParameterSuccessfullyUpdated = false;
//                    }else   qDebug() << "Update Connection Parameter Request Algorithm Wrong! Check it out !";
//                }

                // This below is comment selected because the update of connection parameter
//                if (isGattCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Exchange_GATT_MTU_Size_Api && (int)evtStatus == 0)    emit emitGattCmd_DiscoverAllPrimaryServices();
                if (isGattCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Exchange_GATT_MTU_Size_Api && (int)evtStatus == 0)    emit emitGattCMD_MotionServiceActive();

                if (isGattCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Discover_All_Primary_Services_Api && (int)evtStatus == 0)     emit emitDataToConsole(bandPassModeInputHint, mySensorID);
                if (isGattCommand == true && (int)cmdOpCodeID == (uchar)Cmd_Discover_All_Characteristics_Api && (int)evtStatus == 0)     {      emit emitGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(chosenServiceIndex + 1, mySensorID);   chosenServiceIndex = -1;    }


                /***************************************************************************************************************************************************************/


            }else   {
                qDebug() << "This is EVT_COMMAND_STATUS. Evt Algorithm Wrong or readPackage Size Wrong!!";
                showHexDataToConsole(evtPackageArray, evtPackageArray.length());
            }
        }break;
    /*****************************************************************************************************************/
        case (ushort)EVT_GET_DEVICE_ID_RESPONSE:   {
            if (evtPackageArray.length() == sizeof(int) )  {            //  sizeof(DeviceID)
                uint deviceID = (uchar)evtPackageArray[DATA_LOW_8BIT]
                        + 16*16*(uchar)evtPackageArray[DATA_HIGH_8BIT]
                        + (16*16)*(16*16)*(uchar)evtPackageArray[DATA_LOW_8BIT + (int)sizeof(ushort)]
                        + (16*16)*(16*16)*(16*16)*(uchar)evtPackageArray[DATA_HIGH_8BIT + (int)sizeof(ushort)];
                QByteArray evtFeedBackToConsole;
                evtFeedBackToConsole += "    Got Device ID:\t0x";
                evtFeedBackToConsole += dongleProtocol->getReversedHexStringFromUnSignedRaw(evtPackageArray);
                if (deviceID == (uint)BLE_DONGLE_ID)   evtFeedBackToConsole += "\tThis is BLE_DONGLE";
                else     evtFeedBackToConsole += "\tNot Sen BLE_DONGLE";
                evtFeedBackToConsole += "\n";

                emit emitDataToConsole(evtFeedBackToConsole, mySensorID);

//                emit emitSendCmdInitBleStack();
            }else   {
                qDebug() << "This is EVT_GET_DEVICE_ID_RESPONSE. Evt Algorithm Wrong or readPackage Size Wrong!!";
                showHexDataToConsole(evtPackageArray, evtPackageArray.length());
            }
        }break;
    /*******************************************************************/
        case (ushort)EVT_GET_SUPPORTED_TOOL_VERSION_RESPONSE:   { showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_SUPPORTED_TOOL_VERSION_RESPONSE !";   }break;
        case (ushort)EVT_GET_FIRMWARE_VERSION_RESPONSE:   {  showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_FIRMWARE_VERSION_RESPONSE !";   }break;
        case (ushort)EVT_GET_BLE_STACK_VERSION_RESPONSE:   {  showHexDataToConsole(evtPackageArray, evtPackageArray.length()); qDebug() << "EVT_GET_BLE_STACK_VERSION_RESPONSE !";   }break;
        case (ushort)EVT_REPORT_STACK_MISC_STATUS:   {  showHexDataToConsole(evtPackageArray, evtPackageArray.length()); qDebug() << "EVT_REPORT_STACK_MISC_STATUS !";   }break;
        case (ushort)EVT_GET_SUPPORTED_GAP_ROLES_RESPONSE:   {  showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_SUPPORTED_GAP_ROLES_RESPONSE !";   }break;
        case (ushort)EVT_GET_CURRENT_GAP_ROLE_RESPONSE:   {     showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_CURRENT_GAP_ROLE_RESPONSE !";        }break;
        case (ushort)EVT_GET_SUPPORTED_GATT_ROLES_RESPONSE:   {  showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_SUPPORTED_GATT_ROLES_RESPONSE !";        }break;
        case (ushort)EVT_GET_CURRENT_GATT_ROLE_RESPONSE:   {  showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_CURRENT_GATT_ROLE_RESPONSE !";      }break;
        case (ushort)EVT_GET_RSSI_RESPONSE:   {   showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_RSSI_RESPONSE !";    }break;
        case (ushort)EVT_GET_DEVICE_DESCRIPTION_RESPONSE:   {   showHexDataToConsole(evtPackageArray, evtPackageArray.length());qDebug() << "EVT_GET_DEVICE_DESCRIPTION_RESPONSE !";        }break;
        case (ushort)EVT_GET_HARDWARE_VERSION_RESPONSE:   {    showHexDataToConsole(evtPackageArray, evtPackageArray.length()); qDebug() << "EVT_GET_HARDWARE_VERSION_RESPONSE !";        }break;
      /*******************************************************************/
        default:    {    qDebug() << "Event algorithm Wrong! I am not a general Event!";      }break;
    }

//    if (unDecodePackageData.length() > 0)   {
//        ushort nextUshortValue = (uchar)unDecodePackageData[DATA_LOW_8BIT]
//                        + 16*16*(uchar)unDecodePackageData[DATA_HIGH_8BIT];
//        ushort next2nd_UshortValue = (uchar)unDecodePackageData[DATA_LOW_8BIT + (int)sizeof(ushort)]
//                        + 16*16*(uchar)unDecodePackageData[DATA_HIGH_8BIT + (int)sizeof(ushort)];
//        QByteArray evtTypeByteArray,nextUshortByteArray,next2ndUshortByteArray;
        // upside down intended for Hex String Transform
//        evtTypeByteArray[0] = dongleProtocol->get_H8((uchar*)&evtType);         evtTypeByteArray[1] = dongleProtocol->get_L8((uchar*)&evtType);
//        nextUshortByteArray[0] = dongleProtocol->get_H8((uchar*)&nextUshortValue);     nextUshortByteArray[1] = dongleProtocol->get_L8((uchar*)&nextUshortValue);
//        next2ndUshortByteArray[0] = dongleProtocol->get_H8((uchar*)&next2nd_UshortValue);     next2ndUshortByteArray[1] = dongleProtocol->get_L8((uchar*)&next2nd_UshortValue);
//        qDebug() << "\t\t\t\tevtType = 0x" << (QString)evtTypeByteArray.toHex()
//                 << ",   cmdOpCode/ next1st_UshortValue = 0x" << (QString)nextUshortByteArray.toHex()
//                 << ",   evtStatus/ next2nd_UshortValue = 0x" << (QString)next2ndUshortByteArray.toHex();
//    }
}

void SenDecodeCySmtProtocolPackageObject::L2CAP_EventHandler(ushort evtType, uchar cmdOpCodeID, QByteArray evtPackageArray, int packageLength)
{
    qDebug() << "I am in L2CAP Events Handler!";
    showHexDataToConsole(evtPackageArray, evtPackageArray.length());

    if (evtPackageArray.length() < packageLength)
        qDebug() << "evtPackageArray.length() = " << evtPackageArray.length() << ", packageLength = " << packageLength;

    evtPackageArray = extract_Y_Bytes_Since_X_From(evtPackageArray, packageLength, 0);

}

QByteArray SenDecodeCySmtProtocolPackageObject::extract_Y_Bytes_Since_X_From(QByteArray anyUsignedData, int length, int startPoint)
{
    QByteArray newY_BytesArray;
    for (int i=0, j=startPoint; i<length; i++, j++)  {
        newY_BytesArray[i] = anyUsignedData[j];
    }

    return newY_BytesArray;
}

QByteArray SenDecodeCySmtProtocolPackageObject::getAddFeedBackStringFromAddStartPointer(uchar* peerBdAddr, int dataLen)
{
    QByteArray addFeedBackStringArray;

    int i=0;
    for (; i<(dataLen-1); i++) {
        QByteArray peerBdAddrDatum;
        peerBdAddrDatum += peerBdAddr[i];
        addFeedBackStringArray += peerBdAddrDatum.toHex();
        addFeedBackStringArray +=":";
    }
    QByteArray peerBdAddrDatum_last;
    peerBdAddrDatum_last += peerBdAddr[i];
    addFeedBackStringArray += peerBdAddrDatum_last.toHex();

    return addFeedBackStringArray;
}

QByteArray SenDecodeCySmtProtocolPackageObject::getHexFeedBackStringFromDataStartPointer(uchar* peerBdAddr, int dataLen)
{
    QByteArray addFeedBackStringArray;
    uchar length_DataGroup, type_DataGroup;

    int i=0;
    for (; i<(dataLen-1); i++) {
        QByteArray peerBdAddrDatum;
        peerBdAddrDatum += peerBdAddr[i];
        addFeedBackStringArray += peerBdAddrDatum.toHex();
        addFeedBackStringArray +=":";
    }
    QByteArray peerBdAddrDatum_last;
    peerBdAddrDatum_last += peerBdAddr[i];
    addFeedBackStringArray += peerBdAddrDatum_last.toHex();

    i = 0;
    for(; i<(dataLen-1); )  {
        length_DataGroup = peerBdAddr[i++];
        type_DataGroup = peerBdAddr[i++];
        if (type_DataGroup == Sen_BLE_ADV_ATT_Type)     {
            addFeedBackStringArray += "\n\t   Device: ";
            for (int j=0; j<(length_DataGroup - 1); j++,i++)  {   addFeedBackStringArray += peerBdAddr[i];    }
        }else   {
            i += (length_DataGroup - 1);
        }
   }

    return addFeedBackStringArray;
}

void SenDecodeCySmtProtocolPackageObject::showBasicResponseFormat(QByteArray rawDonglePackage)
{
    QByteArray hexData, hexHigh8Bit, hexLow8Bit;
    hexData.clear();

    for (int i = 0, j = 1; i < rawDonglePackage.length(); i++, j++)  {
        if  (j == 1)     hexData += "Header:\t\t0x    ";
        if  (j == 3)    hexData += "LengthInBytes:\t0x    ";
        if  (j == 5)    hexData += "EvtOpCode:\t\t0x    ";
        if  (j == 7)    hexData += "CmdOpCode:\t0x    ";
        if  (j == 9)    hexData += "Status:\t\t0x    ";
        if (j%2 != 0)   {
            hexLow8Bit += rawDonglePackage[i];
        }else   {
            hexHigh8Bit += rawDonglePackage[i];
            hexData += hexHigh8Bit.toHex();
            hexData += hexLow8Bit.toHex();

            hexHigh8Bit.clear();
            hexLow8Bit.clear();

            hexData += "\n";
        }
//        hexData += "\ti = ";
//        hexData += QString::number(i);

        if ((i+1)%10 == 0)  {   hexData += "********************************\n"; j = 0;   }
    }

    hexData += "~~~~~~~~~~~~~~~~~~~~~~~~\n";
    emit emitDataToConsole(hexData, mySensorID);
}

void SenDecodeCySmtProtocolPackageObject::showHexDataToConsole(QByteArray anyUnsignedData, int arrayLenth)
{
    if (anyUnsignedData.length() < arrayLenth)
        qDebug() << "anyUnsignedData.length() = " << anyUnsignedData.length() << ", arrayLenth = " << arrayLenth;
    anyUnsignedData = extract_Y_Bytes_Since_X_From(anyUnsignedData, arrayLenth, 0);

    QByteArray hexData, hexDatum;
    for (int i = 0; i < anyUnsignedData.length(); i++)  {
        hexData += "0x    ";
        hexDatum += anyUnsignedData[i];
        hexData += hexDatum.toHex();
        hexDatum.clear();

        hexData += "\ti = ";
        hexData += QString::number(i);
        hexData += "\n";
    }

    emit emitDataToConsole(hexData, 0); // 0 means read, wrong data happened     // mySensorID);
}

void SenDecodeCySmtProtocolPackageObject::slotShowHexDataToConsole(QByteArray anyUnsignedData)
{
    QByteArray hexData, hexDatum;
    for (int i = 0; i < anyUnsignedData.length(); i++)  {
        hexData += "0x    ";
        hexDatum += anyUnsignedData[i];
        hexData += hexDatum.toHex();
        hexDatum.clear();

        hexData += "\ti = ";
        hexData += QString::number(i);
        hexData += "\n";
    }

    emit emitDataToConsole(hexData, 0); // 0 means read, wrong data happened     // mySensorID);
}


