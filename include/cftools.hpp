#ifndef CFTOOLS_HPP
#define CFTOOLS_HPP

#include <QProcess>
#include <BaseFiles.hpp>
#include <MessageLogger.hpp>
namespace Network
{
class CFTools : public QObject, private Core::Base::Files
{
    Q_OBJECT

  public:
    CFTools(int index, MessageLogger* logger);

    void submit(QString url, QString lang);
    void killProcess();

    static bool check();
    ~CFTools();

private slots:
    void onReadReady();

  private:
    QProcess *cftool = nullptr;
    MessageLogger* log;
    int index;
};
} // namespace Network
#endif // CFTOOLS_HPP
