/*
 * CSES CLI integration similar to CF Tool
 */
#ifndef CSESTOOL_HPP
#define CSESTOOL_HPP

#include <QObject>
#include <QProcess>
#include <QString>

class MessageLogger;

namespace Extensions
{
class CSESTool : public QObject
{
    Q_OBJECT

  public:
    CSESTool(const QString &path, MessageLogger *logger);
    ~CSESTool() override;
    void submit(const QString &filePath, const QString &contest, const QString &taskId);
    static bool check(const QString &path);
    void updatePath(const QString &p);
    static bool parseCsesUrl(const QString &url, QString &contest, QString &taskId);

  signals:
    void requestToastMessage(const QString &head, const QString &body);

  private slots:
    void onReadReady();
    void onFinished(int exitCode, QProcess::ExitStatus);

  private:
    QString lastStatus;
    QProcess *process = nullptr;
    MessageLogger *log;
    QString path;

    void showToastMessage(const QString &message);
    QString getVersion() const;
};
} // namespace Extensions

#endif // CSESTOOL_HPP
