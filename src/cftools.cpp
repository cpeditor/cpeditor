#include "cftools.hpp"

namespace Network
{

CFTools::CFTools()
{
}

void CFTools::killProcess()
{
}

void CFTools::submit()
{
}

bool CFTools::check()
{
    QProcess program;
    QString commandToStart = "cf";
    QStringList environment = program.systemEnvironment();
    program.start(commandToStart + " --version");
    bool started = program.waitForStarted();
    if (started) // 10 Second timeout
        program.kill();
    return started;
}

} // namespace Network
