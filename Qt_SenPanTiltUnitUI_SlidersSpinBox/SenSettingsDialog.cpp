#include "SenSettingsDialog.h"
#include "ui_SenSettingsDialog.h"


static const char blankString[] = QT_TRANSLATE_NOOP("SenSettingsDialog", "N/A");

SenSettingsDialog::SenSettingsDialog(QWidget *parent) :
    QDialog(parent),
    mySettingsDialog_ui(new Ui::SenSettingsDialog)
{
    mySettingsDialog_ui->setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);

    mySettingsDialog_ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(mySettingsDialog_ui->applyButton, SIGNAL(clicked()),
            this, SLOT(apply()));
    connect(mySettingsDialog_ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfo(int)));
    connect(mySettingsDialog_ui->baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));
    connect(mySettingsDialog_ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomDevicePathPolicy(int)));

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();
}

SenSettingsDialog::~SenSettingsDialog()
{
    delete mySettingsDialog_ui;
}

SenSettingsDialog::Settings SenSettingsDialog::settings() const
{
    return currentSettings;
}

void SenSettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = mySettingsDialog_ui->serialPortInfoListBox->itemData(idx).toStringList();
    mySettingsDialog_ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    mySettingsDialog_ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    mySettingsDialog_ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    mySettingsDialog_ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    mySettingsDialog_ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    mySettingsDialog_ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SenSettingsDialog::apply()
{
    updateSettings();
    hide();
}

void SenSettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !mySettingsDialog_ui->baudRateBox->itemData(idx).isValid();
    mySettingsDialog_ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        mySettingsDialog_ui->baudRateBox->clearEditText();
        QLineEdit *edit = mySettingsDialog_ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SenSettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !mySettingsDialog_ui->serialPortInfoListBox->itemData(idx).isValid();
    mySettingsDialog_ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
        mySettingsDialog_ui->serialPortInfoListBox->clearEditText();
}

void SenSettingsDialog::fillPortsParameters()
{
    mySettingsDialog_ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    mySettingsDialog_ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    mySettingsDialog_ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    mySettingsDialog_ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    mySettingsDialog_ui->baudRateBox->addItem(tr("Custom"));

    mySettingsDialog_ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    mySettingsDialog_ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    mySettingsDialog_ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    mySettingsDialog_ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    mySettingsDialog_ui->dataBitsBox->setCurrentIndex(3);

    mySettingsDialog_ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    mySettingsDialog_ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    mySettingsDialog_ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    mySettingsDialog_ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    mySettingsDialog_ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    mySettingsDialog_ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    mySettingsDialog_ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    mySettingsDialog_ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    mySettingsDialog_ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    mySettingsDialog_ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    mySettingsDialog_ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SenSettingsDialog::fillPortsInfo()
{
    mySettingsDialog_ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        mySettingsDialog_ui->serialPortInfoListBox->addItem(list.first(), list);
    }
}

void SenSettingsDialog::updateSettings()
{
    currentSettings.name = mySettingsDialog_ui->serialPortInfoListBox->currentText();

    if (mySettingsDialog_ui->baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = mySettingsDialog_ui->baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    mySettingsDialog_ui->baudRateBox->itemData(mySettingsDialog_ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                mySettingsDialog_ui->dataBitsBox->itemData(mySettingsDialog_ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = mySettingsDialog_ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                mySettingsDialog_ui->parityBox->itemData(mySettingsDialog_ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = mySettingsDialog_ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                mySettingsDialog_ui->stopBitsBox->itemData(mySettingsDialog_ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = mySettingsDialog_ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                mySettingsDialog_ui->flowControlBox->itemData(mySettingsDialog_ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = mySettingsDialog_ui->flowControlBox->currentText();
}
