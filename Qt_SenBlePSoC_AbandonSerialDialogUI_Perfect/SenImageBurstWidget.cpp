#include "SenImageBurstWidget.h"

SenImageBurstWidget::SenImageBurstWidget(QWidget *parent, int givenSerialNum) : QWidget(parent)
  ,pixelsDataSum(0),pixelRowStart(0),pixelRowEnd(0),PixelColumnStart(0),PixelColumnEnd(0)
  ,errorCount(0),framesCount(0)
{
      serialObject_ID = givenSerialNum;

      image = QImage(30, 30, QImage::Format_RGB32);
      zoom = 15;
      curColor = Qt::black;
      QRgb SenPixelValue;

      for (int i = 0; i < image.width(); ++i) {
          for (int j = 0; j < image.height(); ++j) {
              float ratioArg = (image.width()-1)*(image.height()-1)/255.0;
              unsigned int SenPixeldepth = (unsigned int)(i*j/ratioArg);
              SenPixelValue = qRgb(SenPixeldepth, SenPixeldepth, SenPixeldepth);
              image.setPixel(i, j, SenPixelValue);
//              qDebug() << "i = " << i << ",    j = " << j << ",    SenPixeldepth = " << SenPixeldepth;
          }
      }
//      qDebug() << "width = " << image.width() << ",    height = " << image.height();
}

SenImageBurstWidget::~SenImageBurstWidget()
{
}

void SenImageBurstWidget::setZoomFactor(int newZoom)
{
    if (newZoom < 1)
        newZoom = 1;

    if (newZoom != zoom) {
        zoom = newZoom;
        update();
        updateGeometry();
    }
}

void SenImageBurstWidget::setPenColor(const QColor &newColor)
{
    curColor = newColor;
}

void SenImageBurstWidget::setPixelsImage(const QImage &newImage)
{
    if (newImage != image) {
        image = newImage.convertToFormat(QImage::Format_ARGB32);
        update();
        updateGeometry();
    }
}

QSize SenImageBurstWidget::sizeHint() const
{
    QSize size = zoom * image.size();
    if (zoom >= 3)
        size += QSize(1, 1);
    return size;
}


void SenImageBurstWidget::SenPixelUpdate(const QPoint &pos, bool opaque)
{
    int i = pos.x() / zoom;
    int j = pos.y() / zoom;

    if (image.rect().contains(i, j)) {
        if (opaque) {
            image.setPixel(i, j, readPenColor().rgba());
        } else {
            image.setPixel(i, j, qRgba(0, 0, 0, 0));
        }

        update(SenGetPixelRect(i, j));
    }
}

QRect SenImageBurstWidget::SenGetPixelRect(int i, int j) const
{
    if (zoom >= 3) {
        return QRect(zoom * i + 1, zoom * j + 1, zoom - 1, zoom - 1);
    } else {
        return QRect(zoom * i, zoom * j, zoom, zoom);
    }
}


void SenImageBurstWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int side = qMin(width(), height());
    zoom = side/image.height();

    if (zoom >= 3) {
        painter.setPen(palette().foreground().color());
        for (int i = 0; i <= image.width(); ++i)// Draw column grids
            painter.drawLine(zoom * i, 0,
                             zoom * i, zoom * image.height());
        for (int j = 0; j <= image.height(); ++j)// Draw row grids
            painter.drawLine(0, zoom * j,
                             zoom * image.width(), zoom * j);
    }

    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRect rect = SenGetPixelRect(i, j);
            if (!event->region().intersected(rect).isEmpty()) {
                QColor color = QColor::fromRgba(image.pixel(i, j));
                if (color.alpha() < 255)
                    painter.fillRect(rect, Qt::white);
                painter.fillRect(rect, color);
            }
        }
    }
}

