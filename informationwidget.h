#ifndef INFORMATIONWIDGET_H
#define INFORMATIONWIDGET_H

#include <QWidget>

class QTextEdit;
class QTimer;

namespace Ui {
class InformationWidget;
}

class InformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWidget(QWidget *parent = 0);
    ~InformationWidget();
    static QTextEdit* m_log;

    void setAltitude(int val);
    void setDeviceType(int val);
signals:
    void requestType(int address);
    void requestData(int address);
    void setupAddress(int oldAddress, int newAddress);
private slots:
    void createConnections();
private:
    Ui::InformationWidget *ui;
    QTimer* m_autoSendTimer;

};

#endif // INFORMATIONWIDGET_H
