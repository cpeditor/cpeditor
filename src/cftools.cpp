#include "cftools.hpp"
#include <QDebug>
namespace Network
{

CFTools::CFTools(int index) : Core::Base::Files(index)
{
}

void CFTools::killProcess()
{
}

void CFTools::submit(QString url, QString language)
{
    QString filePath;
    if (language == "Cpp")
    {
        filePath = getProgramFile();
    }
    else if (language == "Java")
    {
        filePath = getProgramFile(".java");
    }
    else if (language == "Python")
    {
        filePath = getProgramFile(".py");
    }
    else
    {
        return;
    }
    // Todo : Invoke cf-tools here.
}

bool CFTools::check()
{
    QProcess program;
    QString commandToStart = "cf";
    QStringList environment = program.systemEnvironment();
    program.start(commandToStart);
    bool started = program.waitForStarted();
    if (started) // 10 Second timeout
        program.kill();
    return started;
}

} // namespace Network
