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

void PreferencesGridPage::addRow(ValueWidget *widget, const QString &tip, const QString &help, const QString &labelText)
{
    registerWidget(widget);
    int row = gridLayout->rowCount();
    if (labelText.isEmpty())
    {
        if (!tip.isEmpty())
            widget->coreWidget()->setToolTip(tip);
        if (!help.isEmpty())
            widget->coreWidget()->setWhatsThis(help);
    }
    else
    {
        QLabel *label = new QLabel(labelText, this);
        if (!tip.isEmpty())
            label->setToolTip(tip);
        if (!help.isEmpty())
            label->setWhatsThis(help);
        gridLayout->addWidget(label, row, 0);
    }
    gridLayout->addWidget(widget->coreWidget(), row, 1);
}
