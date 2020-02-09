#include <QFile>
#include <QTextStream>

namespace Core
{

#define INFO_OF(x) #x ": " << x

class Log
{
  public:
    static void i(const QString &head, const QString &body);
    static void w(const QString &head, const QString &body);
    static void e(const QString &head, const QString &body);
    static void wtf(const QString &head, const QString &body);
    static QTextStream &i(const QString &head);
    static QTextStream &w(const QString &head);
    static QTextStream &e(const QString &head);
    static QTextStream &wtf(const QString &head);

    static void init(int instanceCount, bool dumpToStderr = false);

  private:
    static QString dateTimeStamp();
    static QString platformInformation();
    static void log(const QString &priority, const QString &head, const QString &body);
    static QTextStream &log(const QString &priority, const QString &head);

    static QTextStream logStream;
    static QFile logFile;
};

} // namespace Core