#include <Expand.hpp>

#include "../ui/ui_expand.h"

Expand::Expand(QPlainTextEdit* e) : ui(new Ui::Expand) {
  ui->setupUi(this);
}

Expand::Expand(QTextBrowser* browser) : ui(new Ui::Expand) {
  ui->setupUi(this);
}
