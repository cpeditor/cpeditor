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

#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonParseError>
#include <QObject>
#include <QStandardPaths>
#include <qtest.h>

namespace KSyntaxHighlighting
{
class FormatCollector : public AbstractHighlighter
{
public:
    using AbstractHighlighter::highlightLine;
    void applyFormat(int offset, int length, const Format &format) Q_DECL_OVERRIDE
    {
        Q_UNUSED(offset);
        Q_UNUSED(length);
        formatMap.insert(format.name(), format);
    }
    QHash<QString, Format> formatMap;
};

class ThemeTest : public QObject
{
    Q_OBJECT
private:
    Repository m_repo;

private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
        initRepositorySearchPaths(m_repo);
    }

    void testThemes()
    {
        QVERIFY(!m_repo.themes().isEmpty());
        for (const auto &theme : m_repo.themes()) {
            QVERIFY(theme.isValid());
            QVERIFY(!theme.name().isEmpty());
            QVERIFY(!theme.filePath().isEmpty());
            QVERIFY(QFileInfo::exists(theme.filePath()));
            QVERIFY(m_repo.theme(theme.name()).isValid());
        }
    }

    void testFormat_data()
    {
        QTest::addColumn<QString>("themeName");
        QTest::newRow("default") << "Default";
        QTest::newRow("dark") << "Breeze Dark";
        QTest::newRow("print") << "Printing";
    }

    void testFormat()
    {
        QFETCH(QString, themeName);

        // somewhat complicated way to get proper Format objects
        FormatCollector collector;
        collector.setDefinition(m_repo.definitionForName(QLatin1String("QML")));
        const auto t = m_repo.theme(themeName);
        QVERIFY(t.isValid());
        collector.setTheme(t);
        collector.highlightLine(QLatin1String("normal + property real foo: 3.14"), State());

        QVERIFY(collector.formatMap.size() >= 4);
        // qDebug() << collector.formatMap.keys();

        // normal text
        auto f = collector.formatMap.value(QLatin1String("Normal Text"));
        QVERIFY(f.isValid());
        QVERIFY(f.textStyle() == Theme::Normal);
        QVERIFY(f.isDefaultTextStyle(t));
        QVERIFY(!f.hasTextColor(t));
        QVERIFY(!f.hasBackgroundColor(t));
        QVERIFY(f.id() > 0);

        // visually identical to normal text
        f = collector.formatMap.value(QLatin1String("Symbol"));
        QVERIFY(f.isValid());
        QCOMPARE(f.textStyle(), Theme::Operator);
        QVERIFY(f.isDefaultTextStyle(t));
        QVERIFY(!f.hasTextColor(t));
        QVERIFY(f.id() > 0);

        // visually different to normal text
        f = collector.formatMap.value(QLatin1String("Keywords"));
        QVERIFY(f.isValid());
        QCOMPARE(f.textStyle(), Theme::Keyword);
        QVERIFY(!f.isDefaultTextStyle(t));
        QVERIFY(f.isBold(t));
        QVERIFY(f.id() > 0);

        f = collector.formatMap.value(QLatin1String("Float"));
        QVERIFY(f.isValid());
        QCOMPARE(f.textStyle(), Theme::Float);
        QVERIFY(!f.isDefaultTextStyle(t));
        QVERIFY(f.hasTextColor(t));
        QVERIFY(f.id() > 0);
    }

