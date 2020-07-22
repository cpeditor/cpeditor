/*
    Copyright (C) 2020 Aleix Pol Gonzalez <aleixpol@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <QApplication>
#include <QCommandLineParser>
#include <QPlainTextEdit>
#include <QDebug>

#include <definition.h>
#include <foldingregion.h>
#include <syntaxhighlighter.h>
#include <theme.h>
#include <repository.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument(QStringLiteral("source"), QStringLiteral("The source file to highlight."));
    parser.process(app);

    KSyntaxHighlighting::Repository repository;
    for (const QString &file : parser.positionalArguments()) {
        const auto url = QUrl::fromUserInput(file, {}, QUrl::UserInputResolutionOption::AssumeLocalFile);
        QFile f(url.toLocalFile());
        if (!f.open(QIODevice::ReadOnly))
            continue;

        auto view = new QPlainTextEdit();
        view->setPlainText(QString::fromUtf8(f.readAll()));
        view->resize(500, 500);

        auto highlighter = new KSyntaxHighlighting::SyntaxHighlighter(view->document());
        highlighter->setTheme((view->palette().color(QPalette::Base).lightness() < 128)
                 ? repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                 : repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));

        const auto def = repository.definitionForFileName(url.toLocalFile());
        highlighter->setDefinition(def);

        view->show();
    }

    return app.exec();
}
