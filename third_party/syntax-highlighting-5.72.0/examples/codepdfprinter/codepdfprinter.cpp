/*
    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "codepdfprinter.h"

#include <definition.h>
#include <syntaxhighlighter.h>
#include <theme.h>

#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QPrinter>

CodePdfPrinter::CodePdfPrinter()
    : m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(&m_document))
{
    const auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const QFontMetrics fontMetrics(font);
    m_document.setDefaultFont(font);

    QTextOption textOption(Qt::AlignTop | Qt::AlignLeft);
    textOption.setTabStopDistance(8 * fontMetrics.horizontalAdvance(QLatin1Char(' ')));
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_document.setDefaultTextOption(textOption);

    // light theme for "printing" on white PDF "paper"
    const auto theme = m_repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme);
    m_highlighter->setTheme(theme);
}

CodePdfPrinter::~CodePdfPrinter() = default;

bool CodePdfPrinter::openSourceFile(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open" << fileName << ":" << f.errorString();
        return false;
    }

    const auto def = m_repository.definitionForFileName(fileName);
    m_highlighter->setDefinition(def);

    m_document.setPlainText(QString::fromUtf8(f.readAll()));
    return true;
}

void CodePdfPrinter::printPdfFile(const QString &fileName)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    m_document.print(&printer);
}
