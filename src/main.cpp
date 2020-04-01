/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Core/EventLogger.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStyleFactory>
#include <QTextStream>
#include <singleapplication.h>

#include "appwindow.hpp"
#include "mainwindow.hpp"

#define TOJSON(x) json[#x] = x

void setWindowsDarkTheme()
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    if (settings.value("AppsUseLightTheme") == 0)
    {
        Core::Log::i("main/Normal", "Using dark pallete for the theme as Windows Settings");
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        QColor darkColor = QColor(45, 45, 45);
        QColor disabledColor = QColor(127, 127, 127);
        darkPalette.setColor(QPalette::Window, darkColor);
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
        darkPalette.setColor(QPalette::AlternateBase, darkColor);
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
        darkPalette.setColor(QPalette::Button, darkColor);
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
#endif
}

int main(int argc, char *argv[])
{
    setWindowsDarkTheme();

    SingleApplication app(argc, argv, true);
    SingleApplication::setApplicationName("CP Editor");
    SingleApplication::setApplicationVersion(APP_VERSION);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setWindowIcon(QIcon(":/icon.png"));

    QTextStream cerr(stderr, QIODevice::WriteOnly);

    QString programName(argv[0]);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(programName + " [-d/--depth <depth>] [options] [<path1> [<path2> [...]]]\n" +
                                     programName +
                                     " [-c/--contest] [options] <number of problems> <contest directory>");
    parser.addOptions(
        {{{"d", "depth"}, "Maximum depth when opening files in directories. No limit if not specified.", "depth", "-1"},
         {{"c", "contest"}, "Open a contest. i.e. Open files named A, B, ..., Z in a given directory."},
         /*{{"n", "new"},
          "Open a new window instead of open tabs in an existing window. This may cause error of the competitive "
          "companion server."},*/
         {"cpp", "Open C++ files in given directories. / Use C++ for open contests."},
         {"java", "Open Java files in given directories. / Use Java for open contests."},
         {"python", "Open Python files in given directories. / Use Python for open contests."},
         {"verbose", "Dump all logs to stderr of the application. (use only for debug purpose)"},
         {"no-hot-exit", "Do not load hot exit in this session. You won't be able to load the last session again."}});
    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsOptions);
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.process(app);

#define GETSET(x) bool x = parser.isSet(#x)
    GETSET(cpp);
    GETSET(java);
    GETSET(python);
    GETSET(contest);