    void testDefaultTheme()
    {
        Theme t = m_repo.theme(QLatin1String("Default"));
        QVERIFY(t.isValid());

        // Themes compiled in as resource are never writable
        QVERIFY(t.isReadOnly());

        // make sure all editor colors are properly read
        QCOMPARE(t.editorColor(Theme::BackgroundColor), QColor("#ffffff").rgb());
        QCOMPARE(t.editorColor(Theme::TextSelection), QColor("#94caef").rgb());
        QCOMPARE(t.editorColor(Theme::CurrentLine), QColor("#f8f7f6").rgb());
        QCOMPARE(t.editorColor(Theme::SearchHighlight), QColor("#ffff00").rgb());
        QCOMPARE(t.editorColor(Theme::ReplaceHighlight), QColor("#00ff00").rgb());
        QCOMPARE(t.editorColor(Theme::BracketMatching), QColor("#ffff00").rgb());
        QCOMPARE(t.editorColor(Theme::TabMarker), QColor("#d2d2d2").rgb());
        QCOMPARE(t.editorColor(Theme::SpellChecking), QColor("#bf0303").rgb());
        QCOMPARE(t.editorColor(Theme::IndentationLine), QColor("#d2d2d2").rgb());
        QCOMPARE(t.editorColor(Theme::IconBorder), QColor("#f0f0f0").rgb());
        QCOMPARE(t.editorColor(Theme::CodeFolding), QColor("#94caef").rgb());
        QCOMPARE(t.editorColor(Theme::LineNumbers), QColor("#a0a0a0").rgb());
        QCOMPARE(t.editorColor(Theme::CurrentLineNumber), QColor("#1e1e1e").rgb());
        QCOMPARE(t.editorColor(Theme::WordWrapMarker), QColor("#ededed").rgb());
        QCOMPARE(t.editorColor(Theme::ModifiedLines), QColor("#fdbc4b").rgb());
        QCOMPARE(t.editorColor(Theme::SavedLines), QColor("#2ecc71").rgb());
        QCOMPARE(t.editorColor(Theme::Separator), QColor("#898887").rgb());
        QCOMPARE(t.editorColor(Theme::MarkBookmark), QColor("#0000ff").rgb());
        QCOMPARE(t.editorColor(Theme::MarkBreakpointActive), QColor("#ff0000").rgb());
        QCOMPARE(t.editorColor(Theme::MarkBreakpointReached), QColor("#ffff00").rgb());
        QCOMPARE(t.editorColor(Theme::MarkBreakpointDisabled), QColor("#ff00ff").rgb());
        QCOMPARE(t.editorColor(Theme::MarkExecution), QColor("#a0a0a4").rgb());
        QCOMPARE(t.editorColor(Theme::MarkWarning), QColor("#00ff00").rgb());
        QCOMPARE(t.editorColor(Theme::MarkError), QColor("#ff0000").rgb());
        QCOMPARE(t.editorColor(Theme::TemplateBackground), QColor("#d6d2d0").rgb());
        QCOMPARE(t.editorColor(Theme::TemplatePlaceholder), QColor("#baf8ce").rgb());
        QCOMPARE(t.editorColor(Theme::TemplateFocusedPlaceholder), QColor("#76da98").rgb());
        QCOMPARE(t.editorColor(Theme::TemplateReadOnlyPlaceholder), QColor("#f6e6e6").rgb());
    }

    void testInvalidTheme()
    {
        // somewhat complicated way to get proper Format objects
        FormatCollector collector;
        collector.setDefinition(m_repo.definitionForName(QLatin1String("QML")));
        collector.highlightLine(QLatin1String("normal + property real foo: 3.14"), State());

        QVERIFY(collector.formatMap.size() >= 4);
        auto f = collector.formatMap.value(QLatin1String("Normal Text"));
        QVERIFY(f.isValid());
        QVERIFY(f.isDefaultTextStyle(Theme()));
        QVERIFY(!f.hasTextColor(Theme()));
        QVERIFY(!f.hasBackgroundColor(Theme()));
    }

    void testThemeIntegrity_data()
    {
        QTest::addColumn<QString>("themeFileName");

        QDirIterator it(QStringLiteral(":/org.kde.syntax-highlighting/themes"), QStringList() << QLatin1String("*.theme"), QDir::Files);
        while (it.hasNext()) {
            const QString fileName = it.next();
            QTest::newRow(fileName.toLatin1().data()) << fileName;
        }
    }

    void testThemeIntegrity()
    {
        QFETCH(QString, themeFileName);

        QFile loadFile(themeFileName);
        QVERIFY(loadFile.open(QIODevice::ReadOnly));
        const QByteArray jsonData = loadFile.readAll();
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Failed to parse theme file:" << parseError.errorString();
            QVERIFY(false);
        }

        QJsonObject obj = jsonDoc.object();