void SenImageBurstWidget::SenPixelsDataProcess(QByteArray SenPixelsBurstByteArray, int receiveTimesCount)
{
//    QStringList pixelsDataStringList = pixelsDataString.split(",", QString::SkipEmptyParts);
//    QString pixelsDataStringList = pixelsDataString;

    int columnsOcupationCount = 0;

    // Calculate Sum Image Pixels
    if (receiveTimesCount == 1)    {   pixelsDataSum = SenPixelsBurstByteArray.length();    }
    else    {                          pixelsDataSum += SenPixelsBurstByteArray.length();    }
//    qDebug() << "The "<<receiveTimesCount << "th Time Receive, Pixels Num = " << SenPixelsBurstByteArray.length() << ",    pixelsDataSum = " << pixelsDataSum ;

    if(SenPixelsBurstByteArray.length() > image.height() * image.width())  {
        emit emitBurstBegunFalse();
        SenDeadDelay(200);
        SenDelay(50);
        qDebug() << "Error Attention! LinePixelsData Wrong, SenPixelsBurstByteArray.length() = "<< SenPixelsBurstByteArray.length();
    }

//    if(SenPixelsBurstByteArray.length() > image.height())  {
//        if (++errorCount >= 2)  {
//            emit emitBurstStopForGoodNext();// Stop automatically after a really bad frame(errorCount should == 0 all the time)
//        }
//    }else   { errorCount = 0;   }


    // if No Error in Calculation, Reset Image Pixels and Update
    if (pixelsDataSum > image.height() * image.width()) {
        emit emitBurstBegunFalse();
        qDebug() << "Error Attention! pixelsDataSum is too big, = "<< pixelsDataSum;
    }else   {
        int SenPixelsBurstByteArrayIndex = 0;
        int oldSumFullColumnsOcupation = 0, oldSumAllColumnsOcupation = 0, oldSumColumnsRemainder = 0;
        int newSumFullColumOcupation = 0, newSumAllColumOcupation = 0, newSumColumnsRemainder = 0;

        // Calculate: PixelColumnStart, PixelColumnEnd, columnsOcupationCount, newSumColumnsRemainder;
        if (receiveTimesCount == 1)    {
            PixelColumnStart = (image.width() - 1);

            newSumColumnsRemainder = SenPixelsBurstByteArray.length() % image.height();
            if(newSumColumnsRemainder == 0) {
                columnsOcupationCount = (SenPixelsBurstByteArray.length() / image.height());
            }else   {
                columnsOcupationCount = (SenPixelsBurstByteArray.length() / image.height()) + 1;
            }
            PixelColumnEnd = image.width() - columnsOcupationCount;
        }
        else    {// Not the first time, which means old data exist
            if( ( pixelsDataSum - SenPixelsBurstByteArray.length() ) % image.height() == 0) {
                         PixelColumnStart = PixelColumnEnd - 1;
            }else    {   PixelColumnStart = PixelColumnEnd;       }

            oldSumFullColumnsOcupation = ( pixelsDataSum - SenPixelsBurstByteArray.length() ) / image.height();
            oldSumColumnsRemainder = ( pixelsDataSum - SenPixelsBurstByteArray.length() ) % image.height();
            if (oldSumColumnsRemainder == 0)    {   oldSumAllColumnsOcupation = oldSumFullColumnsOcupation; }
            else   {                                oldSumAllColumnsOcupation = oldSumFullColumnsOcupation + 1; }

            if (oldSumFullColumnsOcupation != (image.width() - 1 - PixelColumnStart) )  {
                qDebug() << "Attention !!!  ColumnsOcupation Algrithm Wrong !!!!!!!!";
                qDebug() << "Sensor " << serialObject_ID << " problem! oldSumFullColumnsOcupation = " << oldSumFullColumnsOcupation << ",    PixelColumnStart = " << PixelColumnStart << ",    PixelColumnEnd = " << PixelColumnEnd << ",    receiveTimesCount = " << receiveTimesCount;
                while(1)  {
                    ;
                }
            }

            newSumFullColumOcupation =  pixelsDataSum / image.height();
            newSumColumnsRemainder = pixelsDataSum % image.height();
            if (newSumColumnsRemainder == 0)    {   newSumAllColumOcupation = newSumFullColumOcupation;
            }else    {    newSumAllColumOcupation = newSumFullColumOcupation + 1; }

            columnsOcupationCount = newSumAllColumOcupation - oldSumFullColumnsOcupation;//[0,4]

            PixelColumnEnd = PixelColumnStart - columnsOcupationCount + 1;
//            qDebug() << "PixelColumnStart = " << PixelColumnStart << ",    PixelColumnEnd = " << PixelColumnEnd << ",    columnsOcupationCount = " << columnsOcupationCount<< "oldSumAllColumnsOcupation = " << oldSumAllColumnsOcupation << ",    newSumAllColumOcupation = " << newSumAllColumOcupation;
        }


        for (int i = PixelColumnStart; i >= PixelColumnEnd; i--) {  //Column No.(decrease) Start No. >= End No.
            // Calculate: PixelRowStart, PixelRowEnd;
            if (receiveTimesCount == 1)    {
                pixelRowStart = 0;

                if (i == PixelColumnEnd)  {
                    if (newSumColumnsRemainder == 0)    {
                        pixelRowEnd = image.height() - 1;
                    }else   {
                        pixelRowEnd = newSumColumnsRemainder - 1;
                    }
                }else   {
                    pixelRowEnd = image.height() - 1;
                }
            } else  {// Not the first time, which means pixelRowStart could be other than 0
                if (pixelRowEnd == image.height() - 1)  {   pixelRowStart = 0;
                }else   {                                   pixelRowStart = pixelRowEnd + 1;    }

                if (i == PixelColumnEnd)  {
                    if (newSumColumnsRemainder == 0)    {
                        pixelRowEnd = image.height() - 1;
                    }else   {
                        pixelRowEnd = newSumColumnsRemainder - 1;
                    }
                }else   {
                    pixelRowEnd = image.height() - 1;
                }
            }

            for (int j = (image.height() - 1 - pixelRowStart); j >= (image.height() - 1 - pixelRowEnd); j--) {//Row No.(decrease)

                if((SenPixelsBurstByteArrayIndex+1)>SenPixelsBurstByteArray.length())
                {
                    qDebug() << "Attention:  SenPixelsBurstByteArrayIndex + 1 = " << SenPixelsBurstByteArrayIndex+1;
                    qDebug() << "Attention:  Column = " << i << ",    Row = " << j;
                    qDebug() << "Attention:  ColumnStart = " << PixelColumnStart << ",    PixelColumnEnd = " << PixelColumnEnd << ",    RowStart = " << pixelRowStart << ",    pixelRowEnd = " << pixelRowEnd;
                    while(1)  {
                        ;
                    }
                }
                else
                {
//                    bool ok;
//                    unsigned int SenPixeldepth = SenPixelsBurstByteArray[SenPixelsBurstByteArrayIndex++].toInt(&ok,16);// Integer OK
                    uchar SenPixeldepth = SenPixelsBurstByteArray[SenPixelsBurstByteArrayIndex++];
//                    qDebug() << "Column = " << i << ",    Row = " << j << ",    SenPixeldepth = " << SenPixeldepth;
                    QRgb SenPixelValue = qRgb(SenPixeldepth, SenPixeldepth, SenPixeldepth);
                    image.setPixel(i, j, SenPixelValue);
                }
            }
        }

        update();
        updateGeometry();

        if(pixelsDataSum == image.width()*image.height())  {
            framesCount++;
            qDebug() << "Sensor " << serialObject_ID << " Complete framesCount = " << framesCount;
        }

//        qDebug() << "pixelsDataString[.length()-1] = " << pixelsDataString[pixelsDataString.length()-1];
//        if(pixelsDataString[pixelsDataString.length()-1] == '\n')   {
//            qDebug() << "I am Here!! I am Here!! I am Here!! I am Here!! I am Here!! I am Here!! I am Here!!";
//        }

//        if(pixelsDataString.length()>=2)
//            qDebug() << "pixelsDataString[.length()-2] = " << pixelsDataString[pixelsDataString.length()-2];
    }//End of Reset Image Pixels and Update
}// End of SenPixelsDataProcess();


void SenImageBurstWidget::SenDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents,msec); // 100 milliseconds
}

void SenImageBurstWidget::SenDeadDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
     qDebug() << "I am dead, please wait........................"; // 100 milliseconds
}






