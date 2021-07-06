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
#include <QPixmap>
#include <QPushButton>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>

SupportEntry::SupportEntry(const QString &text, const QString &icon, QString url, QWidget *parent)
    : QWidget(parent), url(url)
{
	auto *layout = new QHBoxLayout;
    setLayout(layout);

    auto *l = new QLabel(text, this);
    l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(l);

    layout->addStretch();

    auto *i = new QToolButton(this);
    i->setAttribute(Qt::WA_TranslucentBackground, true);
    i->setStyleSheet("background-color: rgba(0, 0, 0, 0%)");
    i->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon ic(icon);
    i->setIcon(ic);
	i->setIconSize(QSize(24, 24));
    connect(i, &QToolButton::clicked, [this]() { emit clicked(this->url); });
    layout->addWidget(i);
}

SupportUsDialog::SupportUsDialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Like CP Editor?"));

    auto *mainLayout = new QVBoxLayout(this);

    auto *titleLayout = new QHBoxLayout;
    mainLayout->addLayout(titleLayout);

    auto *title = new QLabel(tr("Thank you for using CP Editor!"), this);
    auto f = title->font();
    f.setPixelSize(24);
    title->setFont(f);
    titleLayout->addStretch();
    titleLayout->addWidget(title);
    titleLayout->addStretch();

    auto *subtitleLayout = new QHBoxLayout;
    mainLayout->addLayout(subtitleLayout);

    auto *subt = new QLabel(tr("To support us, you can:"), this);
    subtitleLayout->addStretch();
    subtitleLayout->addWidget(subt);
    subtitleLayout->addStretch();

    auto *star = new SupportEntry(tr("Give us a star on GitHub"), ":/donate/star.svg",
                                  "https://github.com/cpeditor/cpeditor/stargazers", this);
    mainLayout->addWidget(star);
    connect(star, &SupportEntry::clicked, this, &SupportUsDialog::onAnchorClicked);

    auto *tweet = new SupportEntry(
        tr("Share CP Editor with your friends"), ":/donate/twitter.svg",
        QString("https://twitter.com/intent/tweet?text=%1&amp;hashtags=CPEditor,CompetitiveProgramming&amp;url=https://"
                "cpeditor.org")
            .arg(QString::fromUtf8(QUrl::toPercentEncoding(
                tr("I'm using @cpeditor_, an IDE specially designed for competitive programmers, which is awesome!")))),
        this);
    mainLayout->addWidget(tweet);
    connect(tweet, &SupportEntry::clicked, this, &SupportUsDialog::onAnchorClicked);

    auto *donate = new SupportEntry(tr("Financially support us"), ":/donate/heart.svg", "#donate", this);
    mainLayout->addWidget(donate);
    connect(donate, &SupportEntry::clicked, this, &SupportUsDialog::onAnchorClicked);

    auto *issue = new SupportEntry(tr("Provide some suggestions to help us do better"), ":/donate/wrench.svg",
                                   "https://github.com/cpeditor/cpeditor/issues/new/choose", this);
    mainLayout->addWidget(issue);
    connect(issue, &SupportEntry::clicked, this, &SupportUsDialog::onAnchorClicked);
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
    else
    {
        QDesktopServices::openUrl(url);
    }
}
