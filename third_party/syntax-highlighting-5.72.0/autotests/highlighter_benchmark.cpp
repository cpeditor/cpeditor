/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "test-config.h"

#include <abstracthighlighter.h>
#include <definition.h>
#include <format.h>
#include <repository.h>
#include <state.h>

#include <QDir>
#include <QObject>
#include <qtest.h>

using namespace KSyntaxHighlighting;

class NullHighlighter : public AbstractHighlighter
{
public:
    /**
     * Read in the given file and cache it for the highlighting benchmarking
     * @param inFileName file to read
     */
    NullHighlighter(const QString &inFileName)
    {
        QFile f(inFileName);
        if (!f.open(QFile::ReadOnly)) {
            qWarning() << "Failed to open input file" << inFileName << ":" << f.errorString();
            return;
        }

        QTextStream in(&f);
        in.setCodec("UTF-8");
        while (!in.atEnd())
            m_fileContents.append(in.readLine());
    }

    /**
     * highlight the in-memory stored file
     * @return number of highlighted lines
     */
    int highlightFile()
    {
        State state;
        for (const auto &line : qAsConst(m_fileContents))
            state = highlightLine(line, state);
        return m_fileContents.size();
    }

protected:
    void applyFormat(int, int, const Format &) Q_DECL_OVERRIDE
    {
    }
    QStringList m_fileContents;
};

class HighlighterBenchmark : public QObject
{
    Q_OBJECT
public:
    explicit HighlighterBenchmark(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

private:
    Repository m_repo;

private Q_SLOTS:
    void initTestCase()
    {
        initRepositorySearchPaths(m_repo);
    }

    void cleanupTestCase()
    {
    }

    void benchmarkHighlight_data()
    {
        QTest::addColumn<QString>("inFile");
        QTest::addColumn<QString>("syntax");

        const QDir dir(QStringLiteral(TESTSRCDIR "/input"));
        for (const auto &fileName : dir.entryList(QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Name)) {
            const auto inFile = dir.absoluteFilePath(fileName);
            if (inFile.endsWith(QLatin1String(".syntax")))
                continue;

            QString syntax;
            QFile syntaxOverride(inFile + QStringLiteral(".syntax"));
            if (syntaxOverride.exists() && syntaxOverride.open(QFile::ReadOnly))
                syntax = QString::fromUtf8(syntaxOverride.readAll()).trimmed();

            QTest::newRow(fileName.toUtf8().constData()) << inFile << syntax;
        }
    }

    void benchmarkHighlight()
    {
        QFETCH(QString, inFile);
        QFETCH(QString, syntax);

        NullHighlighter highlighter(inFile);
        auto def = m_repo.definitionForFileName(inFile);
        if (!syntax.isEmpty())
            def = m_repo.definitionForName(syntax);
        QVERIFY(def.isValid());
        highlighter.setDefinition(def);

        // trigger loading of definition per benchmarking loop
        QVERIFY(!def.formats().isEmpty());

        // benchmark the highlighting
        // try to highlight ~ 20000 lines per file
        // bail out, if file is empty, else we are stuck
        for (int i = 0; i <= 20000;) {
            int lines = highlighter.highlightFile();
            if (lines <= 0)
                break;
            i += lines;
        }
    }
};

QTEST_GUILESS_MAIN(HighlighterBenchmark)

#include "highlighter_benchmark.moc"
