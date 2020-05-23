/*
 * Copyright (c) 2014-2020 Patrizio Bekerle -- <patrizio@bekerle.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * QPlainTextEdit markdown highlighter
 */

#pragma once

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;

QT_END_NAMESPACE

class MarkdownHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

  public:
    enum HighlightingOption
    {
        None = 0,
        FullyHighlightedBlockQuote = 0x01,
        Underline = 0x02
    };
    Q_DECLARE_FLAGS(HighlightingOptions, HighlightingOption)

    MarkdownHighlighter(QTextDocument *parent = nullptr,
                        HighlightingOptions highlightingOptions = HighlightingOption::None);

    static inline QColor codeBlockBackgroundColor()
    {
        const QBrush brush = _formats[CodeBlock].background();

        if (!brush.isOpaque())
        {
            return QColor(Qt::transparent);
        }

        return brush.color();
    }

    static constexpr inline bool isOctal(const char c)
    {
        return (c >= '0' && c <= '7');
    }
    static constexpr inline bool isHex(const char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }
    static constexpr inline bool isCodeBlock(const int state)
    {
        return state == MarkdownHighlighter::CodeBlock || state == MarkdownHighlighter::CodeBlockTilde ||
               state == MarkdownHighlighter::CodeBlockComment || state == MarkdownHighlighter::CodeBlockTildeComment ||
               state >= MarkdownHighlighter::CodeCpp;
    }
    static constexpr inline bool isCodeBlockEnd(const int state)
    {
        return state == MarkdownHighlighter::CodeBlockEnd || state == MarkdownHighlighter::CodeBlockTildeEnd;
    }

    // we use some predefined numbers here to be compatible with
    // the peg-markdown parser
    enum HighlighterState
    {
        NoState = -1,
        Link = 0,
        Image = 3,
        CodeBlock,
        CodeBlockComment,
        Italic = 7,
        Bold,
        List,
        Comment = 11,
        H1,
        H2,
        H3,
        H4,
        H5,
        H6,
        BlockQuote,
        HorizontalRuler = 21,
        Table,
        InlineCodeBlock,
        MaskedSyntax,
        CurrentLineBackgroundColor,
        BrokenLink,
        FrontmatterBlock,
        TrailingSpace,
        CheckBoxUnChecked,
        CheckBoxChecked,

        // code highlighting
        CodeKeyWord = 1000,
        CodeString = 1001,
        CodeComment = 1002,
        CodeType = 1003,
        CodeOther = 1004,
        CodeNumLiteral = 1005,
        CodeBuiltIn = 1006,

        // internal
        CodeBlockIndented = 96,
        CodeBlockTildeEnd = 97,
        CodeBlockTilde = 98,
        CodeBlockTildeComment,
        CodeBlockEnd = 100,
        HeadlineEnd,
        FrontmatterBlockEnd,

        // languages
        /*********
         * When adding a language make sure that its value is a multiple of 2
         * This is because we use the next number as comment for that language
         * In case the language doesn't support multiline comments in the
         * traditional C++ sense, leave the next value empty. Otherwise mark the
         * next value as comment for that language. e.g CodeCpp = 200
         * CodeCppComment = 201
         */
        CodeCpp = 200,
        CodeCppComment = 201,
        CodeJs = 202,
        CodeJsComment = 203,
        CodeC = 204,
        CodeCComment = 205,
        CodeBash = 206,
        CodePHP = 208,
        CodePHPComment = 209,
        CodeQML = 210,
        CodeQMLComment = 211,
        CodePython = 212,
        CodeRust = 214,
        CodeRustComment = 215,
        CodeJava = 216,
        CodeJavaComment = 217,
        CodeCSharp = 218,
        CodeCSharpComment = 219,
        CodeGo = 220,
        CodeGoComment = 221,
        CodeV = 222,
        CodeVComment = 223,
        CodeSQL = 224,
        CodeJSON = 226,
        CodeXML = 228,
        CodeCSS = 230,
        CodeCSSComment = 231,
        CodeTypeScript = 232,
        CodeTypeScriptComment = 233,
        CodeYAML = 234,
        CodeINI = 236,
        CodeTaggerScript = 238,
        CodeVex = 240,
        CodeVexComment = 241,
        CodeCMake = 242,
        CodeMake = 244
    };
    Q_ENUMS(HighlighterState)

    //    enum BlockState {
    //        NoBlockState = 0,
    //        H1,
    //        H2,
    //        H3,
    //        Table,
    //        CodeBlock,
    //        CodeBlockEnd
    //    };

    static void setTextFormats(QHash<HighlighterState, QTextCharFormat> formats);
    static void setTextFormat(HighlighterState state, QTextCharFormat format);
    void clearDirtyBlocks();
    void setHighlightingOptions(const HighlightingOptions options);
    void initHighlightingRules();
  signals:
    void highlightingFinished();

  protected slots:
    void timerTick();

  protected:
    struct HighlightingRule
    {
        explicit HighlightingRule(const HighlighterState state_) : state(state_)
        {
        }
        HighlightingRule() = default;

        QRegularExpression pattern;
        QString shouldContain;
        HighlighterState state = NoState;
        uint8_t capturingGroup = 0;
        uint8_t maskedGroup = 0;
    };

    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

    static void initTextFormats(int defaultFontSize = 12);

    static void initCodeLangs();

    void highlightMarkdown(const QString &text);

    /******************************
     *  BLOCK LEVEL FUNCTIONS
     ******************************/

    void highlightHeadline(const QString &text);

    void highlightSubHeadline(const QString &text, HighlighterState state);

    void highlightAdditionalRules(const QVector<HighlightingRule> &rules, const QString &text);

    void highlightFrontmatterBlock(const QString &text);

    void highlightCommentBlock(const QString &text);

    void highlightThematicBreak(const QString &text);

    void highlightLists(const QString &text);

    /******************************
     *  INLINE FUNCTIONS
     ******************************/

    void highlightInlineRules(const QString &text);

    Q_REQUIRED_RESULT int highlightInlineSpans(const QString &text, int currentPos, const QChar c);

    void highlightEmAndStrong(const QString &text, const int pos);

    Q_REQUIRED_RESULT int highlightInlineComment(const QString &text, int pos);

    void setHeadingStyles(MarkdownHighlighter::HighlighterState rule, const QRegularExpressionMatch &match,
                          const int capturedGroup);

    /******************************
     *  CODE HIGHLIGHTING FUNCTIONS
     ******************************/

    void highlightIndentedCodeBlock(const QString &text);

    void highlightCodeFence(const QString &text);

    void highlightCodeBlock(const QString &text, const QString &opener = QStringLiteral("```"));

    void highlightSyntax(const QString &text);

    Q_REQUIRED_RESULT int highlightNumericLiterals(const QString &text, int i);

    Q_REQUIRED_RESULT int highlightStringLiterals(QChar strType, const QString &text, int i);

    void ymlHighlighter(const QString &text);

    void iniHighlighter(const QString &text);

    void cssHighlighter(const QString &text);

    void xmlHighlighter(const QString &text);

    void makeHighlighter(const QString &text);

    void taggerScriptHighlighter(const QString &text);

    void addDirtyBlock(const QTextBlock &block);

    void reHighlightDirtyBlocks();

    QVector<HighlightingRule> _highlightingRules;
    QVector<QPair<int, int>> _linkRanges;
    static QHash<HighlighterState, QTextCharFormat> _formats;
    static QHash<QString, HighlighterState> _langStringToEnum;
    QVector<QTextBlock> _dirtyTextBlocks;
    QTimer *_timer;
    bool _highlightingFinished;
    HighlightingOptions _highlightingOptions;
    static constexpr int tildeOffset = 300;
};
