#ifndef COMPANIONSERVER_HPP
#define COMPANIONSERVER_HPP
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "MessageLogger.hpp"

namespace Network
{

struct CompanionData
{
    struct TestCases
    {
        TestCases() = default;
        TestCases(QString in, QString out) : input(in), output(out)
        {
        }
        QString input;
        QString output;
    };

    QString name;
    QString contest;
    QString url;
    bool interactive;
    double memoryLimit;
    double timeLimit;
    bool isInputstdin;
    bool isOutputstdout;
    QVector<TestCases> testcases;

    QString toMetaString()
    {
        QString ans;
        ans += "Problem : " + name + "\n";
        ans += "Contest : " + contest + "\n";
        ans += "URL : " + url + "\n";
        ans += "Memory Limit : " + QString::fromStdString(std::to_string(memoryLimit)) + " MB \n";
        ans += "Time Limit : " + QString::fromStdString(std::to_string(timeLimit)) + " milisec \n";
        return ans;
    }
};

class CompanionServer : public QObject
{
    Q_OBJECT

  public:
    CompanionServer(int port);
    void setMessageLogger(MessageLogger* log);
    ~CompanionServer();

  signals:
    void onRequestArrived(CompanionData data);

  private slots:
    void onNewConnection();
    void onTerminateConnection();
    void onReadReady();

  private:
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
    int portNumber;
    MessageLogger* log;
};
} // namespace Network
#endif // COMPANIONSERVER_HPP
