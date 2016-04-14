#ifndef ALTIMETERTOOLMAINWINDOW_H
#define ALTIMETERTOOLMAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QSerialPort;
class QAction;
class QStatusBar;
class SettingsDialog;
class InformationWidget;
class QLabel;

class AltimeterToolMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AltimeterToolMainWindow(QWidget *parent = 0);
    ~AltimeterToolMainWindow();
private slots:
    void createActions();
    void createMenus();
    void createConnections();

    void onSerialRecive();
    void onSerialWrite(QByteArray data);
    void showStatusMessage(QString message);
private:
    QToolBar* m_toolBar;
    QStatusBar* m_statusBar;
    QLabel* m_statusLable;

    QAction* m_connectAct;
    QAction* m_disconnectAct;
    QAction* m_settingsAct;
    SettingsDialog* m_settingsDialog;
    InformationWidget* m_informationWidget;
    QSerialPort* m_serial;

    QByteArray m_serialBuffer;
};

#endif // ALTIMETERTOOLMAINWINDOW_H