#undef GETSET
    bool noHotExit = parser.isSet("no-hot-exit");
    bool shouldDumpTostderr = parser.isSet("verbose");

    auto instance = app.instanceId();
    Core::Log::init(instance, shouldDumpTostderr);
    Core::Log::i("main") << "Instance number: " << instance << endl;

    auto args = parser.positionalArguments();

    if (contest)
    {
        Core::Log::i("main/Contest", "Branched into contest");
        if (args.length() != 2)
        {
            cerr << "Invalid Arguments\n\n"
                 << "See " + programName + " --help for more infomation.\n\n";
            Core::Log::e("Contest", "Exiting because contest arguments are not satisfied");
            return 1;
        }
        Core::Log::i("main/Contest", "Argument length is okay, Begining parsing of arguments to int");

        bool ok = false;
        int number = args[0].toInt(&ok);

        if (!ok || number < 0 || number > 26)
        {
            swap(args[0], args[1]);
            Core::Log::w("main/Contest", "Failed to parse : " + args[1]);
            Core::Log::w("main/Contest", " Trying next (i.e) : " + args[0]);
            number = args[0].toInt(&ok);
        }

        if (!ok || number < 0 || number > 26)
        {
            cerr << "Number of problems should be an integer in 0~26.\n\n"
                 << "See " + programName + " --help for more infomation.\n\n";
            Core::Log::e("main/Contest") << "Bad arguments for contest: [" << args.join(", ") << "]" << endl;
            return 1;
        }

        auto path = args[1];
        if (QFileInfo(path).isRelative())
            path = QDir::current().filePath(path);

        Core::Log::i("main/Contest", "Path extracted as : " + path);

        if (/*!parser.isSet("new") &&*/ app.isSecondary())
        {
            QJsonObject json;
            json["type"] = "contest";
            TOJSON(cpp);
            TOJSON(java);
            TOJSON(python);
            TOJSON(number);
            TOJSON(path);
            if (app.sendMessage("AAAAAAAAAAAAAAAAAAAANOLOSTDATA" + QJsonDocument(json).toBinaryData()))
            {
                Core::Log::i("Contest/Instance",
                             "This is secondary application. Sending to primary instance the binary data : " +
                                 QJsonDocument(json).toJson(QJsonDocument::Compact));
                cerr << "There is already a CP Editor running. New tabs are opened there.\n";
                return 0;
            }
        }

        Core::Log::i("main/Contest") << "Launching the new Appwindow with args: " << INFO_OF(cpp) << ", "
                                     << INFO_OF(java) << ", " << INFO_OF(python) << ", " << INFO_OF(noHotExit) << ", "
                                     << INFO_OF(number) << ", " << INFO_OF(path) << endl;

        AppWindow w(cpp, java, python, noHotExit, number, path);
        Core::Log::i("main/Contest", "Launched window connecting this window to onRecieveMessage()");
        QObject::connect(&app, &SingleApplication::receivedMessage, &w, &AppWindow::onReceivedMessage);
        Core::Log::i("main/Contest", "Showing the application window and begining the event loop");
        w.show();
        return app.exec();
    }
    else
    {
        Core::Log::i("main/Normal", "Branched to normal. Now parsing depth to int");
        bool ok = false;
        int depth = parser.value("depth").toInt(&ok);

        if (!ok || depth < -1)
        {
            Core::Log::e("main/Normal", "Failed to use parse depth. Provided : " + parser.value("depth"));
            cerr << "Depth should be a non-negative integer.\n\n"
                 << "See " + programName + " --help for more infomation.\n\n";
            return 1;
        }

        if (!cpp && !java && !python)
            cpp = java = python = true;

        for (auto &path : args)
        {
            if (QFileInfo(path).isRelative())
                path = QDir::current().filePath(path);
            Core::Log::i("main/Normal", "Path is : " + path);
        }

        if (/*!parser.isSet("new") &&*/ app.isSecondary())
        {
            QJsonObject json;
            json["type"] = "normal";
            TOJSON(depth);
            TOJSON(cpp);
            TOJSON(java);
            TOJSON(python);
            json["paths"] = QJsonArray::fromStringList(args);
            if (app.sendMessage("AAAAAAAAAAAAAAAAAAAANOLOSTDATA" + QJsonDocument(json).toBinaryData()))
            {
                Core::Log::i("main/Normal/Instance",
                             "This is secondary application. Sending to primary instance the data : " +
                                 QJsonDocument(json).toJson(QJsonDocument::Compact));
                cerr << "There is already a CP Editor running. New tabs are opened there.\n";
                return 0;
            }
        }
        Core::Log::i("main/Normal/main") << "Launching the new Appwindow with args: " << INFO_OF(depth) << ", "
                                         << INFO_OF(cpp) << ", " << INFO_OF(java) << ", " << INFO_OF(python) << ", "
                                         << INFO_OF(noHotExit) << ", " << INFO_OF(args.join(", ")) << endl;

        AppWindow w(depth, cpp, java, python, noHotExit, args);
        Core::Log::i("main/Normal", "Launched window connecting this window to onRecieveMessage()");
        QObject::connect(&app, &SingleApplication::receivedMessage, &w, &AppWindow::onReceivedMessage);
        Core::Log::i("main/Normal", "Showing the application window and begining the event loop");

        w.show();
        return app.exec();
    }
}

#undef TOJSON
