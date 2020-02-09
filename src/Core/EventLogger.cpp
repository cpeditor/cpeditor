#include "Core/EventLogger.hpp"
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QSysInfo>
#include <QVector>
#include <generated/version.hpp>

namespace Core
{

// In .cpp file because they are used in implementations of EventLogger only

static const int NUMBER_OF_LOGS_TO_KEEP = 50;
static const QString LOG_FILE_NAME("cpeditor");
static const QString LOG_DIR_NAME("CPEditorLogFiles");

QFile Log::logFile;
QTextStream Log::logStream;

void Log::i(const QString &head, const QString &body)
{
    log("INFO", head, body);
}
void Log::w(const QString &head, const QString &body)
{
    log("WARN", head, body);
}
void Log::e(const QString &head, const QString &body)
{
    log("ERR", head, body);
}
void Log::wtf(const QString &head, const QString &body)
{
    log("WTF", head, body);
}

QTextStream &Log::i(const QString &head)
{
    return log("INFO", head);
}
QTextStream &Log::w(const QString &head)
{
    return log("WARN", head);
}
QTextStream &Log::e(const QString &head)
{
    return log("ERR", head);
}
QTextStream &Log::wtf(const QString &head)
{
    return log("WTF", head);
}

void Log::init(int instance, bool dumptoStderr)
{
    logStream.setDevice(&logFile);
    if (!dumptoStderr)
    {
        auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        if (path.isEmpty())
        {
            e("Logger", "Failed to get writable temp location");
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
                                                 QString::number(instance) + ".log"));
                logFile.open(QIODevice::WriteOnly | QFile::Text);
                if (!logFile.isOpen() || !logFile.isWritable())
                {
                    e("Logger", "Failed to open file " + logFile.fileName());
                }
            }
            else
            {
                e("Logger", "Failed to open directory " + dir.filePath(LOG_DIR_NAME));
            }
        }
    }
    else
    {
        logFile.open(stderr, QIODevice::WriteOnly);
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
    res.append("buildABI : " + QSysInfo::buildAbi() + ", ");
    res.append("buildCPUArchitecture : " + QSysInfo::buildCpuArchitecture() + ", ");
    res.append("currentCPUArchitecture : " + QSysInfo::currentCpuArchitecture() + ", ");
    res.append("kernelType : " + QSysInfo::kernelType() + ", ");
    res.append("kernelVersion : " + QSysInfo::kernelVersion() + ", ");
    res.append("prettyProductName : " + QSysInfo::prettyProductName() + ", ");
    res.append("machineHostName : " + QSysInfo::machineHostName() + ", ");
    res.append("machineUniqueId : " + QSysInfo::machineUniqueId() + ", ");
    res.append("productType : " + QSysInfo::productType() + ", ");
    res.append("productVersion : " + QSysInfo::productVersion() + ", ");
    res.append("Appversion : " APP_VERSION);
    return res;
}

void Log::log(const QString &priority, const QString &head, const QString &body)
{
    log(priority, head) << body << endl;
}

QTextStream &Log::log(const QString &priority, const QString &head)
{
    return logStream << dateTimeStamp() << "[" << priority << "][" << head << "]";
}

} // namespace Core