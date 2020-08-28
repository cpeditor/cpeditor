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

/* Most of the code in this file is under the following copyright

    Copyright (c) 2017, Lukas Holecek <hluk@email.cz>
    This file is part of CopyQ.
    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "fakevimactions.h"
#include "fakevimhandler.h"
#include <Extensions/FakeVimProxy.hpp>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QTextBlock>
#include <QTextEdit>
#include <QTextStream>
#include <QWidget>
#include <QLabel>

namespace Extensions
{

using _ = QLatin1String;
FakeVimProxy::FakeVimProxy(QWidget *widget, QMainWindow *mw, QObject *parent)
    : QObject(parent), m_widget(widget), m_mainWindow(mw)
{
	m_statusData = new QLabel(m_mainWindow);
	m_statusMessage = new QLabel(m_mainWindow);
	setStatusBar();
}

void FakeVimProxy::openFile(QString const &fileName)
{
    emit handleInput(QString(_(":r %1<CR>")).arg(fileName));
    m_fileName = fileName;
}

void FakeVimProxy::changeStatusData(QString const &info)
{
    m_statusData->setText(info);
}
void FakeVimProxy::highlightMatches(QString const &pattern)
{
    QTextCursor cur;
    QTextDocument *doc = nullptr;

    { // in a block so we don't inadvertently use one of them later
        QPlainTextEdit *plainEditor = qobject_cast<QPlainTextEdit *>(m_widget);
        QTextEdit *editor = qobject_cast<QTextEdit *>(m_widget);
        if (editor)
        {
            cur = editor->textCursor();
            doc = editor->document();
        }
        else if (plainEditor)
        {
            cur = plainEditor->textCursor();
            doc = plainEditor->document();
        }
        else
        {
            return;
        }
    }
    Q_ASSERT(doc);

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(Qt::yellow);
    selection.format.setForeground(Qt::black);

    // Highlight matches.
    QRegExp re(pattern);
    cur = doc->find(re);

    m_searchSelection.clear();

    int a = cur.position();
    while (!cur.isNull())
    {
        if (cur.hasSelection())
        {
            selection.cursor = cur;
            m_searchSelection.append(selection);
        }
        else
        {
            cur.movePosition(QTextCursor::NextCharacter);
        }
        cur = doc->find(re, cur);
        int b = cur.position();
        if (a == b)
        {
            cur.movePosition(QTextCursor::NextCharacter);
            cur = doc->find(re, cur);
            b = cur.position();
            if (a == b)
                break;
        }
        a = b;
    }

    updateExtraSelections();
}

void FakeVimProxy::changeStatusMessage(QString const &contents, int cursorPos)
{
    m_statusMessage->setText(cursorPos == -1 ? contents : contents.left(cursorPos) + QChar(10073) + contents.mid(cursorPos));
}

void FakeVimProxy::changeExtraInformation(QString const &info)
{
    QMessageBox::information(m_widget, tr("Information"), info);
}

void FakeVimProxy::setStatusBar()
{
    m_mainWindow->statusBar()->addPermanentWidget(m_statusData);
    m_mainWindow->statusBar()->addWidget(m_statusMessage);
}

void FakeVimProxy::handleExCommand(bool *handled, FakeVim::Internal::ExCommand const &cmd)
{
    if (wantSaveAndQuit(cmd))
    {
        // :wq
        if (save())
            cancel();
    }
    else if (wantSave(cmd))
    {
        save(); // :w
    }
    else if (wantQuit(cmd))
    {
        if (cmd.hasBang)
            invalidate(); // :q!
        else
            cancel(); // :q
    }
    else
    {
        *handled = false;
        return;
    }

    *handled = true;
}

void FakeVimProxy::requestSetBlockSelection(const QTextCursor &tc)
{
    QTextEdit *editor = qobject_cast<QTextEdit *>(m_widget);
    QPlainTextEdit *plainEditor = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!editor && !plainEditor)
    {
        return;
    }

    QPalette pal = m_widget->parentWidget() != nullptr ? m_widget->parentWidget()->palette() : QApplication::palette();

    m_blockSelection.clear();
    m_clearSelection.clear();

    QTextCursor cur = tc;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(pal.color(QPalette::Base));
    selection.format.setForeground(pal.color(QPalette::Text));
    selection.cursor = cur;
    m_clearSelection.append(selection);

    selection.format.setBackground(pal.color(QPalette::Highlight));
    selection.format.setForeground(pal.color(QPalette::HighlightedText));

    int from = cur.positionInBlock();
    int to = cur.anchor() - cur.document()->findBlock(cur.anchor()).position();
    const int min = qMin(cur.position(), cur.anchor());
    const int max = qMax(cur.position(), cur.anchor());
    for (QTextBlock block = cur.document()->findBlock(min); block.isValid() && block.position() < max;
         block = block.next())
    {
        cur.setPosition(block.position() + qMin(from, block.length()));
        cur.setPosition(block.position() + qMin(to, block.length()), QTextCursor::KeepAnchor);
        selection.cursor = cur;
        m_blockSelection.append(selection);
    }

    if (editor)
    {
        disconnect(editor, &QTextEdit::selectionChanged, this, &FakeVimProxy::updateBlockSelection);
        editor->setTextCursor(tc);
        connect(editor, &QTextEdit::selectionChanged, this, &FakeVimProxy::updateBlockSelection);
    }
    else
    {
        disconnect(plainEditor, &QPlainTextEdit::selectionChanged, this, &FakeVimProxy::updateBlockSelection);
        plainEditor->setTextCursor(tc);
        connect(plainEditor, &QPlainTextEdit::selectionChanged, this, &FakeVimProxy::updateBlockSelection);
    }

    QPalette pal2 = m_widget->palette();
    pal2.setColor(QPalette::Highlight, Qt::transparent);
    pal2.setColor(QPalette::HighlightedText, Qt::transparent);
    m_widget->setPalette(pal2);

    updateExtraSelections();
}

void FakeVimProxy::requestDisableBlockSelection()
{
    QTextEdit *editor = qobject_cast<QTextEdit *>(m_widget);
    QPlainTextEdit *plainEditor = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!editor && !plainEditor)
    {
        return;
    }

    QPalette pal = m_widget->parentWidget() != nullptr ? m_widget->parentWidget()->palette() : QApplication::palette();

    m_blockSelection.clear();
    m_clearSelection.clear();

    m_widget->setPalette(pal);

    if (editor)
    {
        disconnect(editor, &QTextEdit::selectionChanged, this, &FakeVimProxy::updateBlockSelection);
    }
    else
    {
        disconnect(plainEditor, &QPlainTextEdit::selectionChanged, this, &FakeVimProxy::updateBlockSelection);
    }

    updateExtraSelections();
}

void FakeVimProxy::updateBlockSelection()
{
    QTextEdit *editor = qobject_cast<QTextEdit *>(m_widget);
    QPlainTextEdit *plainEditor = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!editor && !plainEditor)
    {
        return;
    }

    requestSetBlockSelection(editor ? editor->textCursor() : plainEditor->textCursor());
}

void FakeVimProxy::requestHasBlockSelection(bool *on)
{
    *on = !m_blockSelection.isEmpty();
}

void FakeVimProxy::indentRegion(int beginBlock, int endBlock, QChar typedChar)
{
    QTextDocument *doc = nullptr;
    { // in a block so we don't inadvertently use one of them later
        QPlainTextEdit *plainEditor = qobject_cast<QPlainTextEdit *>(m_widget);
        QTextEdit *editor = qobject_cast<QTextEdit *>(m_widget);
        if (editor)
        {
            doc = editor->document();
        }
        else if (plainEditor)
        {
            doc = plainEditor->document();
        }
        else
        {
            return;
        }
    }
    Q_ASSERT(doc);

    const int indentSize = FakeVim::Internal::theFakeVimSetting(FakeVim::Internal::ConfigShiftWidth)->value().toInt();

    QTextBlock startBlock = doc->findBlockByNumber(beginBlock);

    // Record line lengths for mark adjustments
    QVector<int> lineLengths(endBlock - beginBlock + 1);
    QTextBlock block = startBlock;

    for (int i = beginBlock; i <= endBlock; ++i)
    {
        const auto line = block.text();
        lineLengths[i - beginBlock] = line.length();
        if (typedChar.unicode() == 0 && line.simplified().isEmpty())
        {
            // clear empty lines
            QTextCursor cursor(block);
            while (!cursor.atBlockEnd())
                cursor.deleteChar();
        }
        else
        {
            const auto previousBlock = block.previous();
            const auto previousLine = previousBlock.isValid() ? previousBlock.text() : QString();

            int indent = firstNonSpace(previousLine);
            if (typedChar == '}')
                indent = std::max(0, indent - indentSize);
            else if (previousLine.endsWith("{"))
                indent += indentSize;
            const auto indentString = QString(" ").repeated(indent);

            QTextCursor cursor(block);
            cursor.beginEditBlock();
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, firstNonSpace(line));
            cursor.removeSelectedText();
            cursor.insertText(indentString);
            cursor.endEditBlock();
        }
        block = block.next();
    }
}

void FakeVimProxy::checkForElectricCharacter(bool *result, QChar c)
{
    *result = c == '{' || c == '}';
}

int FakeVimProxy::firstNonSpace(const QString &text)
{
    int indent = 0;
    while (indent < text.length() && text.at(indent) == ' ')
        ++indent;
    return indent;
}

void FakeVimProxy::updateExtraSelections()
{
    QTextEdit *editor = qobject_cast<QTextEdit *>(m_widget);
    QPlainTextEdit *plainEditor = qobject_cast<QPlainTextEdit *>(m_widget);
    if (editor)
    {
        editor->setExtraSelections(m_clearSelection + m_searchSelection + m_blockSelection);
    }
    else if (plainEditor)
    {
        plainEditor->setExtraSelections(m_clearSelection + m_searchSelection + m_blockSelection);
    }
}

bool FakeVimProxy::wantSaveAndQuit(const FakeVim::Internal::ExCommand &cmd)
{
    return cmd.cmd == "wq";
}

bool FakeVimProxy::wantSave(const FakeVim::Internal::ExCommand &cmd)
{
    return cmd.matches("w", "write") || cmd.matches("wa", "wall");
}

bool FakeVimProxy::wantQuit(const FakeVim::Internal::ExCommand &cmd)
{
    return cmd.matches("q", "quit") || cmd.matches("qa", "qall");
}

bool FakeVimProxy::save()
{
    if (!hasChanges())
        return true;

    QTemporaryFile tmpFile;
    if (!tmpFile.open())
    {
        QMessageBox::critical(m_widget, tr("FakeVim Error"),
                              tr("Cannot create temporary file: %1").arg(tmpFile.errorString()));
        return false;
    }

    QTextStream ts(&tmpFile);
    ts << content();
    ts.flush();

    QFile::remove(m_fileName);
    if (!QFile::copy(tmpFile.fileName(), m_fileName))
    {
        QMessageBox::critical(m_widget, tr("FakeVim Error"), tr("Cannot write to file \"%1\"").arg(m_fileName));
        return false;
    }

    return true;
}

void FakeVimProxy::cancel()
{
    if (hasChanges())
    {
        QMessageBox::critical(m_widget, tr("FakeVim Warning"), tr("File \"%1\" was changed").arg(m_fileName));
    }
    else
    {
        invalidate();
    }
}

void FakeVimProxy::invalidate()
{
    QApplication::quit();
}

bool FakeVimProxy::hasChanges()
{
    if (m_fileName.isEmpty() && content().isEmpty())
        return false;

    QFile f(m_fileName);
    if (!f.open(QIODevice::ReadOnly))
        return true;

    QTextStream ts(&f);
    return content() != ts.readAll();
}

QTextDocument *FakeVimProxy::document() const
{
    QTextDocument *doc = NULL;
    if (QPlainTextEdit *ed = qobject_cast<QPlainTextEdit *>(m_widget))
        doc = ed->document();
    else if (QTextEdit *ed = qobject_cast<QTextEdit *>(m_widget))
        doc = ed->document();
    return doc;
}

QString FakeVimProxy::content() const
{
    return document()->toPlainText();
}

void FakeVimProxy::initHandler(FakeVim::Internal::FakeVimHandler *handler)
{
    handler->handleCommand(_("set nopasskeys"));
    handler->handleCommand(_("set nopasscontrolkey"));

    // Set some Vim options.
    handler->handleCommand(_("set expandtab"));
    handler->handleCommand(_("set shiftwidth=8"));
    handler->handleCommand(_("set tabstop=16"));
    handler->handleCommand(_("set autoindent"));
    handler->handleCommand(_("set smartindent"));

    handler->installEventFilter();
    handler->setupWidget();
}

void FakeVimProxy::clearUndoRedo(QWidget *editor)
{
    if (QPlainTextEdit *ed = qobject_cast<QPlainTextEdit *>(editor))
    {
        ed->setUndoRedoEnabled(false);
        ed->setUndoRedoEnabled(true);
    }
    else if (QTextEdit *ed = qobject_cast<QTextEdit *>(editor))
    {
        ed->setUndoRedoEnabled(false);
        ed->setUndoRedoEnabled(true);
    }
}

void FakeVimProxy::connectSignals(FakeVim::Internal::FakeVimHandler *handler, QMainWindow *mainWindow, QWidget *editor,
                                  const QString &fileToEdit)
{
    FakeVimProxy *proxy = new FakeVimProxy(editor, mainWindow, handler);

    handler->commandBufferChanged.connect(
        [proxy](const QString &contents, int cursorPos, int /*anchorPos*/, int /*messageLevel*/) {
            proxy->changeStatusMessage(contents, cursorPos);
        });
    handler->extraInformationChanged.connect([proxy](const QString &text) { proxy->changeExtraInformation(text); });
    handler->statusDataChanged.connect([proxy](const QString &text) { proxy->changeStatusData(text); });
    handler->highlightMatches.connect([proxy](const QString &needle) { proxy->highlightMatches(needle); });
    handler->handleExCommandRequested.connect(
        [proxy](bool *handled, const FakeVim::Internal::ExCommand &cmd) { proxy->handleExCommand(handled, cmd); });
    handler->requestSetBlockSelection.connect(
        [proxy](const QTextCursor &cursor) { proxy->requestSetBlockSelection(cursor); });
    handler->requestDisableBlockSelection.connect([proxy] { proxy->requestDisableBlockSelection(); });
    handler->requestHasBlockSelection.connect([proxy](bool *on) { proxy->requestHasBlockSelection(on); });

    handler->indentRegion.connect([proxy](int beginBlock, int endBlock, QChar typedChar) {
        proxy->indentRegion(beginBlock, endBlock, typedChar);
    });
    handler->checkForElectricCharacter.connect(
        [proxy](bool *result, QChar c) { proxy->checkForElectricCharacter(result, c); });

    QObject::connect(proxy, &FakeVimProxy::handleInput, handler,
                     [handler](const QString &text) { handler->handleInput(text); });

    if (!fileToEdit.isEmpty())
        proxy->openFile(fileToEdit);
}

} // namespace Extensions
