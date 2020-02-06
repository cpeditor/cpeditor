#include <QFile>

#define LOG_FILE_NAME "cpeditor.log"

namespace Core
{
class Log
{
  public:
    static void e(QString head, QString body);
    static void w(QString head, QString body);
    static void wtf(QString head, QString body);
    static void i(QString head, QString body);

  private:
    static void init();
    static QString dateTimeStamp();
    static QString platformInformation();
    static void logWithPriority(QString, QString, QString);

    static QFile logFile;
};
} // namespace Core