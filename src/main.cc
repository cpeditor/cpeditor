/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QTextStream>

#include "appwindow.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("CP Editor");
    QCoreApplication::setApplicationVersion(APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_VERSION_PATCH);

    QTextStream cerr(stderr, QIODevice::WriteOnly);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(R"(
CPEditor [-d/--depth <depth>] [--cpp] [--java] [--python] [--no-hot-exit] [<path1> [<path2> [...]]]
CPEditor [-c/--contest] [--cpp] [--java] [--python] [--no-hot-exit] <number of problems> <contest directory>)");
    parser.addOptions(
        {{{"d", "depth"}, "Maximum depth when opening files in directories. No limit if not specified.", "depth", "-1"},
         {{"c", "contest"}, "Open a contest. i.e. Open files named A, B, ..., Z in a given directory."},
         {"cpp", "Open C++ files in given directories. / Use C++ for open contests."},
         {"java", "Open Java files in given directories. / Use Java for open contests."},
         {"python", "Open Python files in given directories. / Use Python for open contests."},
         {"no-hot-exit", "Do not load hot exit in this session. You won't be able to load the last session again."}});
    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsOptions);
    parser.process(app);

#define GETSET(x) bool x = parser.isSet(#x)
    GETSET(cpp);
    GETSET(java);
    GETSET(python);
    GETSET(contest);
#undef GETSET
    bool noHotExit = parser.isSet("no-hot-exit");
    auto args = parser.positionalArguments();

    if (contest)
    {
        bool ok = false;
        int number = args[0].toInt(&ok);

        if (args.length() != 2)
        {
            cerr << "CPEditor [-c/--contest] [--cpp] [--java] [--python] [--no-hot-exit] <number of problems> "
                    "<contest directory>\n\n";
            return 1;
        }

        if (!ok || number < 0 || number > 26)
        {
            cerr << "CPEditor [-c/--contest] [--cpp] [--java] [--python] [--no-hot-exit] <number of problems> "
                    "<contest directory>\n\nNumber of problems should be an integer in 0~26.\n\n";
            return 1;
        }

        AppWindow w(cpp, java, python, noHotExit, number, args[1]);
        w.show();
        return app.exec();
    }
    else
    {
        bool ok = false;
        int depth = parser.value("depth").toInt(&ok);

        if (!ok || depth < -1)
        {
            cerr << "CPEditor [-d/--depth <depth>] [--cpp] [--java] [--python] [--no-hot-exit] [<path1> [<path2> "
                    "[...]]]\n\nDepth should be a non-negative integer.\n\n";
            return 1;
        }

        if (!cpp && !java && !python)
            cpp = java = python = true;

        AppWindow w(depth, cpp, java, python, noHotExit, args);
        w.show();
        return app.exec();
    }
}
