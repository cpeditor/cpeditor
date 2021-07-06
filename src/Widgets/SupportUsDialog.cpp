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
#include <QDesktopServices>
#include <QLabel>
#include <QMessageBox>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>

SupportEntry::SupportEntry(const QString &text, const QString &icon, QString url, QWidget *parent)
    : QWidget(parent), url(std::move(url))
{
    auto *layout = new QHBoxLayout(this);

    auto *l = new QLabel(text);
    l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(l);

    layout->addStretch();

    auto *i = new QToolButton();
    i->setAttribute(Qt::WA_TranslucentBackground, true);
    i->setStyleSheet("background-color: rgba(0, 0, 0, 0%)");
    i->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon ic(icon);
    i->setIcon(ic);
    i->setIconSize(QSize(fontMetrics().height(), fontMetrics().height()));
    connect(i, &QToolButton::clicked, this, &SupportEntry::onLinkClicked);
    layout->addWidget(i);
}

void SupportEntry::onLinkClicked()
{
    if (url == "#donate")
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
    else
    {
        QDesktopServices::openUrl(url);
    }
}

SupportUsDialog::SupportUsDialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Like CP Editor?"));

    auto *mainLayout = new QVBoxLayout(this);

    auto *titleLayout = new QHBoxLayout;
    mainLayout->addLayout(titleLayout);

    auto *title = new QLabel(tr("Thank you for using CP Editor!"));
    auto f = title->font();
    f.setPointSizeF(font().pointSizeF() * 1.5);
    title->setFont(f);
    titleLayout->addStretch();
    titleLayout->addWidget(title);
    titleLayout->addStretch();

    auto *subtitleLayout = new QHBoxLayout;
    mainLayout->addLayout(subtitleLayout);

    auto *subt = new QLabel(tr("To support us, you can:"));
    subtitleLayout->addStretch();
    subtitleLayout->addWidget(subt);
    subtitleLayout->addStretch();

    auto *star = new SupportEntry(tr("Give us a star on GitHub"), ":/donate/star.svg",
                                  "https://github.com/cpeditor/cpeditor/stargazers");
    mainLayout->addWidget(star);

    auto *tweet = new SupportEntry(
        tr("Share CP Editor with your friends"), ":/donate/twitter.svg",
        QString("https://twitter.com/intent/tweet?text=%1&amp;hashtags=CPEditor,CompetitiveProgramming&amp;url=https://"
                "cpeditor.org")
            .arg(QString::fromUtf8(QUrl::toPercentEncoding(tr(
                "I'm using @cpeditor_, an IDE specially designed for competitive programmers, which is awesome!")))));
    mainLayout->addWidget(tweet);

    auto *donate = new SupportEntry(tr("Financially support us"), ":/donate/heart.svg", "#donate");
    mainLayout->addWidget(donate);

    auto *issue = new SupportEntry(tr("Provide some suggestions to help us do better"), ":/donate/wrench.svg",
                                   "https://github.com/cpeditor/cpeditor/issues/new/choose");
    mainLayout->addWidget(issue);
}
