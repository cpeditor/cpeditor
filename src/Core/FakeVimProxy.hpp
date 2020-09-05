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

#ifndef FAKE_VIM_PROXY_HPP
#define FAKE_VIM_PROXY_HPP

#include "fakevimhandler.h"
#include <QObject>

class QStatusBar;
class QMainWindow;
class QString;
class QLabel;

namespace Core
{
class FakeVimProxy : public QObject
{
    Q_OBJECT

  public:
    FakeVimProxy(QWidget *widget, QMainWindow *mw, QObject *parent);
    void openFile(QString const &filename);
    static void initHandler(FakeVim::Internal::FakeVimHandler *handler);
    static void clearUndoRedo(QWidget *);
    static void connectSignals(FakeVim::Internal::FakeVimHandler *handler, QMainWindow *mainwindow, QWidget *widget);

  signals:
    void handleInput(QString const &key);

  public slots:
    void changeStatusData(QString const &);
    void highlightMatches(QString const &);
    void changeStatusMessage(QString const &, int);
    void changeExtraInformation(QString const &);
    void handleExCommand(bool *, const FakeVim::Internal::ExCommand &);
    void requestSetBlockSelection(QTextCursor const &);
    void requestDisableBlockSelection();
    void updateBlockSelection();
    void requestHasBlockSelection(bool *);
    void indentRegion(int, int, QChar);
    void checkForElectricCharacter(bool *, QChar);

  private:
    static int firstNonSpace(QString const &);
    void updateExtraSelections();
    bool wantSaveAndQuit(FakeVim::Internal::ExCommand const &);
    bool wantSave(FakeVim::Internal::ExCommand const &);
    bool wantQuit(FakeVim::Internal::ExCommand const &);
    bool save();
    void cancel();
    void setStatusBar();
    void invalidate();
    bool hasChanges();
    QTextDocument *document() const;
    QString content() const;

    QWidget *m_widget;
    QMainWindow *m_mainWindow;
    QLabel *m_statusMessage = nullptr;
    QLabel *m_statusData = nullptr;

    QList<QTextEdit::ExtraSelection> m_searchSelection;
    QList<QTextEdit::ExtraSelection> m_clearSelection;
    QList<QTextEdit::ExtraSelection> m_blockSelection;
};
} // namespace Core
#endif
