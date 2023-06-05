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

/* Some codes are from the examples of KDE/syntax-highlighting, which are licensed under the MIT License:

    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "HighLighter.hpp"

#include <KSyntaxHighlighting/AbstractHighlighter>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/FoldingRegion>
#include <KSyntaxHighlighting/Format>
#include <QTextDocument>
#include <utility>

Q_DECLARE_METATYPE(QTextBlock)

namespace KSH = KSyntaxHighlighting;

namespace Editor
{

KSH::FoldingRegion Highlighter::foldingRegion(const QTextBlock &startBlock)
{
    const auto data = dynamic_cast<TextBlockUserData *>(startBlock.userData());
    if (!data)
    {
        return KSH::FoldingRegion();
    }
    for (int i = data->foldingRegions.size() - 1; i >= 0; --i)
    {
        if (data->foldingRegions.at(i).type() == KSH::FoldingRegion::Begin)
        {
            return data->foldingRegions.at(i);
        }
    }
    return KSH::FoldingRegion();
}

Highlighter::Highlighter(QObject *parent) : QSyntaxHighlighter(parent)
{
    qRegisterMetaType<QTextBlock>();
}

Highlighter::Highlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{
    qRegisterMetaType<QTextBlock>();
}

Highlighter::~Highlighter() = default;

void Highlighter::setDefinition(const KSyntaxHighlighting::Definition &def)
{
    m_formatsIdToIndex.clear();
    AbstractHighlighter::setDefinition(def);

    auto definitions = def.includedDefinitions();

    if (!def.isValid() || (definitions.isEmpty() && def.formats().isEmpty()))
    {
        // dummy properties + formats
        m_formats.resize(1);
        m_formatsIdToIndex.insert(std::make_pair(m_formats[0].id(), 0));

        // be done, all below is just for the real highlighting variants
        return;
    }

    for (const auto &includedDefinition : definitions)
    {
        const auto formats = includedDefinition.formats();
        for (const auto &format : formats)
        {
            // register format id => internal attributes, we want no clashs
            const auto nextId = m_formats.size();
            m_formatsIdToIndex.insert(std::make_pair(format.id(), short(nextId)));
            m_formats.push_back(format);
        }
    }

    rehighlight();
}

bool Highlighter::startsFoldingRegion(const QTextBlock &startBlock) const
{
    return foldingRegion(startBlock).type() == KSH::FoldingRegion::Begin;
}

QTextBlock Highlighter::findFoldingRegionEnd(const QTextBlock &startBlock) const
{
    const auto region = foldingRegion(startBlock);

    auto block = startBlock;
    int depth = 1;
    while (block.isValid())
    {
        block = block.next();
        const auto data = dynamic_cast<TextBlockUserData *>(block.userData());
        if (!data)
        {
            continue;
        }
        for (auto it = data->foldingRegions.constBegin(); it != data->foldingRegions.constEnd(); ++it)
        {
            if ((*it).id() != region.id())
            {
                continue;
            }
            if ((*it).type() == KSH::FoldingRegion::End)
            {
                --depth;
            }
            else if ((*it).type() == KSH::FoldingRegion::Begin)
            {
                ++depth;
            }
            if (depth == 0)
            {
                return block;
            }
        }
    }

    return QTextBlock();
}

void Highlighter::highlightBlock(const QString &text)
{

    KSH::State state;
    if (currentBlock().position() > 0)
    {
        const auto prevBlock = currentBlock().previous();
        const auto prevData = dynamic_cast<TextBlockUserData *>(prevBlock.userData());
        if (prevData)
        {
            state = prevData->state;
        }
    }
    foldingRegions.clear();
    m_attributes.clear();

    state = highlightLine(text, state);

    auto data = dynamic_cast<TextBlockUserData *>(currentBlockUserData());
    if (!data)
    { // first time we highlight this
        data = new TextBlockUserData;
        data->state = state;
        data->foldingRegions = foldingRegions;
        data->attributes = m_attributes;
        setCurrentBlockUserData(data);
        return;
    }

    data->attributes = m_attributes;

    if (data->state == state && data->foldingRegions == foldingRegions)
    { // we ended up in the same state, so we are done here
        return;
    }
    data->state = state;
    data->foldingRegions = foldingRegions;

    const auto nextBlock = currentBlock().next();
    if (nextBlock.isValid())
    {
        QMetaObject::invokeMethod(this, "rehighlightBlock", Qt::QueuedConnection, Q_ARG(QTextBlock, nextBlock));
    }
}

void Highlighter::applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format)
{
    if (length == 0)
    {
        return;
    }

    QTextCharFormat tf;
    // always set the foreground color to avoid palette issues
    tf.setForeground(format.textColor(theme()));

    if (format.hasBackgroundColor(theme()))
    {
        tf.setBackground(format.backgroundColor(theme()));
    }
    if (format.isBold(theme()))
    {
        tf.setFontWeight(QFont::Bold);
    }
    if (format.isItalic(theme()))
    {
        tf.setFontItalic(true);
    }
    if (format.isUnderline(theme()))
    {
        tf.setFontUnderline(true);
    }
    if (format.isStrikeThrough(theme()))
    {
        tf.setFontStrikeOut(true);
    }

    const auto it = m_formatsIdToIndex.find(format.id());
    //    Q_ASSERT(it != m_formatsIdToIndex.end());

    if (it != m_formatsIdToIndex.end())
    {
        m_attributes.emplace_back(offset, length, it->second);
    }

    QSyntaxHighlighter::setFormat(offset, length, tf);
}

void Highlighter::applyFolding(int offset, int length, KSH::FoldingRegion region)
{
    Q_UNUSED(offset);
    Q_UNUSED(length);

    if (region.type() == KSH::FoldingRegion::Begin)
    {
        foldingRegions.push_back(region);
    }

    if (region.type() == KSH::FoldingRegion::End)
    {
        for (int i = foldingRegions.size() - 1; i >= 0; --i)
        {
            if (foldingRegions.at(i).id() != region.id() || foldingRegions.at(i).type() != KSH::FoldingRegion::Begin)
            {
                continue;
            }
            foldingRegions.removeAt(i);
            return;
        }
        foldingRegions.push_back(region);
    }
}

KSH::Format Highlighter::getFormat(int pos)
{
    auto block = document()->findBlock(pos);
    const auto *data = dynamic_cast<TextBlockUserData *>(block.userData());
    pos -= block.position();
    auto &attr = data->attributes;
    auto found = std::upper_bound(attr.cbegin(), attr.cend(), pos,
                                  [](const int &p, const Attribute &x) { return p < x.offset + x.length; });
    if (found != attr.cend() && found->offset <= pos && pos < (found->offset + found->length))
    {
        return m_formats[found->attributeValue];
    }
    return m_formats.front();
}

} // namespace Editor
