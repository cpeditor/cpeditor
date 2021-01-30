/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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

#include "Widgets/SupportUsDialog.hpp"
#include "Core/EventLogger.hpp"
#include "Core/Translator.hpp"
#include "Util/FileUtil.hpp"
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

SupportUsDialog::SupportUsDialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Like CP Editor?"));

    auto *mainLayout = new QVBoxLayout(this);

    textBrowser = new QTextBrowser();
    mainLayout->addWidget(textBrowser);

    textBrowser->setFrameStyle(QFrame::NoFrame);
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setMarkdown(
        QString(R"(%1
-   <a href="https://github.com/cpeditor/cpeditor/stargazers">
      <img src=":/donate/star.png"/>
    </a>
    %2
-   <a href="https://twitter.com/intent/tweet?text=&amp;hashtags=CPEditor,CompetitiveProgramming&amp;url=https://cpeditor.org">
      <img src=":/donate/tweet.png"/>
    </a>
    %3
-   <a href="#donate">
      <img src=":/donate/sponsor.png"/>
    </a>
    %4

%5)")
            .arg(tr("Thank you for using CP Editor! To support us, you can:"))
            .arg(tr("Give us a star on GitHub"))
            .arg(tr("Share CP Editor with your friends"))
            .arg(tr("Financially support us"))
            .arg(tr("Or, [provide some suggestions](https://github.com/cpeditor/cpeditor/issues/new/choose) to help us "
                    "do better."))
            // this must be the last one, because the percent encoding includes %1, %2, etc., which breaks QString::arg
            .replace("?text=", "?text=" + QString::fromUtf8(QUrl::toPercentEncoding(
                                              tr("I'm using @cpeditor_, an IDE specially designed for competitive "
                                                 "programmers, which is awesome!")))));

    setMinimumHeight(textBrowser->height());
    setMinimumWidth(textBrowser->width());

    connect(textBrowser, &QTextBrowser::anchorClicked, this, &SupportUsDialog::onAnchorClicked);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &QDialog::accept);
}

void SupportUsDialog::onAnchorClicked(const QUrl &url)
{
    if (url.toString() == "#donate")
    {
        auto *dialog = new QMessageBox(this);
        dialog->setTextFormat(Qt::MarkdownText);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setModal(true);
        dialog->setWindowTitle(tr("Donate"));
        dialog->setText(
            Util::readFile(QString(":/DONATE%1.md").arg(Core::Translator::langSuffix())).replace("resources/", ":/"));
        dialog->show();
    }
}
