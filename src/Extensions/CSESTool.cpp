/*
 * CSES CLI integration implementation
 */
#include "Extensions/CSESTool.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include <QString>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>

namespace Extensions
{

CSESTool::CSESTool(const QString &p, MessageLogger *logger) : log(logger), path(p)
{
    LOG_INFO(INFO_OF(p))
}

CSESTool::~CSESTool()
{
    delete process;
}

void CSESTool::submit(const QString &filePath, const QString &contest, const QString &taskId)
{
    if (process != nullptr)
    {
        if (process->state() == QProcess::Running)
        {
            LOG_WARN("CSES CLI was already running, killing it now");
            process->kill();
            delete process;
            if (log)
                log->error(tr("CSES CLI"), tr("CSES CLI was killed"));
        }
        else
            delete process;
        process = nullptr;
    }

    LOG_INFO(INFO_OF(filePath) << INFO_OF(contest) << INFO_OF(taskId));

    if (contest.isEmpty() || taskId.isEmpty())
    {
        if (log)
            log->error(tr("CSES CLI"), tr("Empty contest or task id"));
        return;
    }

    process = new QProcess();
    process->setProgram(path);

    auto version = getVersion();
    if (version.isEmpty())
    {
        if (log)
            log->error(tr("CSES CLI"), tr("Failed to get the version of CSES CLI. Have you set the correct path in Preferences?"));
        return;
    }

    process->setArguments({"submit", "-c", contest, "-t", taskId, filePath});
    LOG_INFO(INFO_OF(process->arguments().join(' ')));

    connect(process, &QProcess::readyReadStandardOutput, this, &CSESTool::onReadReady);
    connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &CSESTool::onFinished);
    process->start();
    bool started = process->waitForStarted(2000);
    if (started)
    {
        if (log)
            log->info(tr("CSES CLI"), tr("CSES CLI has started"));
    }
    else
    {
        process->kill();
        if (log)
            log->error(tr("CSES CLI"), tr("Failed to start CSES CLI in 2 seconds. Have you set the correct path in Preferences?"));
    }
}

bool CSESTool::check(const QString &p)
{
    LOG_INFO(INFO_OF(p));
    if (p.isEmpty()) 
    {
        LOG_WARN("CSES CLI path is empty");
        return false;
    }
    
    QProcess check;
    // CSES CLI shows version info when run without arguments, not with --version
    check.start(p, {});
    bool finished = check.waitForFinished(2000);
    LOG_INFO(BOOL_INFO_OF(finished) << INFO_OF(check.exitCode()) << INFO_OF(check.exitStatus()) 
             << INFO_OF(check.error()) << INFO_OF(check.errorString()));
    
    if (!finished)
    {
        LOG_WARN("CSES CLI check timed out or failed to start");
        return false;
    }
    
    // CSES CLI exits with 0 when showing help/version info
    return check.exitCode() == 0;
}

void CSESTool::updatePath(const QString &p)
{
    LOG_INFO(INFO_OF(p));
    path = p;
}

    bool CSESTool::parseCsesUrl(const QString &url, QString &contest, QString &taskId)
{
    LOG_INFO(INFO_OF(url));

    // Pattern 1: Matches standard and contest tasks (e.g., cses.fi/647/task/A/ or cses.fi/problemset/task/1643)
    // Changed ([0-9]+) to ([^/]+) to allow alphabetical task IDs, and added /? at the end
    auto match = QRegularExpression(R"(.*://cses\.fi/([^/]+)/task/([^/]+)/?$)").match(url);
    if (match.hasMatch())
    {
        contest = match.captured(1);
        taskId = match.captured(2);
        return true;
    }

    // Pattern 2: Fallback for multi-level category structures (e.g., cses.fi/category/subcategory/taskId)
    match = QRegularExpression(R"(.*://cses\.fi/([^/]+)/([^/]+)/([^/]+)/?$)").match(url);
    if (match.hasMatch())
    {
        contest = match.captured(1);
        taskId = match.captured(3);
        return true;
    }

    return false;
}

void CSESTool::onReadReady()
{
    QString response = process->readAllStandardOutput();
    response.remove(QRegularExpression("\x1b\\[.. "));
    if (!response.trimmed().isEmpty())
    {
        // log everything as info; CSES CLI output may contain useful info
        if (log)
            log->info(tr("CSES CLI"), response);
        lastStatus = response.left(response.indexOf('\n') >= 0 ? response.indexOf('\n') : response.length());
    }
}

void CSESTool::onFinished(int exitCode, QProcess::ExitStatus)
{
    if (exitCode == 0)
    {
        showToastMessage(lastStatus);
    }
    else
    {
        showToastMessage(tr("CSES CLI failed"));
        if (log)
            log->error(tr("CSES CLI"), tr("CSES CLI finished with non-zero exit code %1").arg(exitCode));
    }
    QString err = process->readAllStandardError();
    if (!err.trimmed().isEmpty())
        if (log)
            log->error(tr("CSES CLI"), err);
}

void CSESTool::showToastMessage(const QString &message)
{
    // Use the existing CF toast setting for visibility to avoid depending on generated helpers.
    if (SettingsHelper::isCSESCLIShowToastMessages())
        emit requestToastMessage(tr("CSES"), message);
}

QString CSESTool::getVersion() const
{
    QProcess processV;
    // CSES CLI shows version info when run without arguments
    processV.start(path, {});
    if (!processV.waitForFinished(2000))
    {
        LOG_WARN("CSES CLI didn't finish after 2 second");
        return "";
    }
    QString output = processV.readAllStandardOutput();
    // Parse version from output like "CSES CLI version 0.1.4"
    QString version = QRegularExpression(R"((?<=version )\d+\.\d+\.\d+)").match(output).captured();
    LOG_INFO(INFO_OF(version));
    return version;
}

} // namespace Extensions
