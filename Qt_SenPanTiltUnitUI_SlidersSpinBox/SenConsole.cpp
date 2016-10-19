
#include "SenConsole.h"

SenConsole::SenConsole(QWidget *parent)
    : QPlainTextEdit(parent)
    , localEchoEnabled(false), keyPressBlockingActive(false), lineDebugActive(false)
{
//    document()->setMaximumBlockCount(100);
//    QPalette p = palette();
//    p.setColor(QPalette::Base, Qt::black);
//    p.setColor(QPalette::Text, Qt::green);
//    setPalette(p);

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
    setCurrentCharFormat(firstSerialCharFormat);

}

void SenConsole::putData(const QByteArray &data)
{
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
        break;
    case Qt::Key_Left:      emit emitStepLeft();
        break;
    case Qt::Key_Right:     emit emitStepRight();
        break;
    case Qt::Key_Up:    emit emitStepUp();
        break;
    case Qt::Key_Down:      emit emitStepDown();
        break;
    case Qt::Key_G:      emit emitStep10Left();
        break;
    case Qt::Key_J:     emit emitStep10Right();
        break;
    case Qt::Key_Y:    emit emitStep10Up();
        break;
    case Qt::Key_H:      emit emitStep10Down();
        break;
    default:
        if (localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
        /****** Sen Start *******/
//        if (e->key() == Qt::)
        SenKeyPressBlockingControlTest(e->text().toLocal8Bit());
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
//    qDebug() << "data.length() = " << data.length() << "**************************************************************";
    if (data.length() != 0)   {
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


SenSlidersConsole::SenSlidersConsole(const QString &title,
                           QWidget *parent)
    : QGroupBox(title, parent)
{
//    slider = new QSlider(orientation);
//    slider->setFocusPolicy(Qt::StrongFocus);
//    slider->setTickPosition(QSlider::TicksBothSides);
//    slider->setTickInterval(10);
//    slider->setSingleStep(1);

//    scrollBar = new QScrollBar(orientation);
//    scrollBar->setFocusPolicy(Qt::StrongFocus);

//    dial = new QDial;
//    dial->setFocusPolicy(Qt::StrongFocus);

//    connect(slider, SIGNAL(valueChanged(int)), scrollBar, SLOT(setValue(int)));
//    connect(scrollBar, SIGNAL(valueChanged(int)), dial, SLOT(setValue(int)));
//    connect(dial, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
////! [0] //! [1]
//    connect(dial, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
////! [1] //! [2]

////! [2] //! [3]
//    QBoxLayout::Direction direction;
////! [3] //! [4]

//    if (orientation == Qt::Horizontal)
//        direction = QBoxLayout::TopToBottom;
//    else
//        direction = QBoxLayout::LeftToRight;

//    QBoxLayout *slidersLayout = new QBoxLayout(direction);
//    slidersLayout->addWidget(slider);
//    slidersLayout->addWidget(scrollBar);
//    slidersLayout->addWidget(dial);
//    setLayout(slidersLayout);
}
