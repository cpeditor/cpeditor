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
#include <foldingregion.h>
#include <repository.h>
#include <state.h>

#include <QDir>
#include <QFile>
#include <QObject>
#include <QStandardPaths>
#include <QTextStream>
#include <qtest.h>

using namespace KSyntaxHighlighting;

class FoldingHighlighter : public AbstractHighlighter
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
        bool indentationFoldEnabled = definition().indentationBasedFoldingEnabled();
        if (indentationFoldEnabled)
            m_out << "<indentfold>";
        while (!in.atEnd()) {
            const auto currentLine = in.readLine();
            state = highlightLine(currentLine, state);

            if (indentationFoldEnabled != state.indentationBasedFoldingEnabled()) {
                indentationFoldEnabled = state.indentationBasedFoldingEnabled();
                if (indentationFoldEnabled)
                    m_out << "<indentfold>";
                else
                    m_out << "</indentfold>";
            }

            int offset = 0;
            for (const auto &fold : qAsConst(m_folds)) {
                m_out << currentLine.mid(offset, fold.offset - offset);
                if (fold.region.type() == FoldingRegion::Begin)
                    m_out << "<beginfold id='" << fold.region.id() << "'>";
                else
                    m_out << "<endfold id='" << fold.region.id() << "'>";
                m_out << currentLine.mid(fold.offset, fold.length);
                if (fold.region.type() == FoldingRegion::Begin)
                    m_out << "</beginfold id='" << fold.region.id() << "'>";
                else
                    m_out << "</endfold id='" << fold.region.id() << "'>";
                offset = fold.offset + fold.length;
            }
            m_out << currentLine.mid(offset) << '\n';
            m_folds.clear();
        }

        m_out.flush();
    }

protected:
    void applyFormat(int offset, int length, const Format &format) Q_DECL_OVERRIDE
    {
        Q_UNUSED(offset);
        Q_UNUSED(length);
        Q_UNUSED(format);
    }

    void applyFolding(int offset, int length, FoldingRegion region) Q_DECL_OVERRIDE
    {
        Q_ASSERT(region.isValid());
        m_folds.push_back({offset, length, region});
    }

private:
    QTextStream m_out;
    struct Fold {
        int offset;
        int length;
        FoldingRegion region;
    };
    QVector<Fold> m_folds;
};

class FoldingTest : public QObject
{
    Q_OBJECT
public:
    explicit FoldingTest(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

private:
private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testFolding_data()
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

            QTest::newRow(fileName.toUtf8().constData()) << inFile << (QStringLiteral(TESTBUILDDIR "/folding.out/") + fileName + QStringLiteral(".fold")) << (QStringLiteral(TESTSRCDIR "/folding/") + fileName + QStringLiteral(".fold"))
                                                         << syntax;
        }

        QDir().mkpath(QStringLiteral(TESTBUILDDIR "/folding.out/"));
    }

    void testFolding()
    {
        QFETCH(QString, inFile);
        QFETCH(QString, outFile);
        QFETCH(QString, refFile);
        QFETCH(QString, syntax);

        Repository m_repo;
        initRepositorySearchPaths(m_repo);

        auto def = m_repo.definitionForFileName(inFile);
        if (!syntax.isEmpty())
            def = m_repo.definitionForName(syntax);

        FoldingHighlighter highlighter;

        QVERIFY(def.isValid());
        highlighter.setDefinition(def);
        highlighter.highlightFile(inFile, outFile);

        /**
         * compare results
         */
        compareFiles(refFile, outFile);
    }
};

QTEST_GUILESS_MAIN(FoldingTest)

#include "foldingtest.moc"
