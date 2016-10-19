
#include "SenConsole.h"

SenConsole::SenConsole(QWidget *parent)
    : QPlainTextEdit(parent)
    , localEchoEnabled(false), keyPressBlockingActive(false), lineDebugActive(false)
{
    /////
    QFont timesFont("Times", 11);
    zeroSerialCharFormat.setFont(timesFont);
    zeroSerialCharFormat.setForeground((QBrush)Qt::red);
    firstSerialCharFormat.setFont(timesFont);
    firstSerialCharFormat.setForeground((QBrush)Qt::green);
    secondSerialCharFormat.setFont(timesFont);
    secondSerialCharFormat.setForeground((QBrush)QColor(255,255,0));
    /////
    document()->setMaximumBlockCount(1000);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);

}

void SenConsole::putData(const QByteArray &data, int colorNumber)
{
    /////
    if (colorNumber == 0)   {
        setCurrentCharFormat(zeroSerialCharFormat);
    }else if (colorNumber == 1) {
        setCurrentCharFormat(firstSerialCharFormat);
    }else if (colorNumber == 2) {
        setCurrentCharFormat(secondSerialCharFormat);
    }else   qDebug() << "There is something wrong about the colorNumber";
    /////
    insertPlainText(QString(data));

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void SenConsole::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void SenConsole::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
        /****** Sen Start *******/
//        SenKeyPressBlockingControlTest(e->text().toLocal8Bit());
        /****** Sen End *******/
    }
}

void SenConsole::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void SenConsole::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void SenConsole::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}

/****** Sen Start *******/
void SenConsole::SenDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents,msec); // 100 milliseconds
}

void SenConsole::SenDeadDelay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(10*msec);//.addSecs(1);
    while( QTime::currentTime() < dieTime )
     qDebug() << "I am dead, please wait........................"; // 100 milliseconds
}

void SenConsole::SenKeyPressBlockingControlTest(const QByteArray &data)
{
//    qDebug() << "data[0] = " << data[0] << "**************************************************************";
    if (data[0] == '7') {
        lineDebugActive = true;
    }else if (data[0] == '8')   {
        lineDebugActive = false;
    }

    if (data[0] == '9')    {
        keyPressBlockingActive = false;
    }
    else    {
        keyPressBlockingActive = true;
    }
   ;
}

void SenConsole::SenKeyPressBlocking()
{
    while(keyPressBlockingActive){
        SenDelay(100);
    }
    keyPressBlockingActive = true;
}
/****** Sen End *******/
