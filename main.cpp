#include "altimetertoolmainwindow.h"
#include "informationwidget.h"

#include <QApplication>
#include <QDateTime>
#include <QTextEdit>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString infoHtml = "<font color=\"Lime\">";
    QString alertHtml = "<font color=\"DeepPink\">";
    QString criticalHtml = "<font color=\"Red\">";
    QString debugHtml = "<font color=\"Aqua\">";
    QString endHtml = "</font>";
    QString infoMsg = msg;
    QString time = QDateTime::currentDateTime().toString("hh.mm.ss.z ");
    if(InformationWidget::m_log == 0)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtInfoMsg:
            fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            abort();
        }
    }
    else
    {
        switch (type) {
        case QtInfoMsg:
            if(InformationWidget::m_log != 0) {
                infoMsg = infoHtml + infoMsg;
                infoMsg = infoMsg + endHtml;
                time += infoMsg;
                InformationWidget::m_log->append(time.simplified().trimmed());
            }
            break;
        case QtDebugMsg:
            if(InformationWidget::m_log != 0) {
                infoMsg = debugHtml + infoMsg;
                infoMsg = infoMsg + endHtml;
                time += infoMsg;
                InformationWidget::m_log->append(infoMsg.trimmed());
            }
            break;
        case QtWarningMsg:
            if(InformationWidget::m_log != 0) {
                infoMsg = alertHtml + infoMsg;
                infoMsg = infoMsg + endHtml;
                time += infoMsg;
                InformationWidget::m_log->append(infoMsg.simplified().trimmed());
            }
            break;
        case QtCriticalMsg:
            if(InformationWidget::m_log != 0) {
                InformationWidget::m_log->append(QDateTime::currentDateTime().toString("HH:MM:SS"));
                infoMsg = criticalHtml + infoMsg;
                infoMsg = infoMsg + endHtml;
                time += infoMsg;
                InformationWidget::m_log->append(infoMsg.simplified().trimmed());
            }
            break;
        case QtFatalMsg:
            abort();
        default:
            break;
        }
    }
}




int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    AltimeterToolMainWindow w;
    w.show();

    return a.exec();
}
