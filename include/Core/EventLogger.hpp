#include <QFile>
#include <QTextStream>
#include <iomanip>
#include <sstream>

#define LOG_FILE_NAME "cpeditor.log"

namespace Core
{

template <class T> QString Stringify(T val)
{
    std::ostringstream out;
    out << std::boolalpha << std::fixed << std::setprecision(10) << val;
    return QString::fromStdString(out.str());
}

class Log
{
  public:
    static void e(QString head, QString body);
    static void w(QString head, QString body);
    static void wtf(QString head, QString body);
    static void i(QString head, QString body);

    static void init(bool dumpToStderr = false);

  private:
    static QString dateTimeStamp();
    static QString platformInformation();
    static void logWithPriority(QString, QString, QString);

    static QFile logFile;
    static bool shouldWriteToStderr;
};
} // namespace Core