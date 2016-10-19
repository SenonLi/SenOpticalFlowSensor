#include "SenSerialSettingDialog.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SenSerialSettingWidget", "N/A");

SenSerialSettingWidget::SenSerialSettingWidget(QWidget *parent)
    :customerControlActive(false)
{
    qRegisterMetaType<SerialInfoGroup>();// For SerialSettingsInfo Structure store

    intValidator = new QIntValidator(0, 4000000, this);

    // Layout:
    serialPortsSelectGroupBox = new QGroupBox(QString("Select Serial Port"));
    QVBoxLayout *serialPortVBoxLayout = new QVBoxLayout;
    serialPortInfoListBox = new QComboBox;
    descriptionLabel = new QLabel(QString("Description:  "));
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    manufacturerLabel = new QLabel(QString("Description:  "));
    manufacturerLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    serialPortVBoxLayout->addSpacing(1);
    serialPortVBoxLayout->addWidget(serialPortInfoListBox);
    serialPortVBoxLayout->addWidget(descriptionLabel);
    serialPortVBoxLayout->addWidget(manufacturerLabel);
    serialPortVBoxLayout->addStretch();
    serialPortsSelectGroupBox->setLayout(serialPortVBoxLayout);
    serialPortsSelectGroupBox->setMinimumWidth(200);
    serialPortsSelectGroupBox->layout()->setContentsMargins(5,5,5,5);

    parametersSelectGroupBox = new QGroupBox(QString("Select Parameters"));
    QHBoxLayout *parametersHBoxLayout = new QHBoxLayout;
    parametersSelectGroupBox->setLayout(parametersHBoxLayout);
    parametersSelectGroupBox->setMinimumWidth(200);
    parametersSelectGroupBox->layout()->setContentsMargins(5,5,5,5);
    baudRateLabel = new QLabel(QString("BaudRate:  "));
    baudRateBox = new QComboBox;
    baudRateBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    parametersSelectGroupBox->layout()->addWidget(baudRateLabel);
    parametersSelectGroupBox->layout()->addWidget(baudRateBox);

    QHBoxLayout *infoSelectLayout = new QHBoxLayout;
    infoSelectLayout->setContentsMargins(0,0,0,0);
    infoSelectLayout->addWidget(serialPortsSelectGroupBox);
    infoSelectLayout->addSpacing(2);
    infoSelectLayout->addWidget(parametersSelectGroupBox);

    this->setLayout(infoSelectLayout);
    this->layout()->setContentsMargins(2,2,2,2);
    this->setMinimumHeight(100);


    // Serial Dialog Info Link
    baudRateBox->setInsertPolicy(QComboBox::NoInsert);

//    connect(mySettingsDialog_ui->applyButton, SIGNAL(clicked()),
//            this, SLOT(apply()));
    connect(serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfo(int)));
    connect(baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));
    connect(serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomDevicePathPolicy(int)));

    fillPortsParameters();
    fillPortsInfo();

    if(serialPortInfoListBox->count() > 0)
    {
        serialPortInfoListBox->setCurrentIndex(0);
        showPortInfo(0);
    }

    updateSettings();
}

/****************************************************************************/
SenSerialSettingWidget::~SenSerialSettingWidget()
{
}

void SenSerialSettingWidget::fillPortsInfo()
{
    serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString);
//        qDebug() << "info.portName() = " << info.portName();
        if (info.portName() == QString("COM4"))   {
            serialPortInfoListBox->insertItem(0, list.first(), list);
        }else   serialPortInfoListBox->addItem(list.first(), list);
    }
}

void SenSerialSettingWidget::fillPortsParameters()
{
    baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateBox->addItem(QStringLiteral("43200"), QSerialPort::BaudRate(43200));
    baudRateBox->addItem(tr("Custom"));
}

void SenSerialSettingWidget::updateSettings()
{
    currentSettings.name = serialPortInfoListBox->currentText();

    if (baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
        baudRateBox->itemData(baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

}

void SenSerialSettingWidget::apply()
{
    customerControlActive = true;
    updateSettings();
}

void SenSerialSettingWidget::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = serialPortInfoListBox->itemData(idx).toStringList();
    descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
}

void SenSerialSettingWidget::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !baudRateBox->itemData(idx).isValid();
    baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        baudRateBox->clearEditText();
        QLineEdit *edit = baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SenSerialSettingWidget::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !serialPortInfoListBox->itemData(idx).isValid();
    serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
        serialPortInfoListBox->clearEditText();
}






SenSerialSettingDialog::SenSerialSettingDialog(QDialog *parent):QDialog(parent)
{
    this->setWindowTitle(QString("Serial for BLE dongle:"));

    widget = new SenSerialSettingWidget();
    QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    QVBoxLayout *finalLayout = new QVBoxLayout;
    finalLayout->addWidget(widget);
    finalLayout->addSpacing(1);
    finalLayout->addWidget(dialogButtonBox);

    this->setLayout(finalLayout);
    this->layout()->setContentsMargins(0,0,0,0);
//    this->resize(QSize(800,600));

    connect(dialogButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(apply()));
    connect(dialogButtonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

SerialInfoGroup SenSerialSettingDialog::settings() const
{
    return widget->getSerialSettings();
}

