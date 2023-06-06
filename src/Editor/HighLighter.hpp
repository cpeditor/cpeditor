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

#ifndef CPEDITOR_HIGHLIGHTER_HPP
#define CPEDITOR_HIGHLIGHTER_HPP

#include <KSyntaxHighlighting/AbstractHighlighter>

#include <KSyntaxHighlighting/State>
#include <QSyntaxHighlighter>
#include <unordered_map>

namespace Editor
{

/**
 * Attribute storage
 */
class Attribute
{
  public:
    /**
     * Attribute constructor
     * @param _offset offset
     * @param _length length
     * @param _attributeValue attribute value
     */
    explicit Attribute(int _offset = 0, int _length = 0, short _attributeValue = 0)
        : offset(_offset), length(_length), attributeValue(_attributeValue)
    {
    }

    int offset;
    int length;
    short attributeValue;
};

class TextBlockUserData : public QTextBlockUserData
{
  public:
    KSyntaxHighlighting::State state;
    QList<KSyntaxHighlighting::FoldingRegion> foldingRegions;
    std::vector<Attribute> attributes;
};

class SyntaxHighlighterPrivate;

/** A QSyntaxHighlighter implementation for use with QTextDocument.
 *  This supports partial re-highlighting during editing and
 *  tracks syntax-based code folding regions.
 *
 *  @since 5.28
 */
class Highlighter : public QSyntaxHighlighter, public KSyntaxHighlighting::AbstractHighlighter
{
    Q_OBJECT
  public:
    explicit Highlighter(QObject *parent = nullptr);
    explicit Highlighter(QTextDocument *document);
    ~Highlighter() override;

    void setDefinition(const KSyntaxHighlighting::Definition &def) override;

    /** Returns whether there is a folding region beginning at @p startBlock.
     *  This only considers syntax-based folding regions,
     *  not indention-based ones as e.g. found in Python.
     *
     *  @see findFoldingRegionEnd
     */
    static bool startsFoldingRegion(const QTextBlock &startBlock);

    /** Finds the end of the folding region starting at @p startBlock.
     *  If multiple folding regions begin at @p startBlock, the end of
     *  the last/innermost one is returned.
     *  This returns an invalid block if no folding region end is found,
     *  which typically indicates an unterminated region and thus folding
     *  until the document end.
     *  This method performs a sequential search starting at @p startBlock
     *  for the matching folding region end, which is a potentially expensive
     *  operation.
     *
     *  @see startsFoldingRegion
     */
    static QTextBlock findFoldingRegionEnd(const QTextBlock &startBlock);

    KSyntaxHighlighting::Format getFormat(int pos);

  protected:
    void highlightBlock(const QString &text) override;
    void applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format) override;
    void applyFolding(int offset, int length, KSyntaxHighlighting::FoldingRegion region) override;

  private:
    static KSyntaxHighlighting::FoldingRegion foldingRegion(const QTextBlock &startBlock);

    QList<KSyntaxHighlighting::FoldingRegion> foldingRegions;

    std::vector<KSyntaxHighlighting::Format> m_formats;
    std::unordered_map<quint16, short> m_formatsIdToIndex;

    std::vector<Attribute> m_attributes;
};

} // namespace Editor
#endif // CPEDITOR_HIGHLIGHTER_HPP
