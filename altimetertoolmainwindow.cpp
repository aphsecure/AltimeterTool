#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QFile>
#include <QSerialPort>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

#include "altimetertoolmainwindow.h"
#include "altimeterhandler.h"
#include "settingsdialog.h"
#include "informationwidget.h"
#include "murmagics.h"

AltimeterToolMainWindow::AltimeterToolMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_settingsDialog(new SettingsDialog(this)),
      m_informationWidget(new InformationWidget(this)),
      m_serial(new QSerialPort(this))
{
    createActions();
    createMenus();
    createConnections();

    setCentralWidget(m_informationWidget);

    QFile mainWindowSheet("://style/style.qss");
    mainWindowSheet.open(QFile::ReadOnly);
    QString styleSheet = mainWindowSheet.readAll();
    setStyleSheet(styleSheet);
    mainWindowSheet.close();

    setContextMenuPolicy(Qt::NoContextMenu);
    m_informationWidget->setDisabled(true);
}

AltimeterToolMainWindow::~AltimeterToolMainWindow()
{

}

void AltimeterToolMainWindow::createActions()
{
    m_connectAct = new QAction(QIcon(":/icons/connect.png"), tr("Connect (CTRL + A)"), this);
    m_connectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));

    m_disconnectAct = new QAction(QIcon(":/icons/disconnect.png"), tr("Disconnect (CTRL + D)"), this);
    m_disconnectAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    m_disconnectAct->setDisabled(true);

    m_settingsAct = new QAction(QIcon(":/icons/settings.png"), tr("Properties (CTRL + P)"), this);
    m_settingsAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
}

void AltimeterToolMainWindow::createMenus()
{
    QToolBar* toolBar = addToolBar("ToolBar");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addAction(m_connectAct);
    toolBar->addAction(m_disconnectAct);
    toolBar->addAction(m_settingsAct);
    toolBar->setMovable(false);

    m_statusLable = new QLabel(tr("No connection"), this);
    QStatusBar* m_statusBar = statusBar();
    m_statusBar->addWidget(m_statusLable);

}

void AltimeterToolMainWindow::createConnections()
{
    connect(m_settingsAct, SIGNAL(triggered(bool)), m_settingsDialog, SLOT(exec()));

    const auto onConnect = [this]() {
        m_disconnectAct->setEnabled(true);

        SettingsDialog::Settings p = m_settingsDialog->settings();
        m_serial->setPortName(p.name);
        m_serial->setBaudRate(p.baudRate);
        m_serial->setDataBits(p.dataBits);
        m_serial->setParity(p.parity);
        m_serial->setStopBits(p.stopBits);
        m_serial->setFlowControl(p.flowControl);
        if (m_serial->open(QIODevice::ReadWrite)) {
            m_connectAct->setDisabled(true);
            m_informationWidget->setEnabled(true);
            m_disconnectAct->setEnabled(true);
            m_settingsAct->setDisabled(true);
            showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        } else {
            QMessageBox::critical(this, tr("Error"), m_serial->errorString());

            showStatusMessage(tr("Open error"));
        }
    };

    const auto onDisconnect = [this]() {
        m_serial->close();
        m_informationWidget->setDisabled(true);
        m_connectAct->setEnabled(true);
        m_disconnectAct->setDisabled(true);
        m_settingsAct->setEnabled(true);
        m_serialBuffer.clear();
        showStatusMessage(tr("No connection"));
    };

    const auto onRequestData = [this](int address) {
        QByteArray data;
        AltimeterHandler::prepareRequestData(data, address);
        onSerialWrite(data);
    };

    const auto onRequestType = [this](int address) {
        QByteArray data;
        AltimeterHandler::prepareRequestType(data, address);
        onSerialWrite(data);
    };

    const auto onSetupAddress = [this](int addressOld, int addressNew) {
        QByteArray data;
        AltimeterHandler::setNewAddress(data, addressNew, addressOld);
        onSerialWrite(data);
    };

    connect(m_informationWidget, &InformationWidget::requestData, onRequestData);
    connect(m_informationWidget, &InformationWidget::setupAddress, onSetupAddress);
    connect(m_informationWidget, &InformationWidget::requestType, onRequestType);

    connect(m_connectAct, &QAction::triggered, onConnect);
    connect(m_disconnectAct, &QAction::triggered, onDisconnect);

    connect(m_serial, SIGNAL(readyRead()), this, SLOT(onSerialRecive()));
}

void AltimeterToolMainWindow::onSerialRecive()
{
    if (m_serial->bytesAvailable() < ::service_info_size
            && m_serialBuffer.size() < ::service_info_size) {
        return;
    }

    m_serialBuffer.append(m_serial->readAll());
    qInfo() << "INFO: Data received:" << m_serialBuffer.toHex().toUpper();

    //!Cleanup trash.
    while (static_cast<uint8_t>(m_serialBuffer.at(::first_package_header_position)) != ::first_package_header_value) {

        qWarning() << "WARNING: AltimeterToolMainWindow::onSerialRecive -> Weird data recievd. Cleanup.";
        qDebug() << "BUFFER:" << m_serialBuffer.toHex().toUpper();
        if (m_serialBuffer.size() < 2) {
            return;
        }
        m_serialBuffer.remove(0, 1);
    }

    if (m_serialBuffer.size() < static_cast<uint8_t>(m_serialBuffer.at(::package_size_position))) {
        return;
    }

    uint8_t id = m_serialBuffer.at(::message_id_position);

    unsigned int altitude = 0;
    unsigned int from = 0;
    unsigned int address = 0;
    unsigned int type;

    switch (id) {
    case ::device_type_message :
        if (AltimeterHandler::processReplyType(m_serialBuffer, type, address)) {
            m_informationWidget->setDeviceType(type);
            m_serialBuffer.remove(0, m_serialBuffer.at(::package_size_position));
        }
        else {
            m_serialBuffer.clear();
        }
        return;
    case ::altimeter_status :
        if (AltimeterHandler::processReplyData(m_serialBuffer, altitude, from)) {
            m_informationWidget->setAltitude(altitude);
            m_serialBuffer.remove(0, m_serialBuffer.at(::package_size_position));
        }
        else {
            m_serialBuffer.clear();
        }
        return;
    default:
        qWarning() << "WARNING: AltimeterToolMainWindow::onSerialRecive -> Weird data recievd: UNKNOWN message ID.";
        qInfo() << "INFO: Data received:" << m_serialBuffer.toHex().toUpper();
        m_serialBuffer.remove(0, m_serialBuffer.at(::package_size_position));
        return;
    }
    m_serialBuffer.clear();
}

void AltimeterToolMainWindow::onSerialWrite(QByteArray data)
{
    int64_t writeDataSize = 0L;
    if (Q_LIKELY(m_serial->isOpen())) {
        writeDataSize = m_serial->write(data);
        qInfo() << "INFO: Sending data:" << data.toHex().toUpper();
    } else {
        qInfo() << "INFO: Serial CLOSED.";
        return;
    }
    if (Q_UNLIKELY(writeDataSize != data.size())) {
        qCritical() << "CRITICAL: AltimeterToolMainWindow::onSerialWrite -> Data sending FAILURE";
    }
}

void AltimeterToolMainWindow::showStatusMessage(QString message)
{
    m_statusLable->setText(message);
}
