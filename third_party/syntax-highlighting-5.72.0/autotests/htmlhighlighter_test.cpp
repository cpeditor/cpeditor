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

#include <definition.h>
#include <htmlhighlighter.h>
#include <repository.h>
#include <theme.h>

#include <QDir>
#include <QObject>
#include <QStandardPaths>
#include <qtest.h>

using namespace KSyntaxHighlighting;

class HTMLHighlighterTest : public QObject
{
    Q_OBJECT
public:
    explicit HTMLHighlighterTest(QObject *parent = nullptr)
        : QObject(parent)
        , m_repo(nullptr)
    {
    }

private:
    Repository *m_repo;

private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
        m_repo = new Repository;
        initRepositorySearchPaths(*m_repo);
    }

    void cleanupTestCase()
    {
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

            QTest::newRow(fileName.toUtf8().constData()) << inFile << (QStringLiteral(TESTBUILDDIR "/html.output/") + fileName + QStringLiteral(".html")) << (QStringLiteral(TESTSRCDIR "/html/") + fileName + QStringLiteral(".html"))
                                                         << syntax;
        }

        QDir().mkpath(QStringLiteral(TESTBUILDDIR "/html.output/"));
    }

    void testHighlight()
    {
        QFETCH(QString, inFile);
        QFETCH(QString, outFile);
        QFETCH(QString, refFile);
        QFETCH(QString, syntax);
        QVERIFY(m_repo);

        HtmlHighlighter highlighter;
        highlighter.setTheme(m_repo->defaultTheme());
        QVERIFY(highlighter.theme().isValid());

        auto def = m_repo->definitionForFileName(inFile);
        if (!syntax.isEmpty())
            def = m_repo->definitionForName(syntax);
        QVERIFY(def.isValid());
        highlighter.setDefinition(def);
        highlighter.setOutputFile(outFile);
        highlighter.highlightFile(inFile);

        /**
         * compare results
         */
        compareFiles(refFile, outFile);
    }
};

QTEST_GUILESS_MAIN(HTMLHighlighterTest)

#include "htmlhighlighter_test.moc"
