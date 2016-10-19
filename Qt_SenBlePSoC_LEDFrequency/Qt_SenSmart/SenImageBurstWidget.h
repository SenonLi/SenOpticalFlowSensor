#ifndef SENIMAGEBURSTWIDGET_H
#define SENIMAGEBURSTWIDGET_H

#include <QWidget>

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QRgb>

#include <QLabel>
#include <QStringList>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include <QTime>
#include <QCoreApplication>
#include <QByteArray>

class SenImageBurstWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor penColor READ readPenColor WRITE setPenColor)
    Q_PROPERTY(QImage pixelsImage READ readPixelsImage WRITE setPixelsImage)
    Q_PROPERTY(int zoomFactor READ readZoomFactor WRITE setZoomFactor)

public:
    explicit SenImageBurstWidget(QWidget *parent = 0, int givenSerialNum = 9);
    ~SenImageBurstWidget();

    void setPenColor(const QColor &newColor);
    QColor readPenColor() const { return curColor; }
    void setZoomFactor(int newZoom);
    int readZoomFactor() const { return zoom; }
    void setPixelsImage(const QImage &newImage);
    QImage readPixelsImage() const { return image; }
    QSize sizeHint() const;

    void senStopPrepare()   {pixelsDataSum=0; pixelRowStart=0; pixelRowEnd=0; PixelColumnStart=0;
                                PixelColumnEnd = 0; errorCount = 0; framesCount = 0; }

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor curColor;
    QImage image;
    int zoom;

    void SenPixelUpdate(const QPoint &pos, bool opaque);
    QRect SenGetPixelRect(int i, int j) const;

    void SenDelay(int msec);
    void SenDeadDelay(int msec);

    QLabel *pixelsBurstQLable;

    int pixelsDataSum, pixelRowStart, pixelRowEnd, PixelColumnStart, PixelColumnEnd;
    int errorCount,framesCount;

    int serialObject_ID;

signals:
    void emitBurstBegunFalse();
    void emitBurstStopForGoodNext();

public slots:
    void SenPixelsDataProcess(QByteArray SenPixelsBurstByteArray, int receiveTimesCount);
};

#endif // SENIMAGEBURSTWIDGET_H
