#include "Preferences/PreferencesGridPage.hpp"

PreferencesGridPage::PreferencesGridPage(QWidget *parent) : PreferencesPage(parent)
{
    VLayout = new QVBoxLayout();
    HLayout = new QHBoxLayout();
    gridLayout = new QGridLayout();

    addLayout(VLayout);

    VLayout->addSpacing(30);
    VLayout->addLayout(HLayout);
    VLayout->addStretch();

    HLayout->addStretch();
    HLayout->addLayout(gridLayout);
    HLayout->addStretch();
}

void PreferencesGridPage::addRow(ValueWidget *widget)
{
    registerWidget(widget);
    gridLayout->addWidget(widget->coreWidget(), gridLayout->rowCount(), 1);
}

void PreferencesGridPage::addRow(const QString &labelText, ValueWidget *widget)
{
    registerWidget(widget);
    int row = gridLayout->rowCount();
    gridLayout->addWidget(new QLabel(labelText, this), row, 0);
    gridLayout->addWidget(widget->coreWidget(), row, 1);
}