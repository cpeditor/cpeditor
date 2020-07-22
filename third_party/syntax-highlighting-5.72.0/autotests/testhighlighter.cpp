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
#include <theme.h>

#include <QDir>
#include <QFile>
#include <QObject>
#include <QTextStream>
#include <qtest.h>

using namespace KSyntaxHighlighting;

class TestHighlighter : public AbstractHighlighter
{
public:
    void highlightFile(const QString &inFileName, const QString &outFileName)
    {
        QFile outFile(outFileName);
        if (!outFile.open(QFile::WriteOnly | QFile::Truncate)) {
            qWarning() << "Failed to open output file" << outFileName << ":" << outFile.errorString();
            return;
        }
        m_out.setDevice(&outFile);
        m_out.setCodec("UTF-8");

        QFile f(inFileName);
        if (!f.open(QFile::ReadOnly)) {
            qWarning() << "Failed to open input file" << inFileName << ":" << f.errorString();
            return;
        }

        QTextStream in(&f);
        in.setCodec("UTF-8");
        State state;
        while (!in.atEnd()) {
            m_currentLine = in.readLine();
            state = highlightLine(m_currentLine, state);
            m_out << "<br/>\n";
        }

        m_out.flush();
    }

protected:
    void applyFormat(int offset, int length, const Format &format) Q_DECL_OVERRIDE
    {
        if (format.name().isEmpty())
            m_out << "<dsNormal>" << m_currentLine.midRef(offset, length) << "</dsNormal>";
        else
            m_out << "<" << format.name() << ">" << m_currentLine.midRef(offset, length) << "</" << format.name() << ">";
    }

private:
    QTextStream m_out;
    QString m_currentLine;
};

class TestHighlighterTest : public QObject
{
    Q_OBJECT
public:
    explicit TestHighlighterTest(QObject *parent = nullptr)
        : QObject(parent)
        , m_repo(nullptr)
    {
    }

private:
    Repository *m_repo;
    QSet<QString> m_coveredDefinitions;

private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
        m_repo = new Repository;
        initRepositorySearchPaths(*m_repo);
    }

    void cleanupTestCase()
    {
        QFile coveredList(QLatin1String(TESTBUILDDIR "/covered-definitions.txt"));
        QFile uncoveredList(QLatin1String(TESTBUILDDIR "/uncovered-definition.txt"));
        QVERIFY(coveredList.open(QFile::WriteOnly));
        QVERIFY(uncoveredList.open(QFile::WriteOnly));

        int count = 0;
        for (const auto &def : m_repo->definitions()) {
            if (def.isHidden() || !def.isValid())
                continue;
            ++count;
            if (m_coveredDefinitions.contains(def.name()))
                coveredList.write(def.name().toUtf8() + '\n');
            else
                uncoveredList.write(def.name().toUtf8() + '\n');
        }

        qDebug() << "Syntax definitions with test coverage:" << ((float)m_coveredDefinitions.size() * 100.0f / (float)count) << "%";

        delete m_repo;
        m_repo = nullptr;
    }

    void testHighlight_data()
    {
        QTest::addColumn<QString>("inFile");
        QTest::addColumn<QString>("outFile");
        QTest::addColumn<QString>("refFile");
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

            QTest::newRow(fileName.toUtf8().constData()) << inFile << (QStringLiteral(TESTBUILDDIR "/output/") + fileName + QStringLiteral(".ref")) << (QStringLiteral(TESTSRCDIR "/reference/") + fileName + QStringLiteral(".ref")) << syntax;
        }

        QVERIFY(QDir().mkpath(QStringLiteral(TESTBUILDDIR "/output/")));
    }

    void testHighlight()
    {
        QFETCH(QString, inFile);
        QFETCH(QString, outFile);
        QFETCH(QString, refFile);
        QFETCH(QString, syntax);
        QVERIFY(m_repo);

        auto def = m_repo->definitionForFileName(inFile);
        if (!syntax.isEmpty())
            def = m_repo->definitionForName(syntax);

        TestHighlighter highlighter;
        highlighter.setTheme(m_repo->defaultTheme());
        QVERIFY(highlighter.theme().isValid());

        QVERIFY(def.isValid());
        qDebug() << "Using syntax" << def.name();
        m_coveredDefinitions.insert(def.name());
        highlighter.setDefinition(def);
        highlighter.highlightFile(inFile, outFile);

        /**
         * compare results
         */
        compareFiles(refFile, outFile);
    }
};

QTEST_GUILESS_MAIN(TestHighlighterTest)

#include "testhighlighter.moc"