        // verify metadata
        QVERIFY(obj.contains(QLatin1String("metadata")));
        const QJsonObject metadata = obj.value(QLatin1String("metadata")).toObject();
        QVERIFY(metadata.contains(QLatin1String("name")));
        QVERIFY(!metadata.value(QLatin1String("name")).toString().isEmpty());
        QVERIFY(metadata.contains(QLatin1String("revision")));
        QVERIFY(metadata.value(QLatin1String("revision")).toInt() > 0);

        // verify completeness of text styles
        static const auto idx = Theme::staticMetaObject.indexOfEnumerator("TextStyle");
        QVERIFY(idx >= 0);
        const auto metaEnum = Theme::staticMetaObject.enumerator(idx);
        QVERIFY(obj.contains(QLatin1String("text-styles")));
        const QJsonObject textStyles = obj.value(QLatin1String("text-styles")).toObject();
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            QCOMPARE(i, metaEnum.value(i));
            const QString textStyleName = QLatin1String(metaEnum.key(i));
            QVERIFY(textStyles.contains(textStyleName));
            const QJsonObject textStyle = textStyles.value(textStyleName).toObject();
            QVERIFY(textStyle.contains(QLatin1String("text-color")));

            // verify valid entry
            const QStringList definedColors = textStyle.keys();
            for (const auto &key : definedColors) {
                const QString context = textStyleName + QLatin1Char('/') + key + QLatin1Char('=') + textStyle.value(key).toString();
                if (key == QLatin1String("text-color") || key == QLatin1String("selected-text-color") || key == QLatin1String("background-color") || key == QLatin1String("selected-background-color")) {
                    QVERIFY2(QColor::isValidColor(textStyle.value(key).toString()), context.toLatin1().data());
                } else if (key == QLatin1String("bold") || key == QLatin1String("italic") || key == QLatin1String("underline") || key == QLatin1String("strike-through")) {
                    QVERIFY2(textStyle.value(key).isBool(), context.toLatin1().data());
                }
            }
        }

        // editor area colors
        const QStringList requiredEditorColors = {QLatin1String("background-color"),
                                                  QLatin1String("bracket-matching"),
                                                  QLatin1String("code-folding"),
                                                  QLatin1String("current-line"),
                                                  QLatin1String("current-line-number"),
                                                  QLatin1String("icon-border"),
                                                  QLatin1String("indentation-line"),
                                                  QLatin1String("line-numbers"),
                                                  QLatin1String("mark-bookmark"),
                                                  QLatin1String("mark-breakpoint-active"),
                                                  QLatin1String("mark-breakpoint-disabled"),
                                                  QLatin1String("mark-breakpoint-reached"),
                                                  QLatin1String("mark-error"),
                                                  QLatin1String("mark-execution"),
                                                  QLatin1String("mark-warning"),
                                                  QLatin1String("modified-lines"),
                                                  QLatin1String("replace-highlight"),
                                                  QLatin1String("saved-lines"),
                                                  QLatin1String("search-highlight"),
                                                  QLatin1String("selection"),
                                                  QLatin1String("separator"),
                                                  QLatin1String("spell-checking"),
                                                  QLatin1String("tab-marker"),
                                                  QLatin1String("template-background"),
                                                  QLatin1String("template-focused-placeholder"),
                                                  QLatin1String("template-placeholder"),
                                                  QLatin1String("template-read-only-placeholder"),
                                                  QLatin1String("word-wrap-marker")};

        // verify all editor colors are defined - not more, not less
        QVERIFY(obj.contains(QLatin1String("editor-colors")));
        const QJsonObject editorColors = obj.value(QLatin1String("editor-colors")).toObject();
        QStringList definedEditorColors = editorColors.keys();
        std::sort(definedEditorColors.begin(), definedEditorColors.end());
        QCOMPARE(definedEditorColors, requiredEditorColors);

        // verify all editor colors are valid
        for (const auto &key : requiredEditorColors) {
            QVERIFY(QColor::isValidColor(editorColors.value(key).toString()));
        }
    }
};
}

QTEST_GUILESS_MAIN(KSyntaxHighlighting::ThemeTest)

#include "theme_test.moc"
