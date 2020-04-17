#include "Settings/PreferencesGridPage.hpp"

PreferencesGridPage::PreferencesGridPage(bool alignTop, QWidget *parent) : PreferencesPage(parent)
{
    VLayout = new QVBoxLayout();
    HLayout = new QHBoxLayout();
    gridLayout = new QGridLayout();

    addLayout(VLayout);

    VLayout->addSpacing(30);
    VLayout->addLayout(HLayout);
    if (alignTop)
        VLayout->addStretch();
    VLayout->addSpacing(30);

    HLayout->addStretch();
    HLayout->addLayout(gridLayout);
    HLayout->addStretch();
}

void PreferencesGridPage::addRow(ValueWidget *widget)
{
    registerWidget(widget);
    gridLayout->addWidget(widget->coreWidget(), gridLayout->rowCount(), 1);
}

void PreferencesGridPage::addRow(const QString &labelText, const QString &labelTip, const QString &labelHelp, ValueWidget *widget)
{
    registerWidget(widget);
    int row = gridLayout->rowCount();
    QLabel *label = new QLabel(labelText, this);
    if (!labelTip.isEmpty())
        label->setToolTip(labelTip);
    if (!labelHelp.isEmpty())
        label->setWhatsThis(labelHelp);
    gridLayout->addWidget(label, row, 0);
    gridLayout->addWidget(widget->coreWidget(), row, 1);
}