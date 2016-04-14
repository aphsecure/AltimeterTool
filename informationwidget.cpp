#include "informationwidget.h"
#include "ui_informationwidget.h"

#include <QTextEdit>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>

QTextEdit* InformationWidget::m_log = 0;

InformationWidget::InformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWidget),
    m_autoSendTimer(new QTimer(this))
{
    ui->setupUi(this);
    m_log = new QTextEdit;
    ui->logBox->layout()->addWidget(m_log);
    m_log->setReadOnly(true);
    createConnections();
}

InformationWidget::~InformationWidget()
{
    delete ui;
}

void InformationWidget::setAltitude(int val)
{
    ui->altimetrLCDValue->display(val);
}

void InformationWidget::setDeviceType(int val)
{
    ui->devTypeLable->setText(QString("%1").arg(val, 0, 16).toUpper());
}

void InformationWidget::createConnections()
{
    connect(ui->requestDataButton, &QPushButton::clicked, [this]() {
        emit requestData(ui->addressBox->value());
    });

    connect(ui->requestTypeButton, &QPushButton::clicked, [this]() {
        emit requestType(ui->addressBox->value());
    });

    connect(ui->setAddressButton, &QPushButton::clicked, [this]() {
        emit setupAddress(ui->currentAddressBox->value() ,ui->newAddressBox->value());
    });

    connect(m_autoSendTimer, &QTimer::timeout, [this]() {
        emit requestData(ui->addressBox->value());
    });

    const auto saveToFile = [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save file", "", ".txt");

        if (fileName.isEmpty()) {
            qInfo() << "INFO: saveToFile -> cancelled";
            return;
        }

        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << m_log->toPlainText();
        }
        file.close();

    };

    connect(ui->dumpToFileButton, &QPushButton::clicked, saveToFile);

    connect(ui->autoSendBox, &QCheckBox::clicked, [this](bool opt) {
        if (opt) {
            ui->requestDataButton->setDisabled(true);
            ui->setAddressButton->setDisabled(true);
            ui->requestTypeButton->setDisabled(true);
            m_autoSendTimer->start(80);
        } else {
            ui->requestDataButton->setDisabled(false);
            ui->setAddressButton->setDisabled(false);
            ui->requestTypeButton->setDisabled(false);
            m_autoSendTimer->stop();
        }
    });
}
