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