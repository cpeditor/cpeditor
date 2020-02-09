#include "Core/EventLogger.hpp"
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QSysInfo>
#include <QVector>
#include <generated/version.hpp>
#include <iostream>

namespace Core
{

// In .cpp file because they are used in implementations of EventLogger only

static const int NUMBER_OF_LOGS_TO_KEEP = 50;
static const QString LOG_FILE_NAME("cpeditor");
static const QString LOG_DIR_NAME("CPEditorLogFiles");

QFile Log::logFile;
bool Log::shouldWriteToStderr;

void Log::e(QString head, QString body)
{
    logWithPriority("[ERR]", head, body);
}
void Log::w(QString head, QString body)
{
    logWithPriority("[WARN]", head, body);
}
void Log::wtf(QString head, QString body)
{
    logWithPriority("[WTF]", head, body);
}
void Log::i(QString head, QString body)
{
    logWithPriority("[INFO]", head, body);
}

void Log::init(int instance, bool dumptoStderr)
{
    shouldWriteToStderr = dumptoStderr;
    if (!dumptoStderr)
    {
        auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        if (path.isEmpty())
        {
            e("Logger", "Failed to get writable temp location");
            shouldWriteToStderr = true;
        }
        else
        {
            QDir dir(path);
            dir.mkdir(LOG_DIR_NAME);
            if (dir.cd(LOG_DIR_NAME))
            {
                auto entries = dir.entryList({LOG_FILE_NAME + "*.log"}, QDir::Files, QDir::Time);
                for (int i = NUMBER_OF_LOGS_TO_KEEP; i < entries.length(); ++i)
                    dir.remove(entries[i]);
                logFile.setFileName(dir.filePath(LOG_FILE_NAME +
                                                 QDateTime::currentDateTime().toString("-yyyy-MM-dd-hh-mm-ss-zzz-") +
                                                 Core::Stringify(instance) + ".log"));
                logFile.open(QIODevice::WriteOnly | QFile::Text);
                if (!logFile.isOpen() || !logFile.isWritable())
                {
                    e("Logger", "Failed to open file " + logFile.fileName());
                    shouldWriteToStderr = true;
                }
            }
            else
            {
                e("Logger", "Failed to open directory " + dir.filePath(LOG_DIR_NAME));
                shouldWriteToStderr = true;
            }
        }
        if (shouldWriteToStderr)
            e("Logger", "Failed to write log in the file");
    }
    i("Logger", "Event logger has started");
    i("SysInfo", "Gathering system information");
    i("SysInfo", platformInformation());
}

QString Log::dateTimeStamp()
{
    return "[" + QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + "]";
}

QString Log::platformInformation()
{
    QString res;
    // Check https://doc.qt.io/qt-5/qsysinfo.html to know what each identifier means
    res.append("\nbuildABI : " + QSysInfo::buildAbi() + "\n");
    res.append("buildCPUArchitecture : " + QSysInfo::buildCpuArchitecture() + "\n");
    res.append("currentCPUArchitecture : " + QSysInfo::currentCpuArchitecture() + "\n");
    res.append("kernelType : " + QSysInfo::kernelType() + "\n");
    res.append("kernelVersion : " + QSysInfo::kernelVersion() + "\n");
    res.append("prettyProductName : " + QSysInfo::prettyProductName() + "\n");
    res.append("machineHostName : " + QSysInfo::machineHostName() + "\n");
    res.append("machineUniqueId : " + QSysInfo::machineUniqueId() + "\n");
    res.append("productType : " + QSysInfo::productType() + "\n");
    res.append("productVersion : " + QSysInfo::productVersion() + "\n");
    res.append("Appversion : " + QString::fromStdString(APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_VERSION_PATCH));
    return res;
}

void Log::logWithPriority(QString priority, QString const &head, QString const &body)
{
    QString logContent;
    logContent.append(dateTimeStamp());
    logContent.append(priority);
    logContent.append("[" + head + "]");
    logContent.append(body);
    logContent.append("\n");
    if (logFile.isWritable())
    {
        logFile.write(logContent.toLocal8Bit());
        logFile.flush();
    }
    else
    {
        shouldWriteToStderr = true;
    }
    if (shouldWriteToStderr)
    {
        QTextStream cerr(stderr, QIODevice::WriteOnly);
        cerr << logContent;
    }
}

} // namespace Core