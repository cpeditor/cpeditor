/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in an unexpected way and
 * causes your bad performance and / or lose any contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include <QCodeEditor>
#include <QString>

namespace Core
{
class ExtendedQCodeEditorFeatures
{
public:
  ExtendedQCodeEditorFeatures(QCodeEditor *editor, int tabSize);
    
	QString language();
  int tabSize();

	void setLanguage(QString lang);
  void setTabSize(int tab);
  void setEditor(QCodeEditor *editor);

  void toggleSelectionComment(bool forceLineCommenter = false);
    
	bool selectionTab();
	void selectionUnTab();
  void moveSelectionUp();
  void moveSelectionDown();


private:
  QString language_;
  int tabSize_;
  QCodeEditor *editor_ = nullptr;
  bool selectionPrepend_(QString content);
  bool selectionRemoveFirstCharIf_(QStringList deleteList);
};
} // namespace Core