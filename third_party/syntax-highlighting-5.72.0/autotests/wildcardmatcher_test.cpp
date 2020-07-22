/*
    Copyright (C) 2007 Sebastian Pipping <webmaster@hartwork.org>

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

#include <wildcardmatcher_p.h>

#include <QObject>
#include <qtest.h>

class WildcardMatcherTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testPositiveMatch_data()
    {
        QTest::addColumn<QString>("str");
        QTest::addColumn<QString>("pattern");

        QTest::newRow("*.txt") << "abc.txt"
                               << "*.txt";

        QTest::newRow("*Makefile* 1") << "Makefile.am"
                                      << "*Makefile*";
        QTest::newRow("*Makefile* 2") << "Makefile.am"
                                      << "Makefile*";

        QTest::newRow("control") << "control"
                                 << "control";

        QTest::newRow("a??d") << "abcd"
                              << "a??d";

        QTest::newRow("?") << "a"
                           << "?";
        QTest::newRow("*?*") << "a"
                             << "*?*";
        QTest::newRow("*") << "a"
                           << "*";
        QTest::newRow("**") << "a"
                            << "**";
        QTest::newRow("***") << "a"
                             << "***";

        QTest::newRow("empty 1") << ""
                                 << "*";
        QTest::newRow("empty 2") << ""
                                 << "**";

        QTest::newRow("a*") << "ab"
                            << "a*";
        QTest::newRow("*b") << "ab"
                            << "*b";
        QTest::newRow("a?") << "ab"
                            << "a?";
        QTest::newRow("?b") << "ab"
                            << "?b";

        QTest::newRow("a*b*c") << "aXXbXXbYYaYc"
                               << "a*b*c";
    }

    void testPositiveMatch()
    {
        QFETCH(QString, str);
        QFETCH(QString, pattern);
        QVERIFY(KSyntaxHighlighting::WildcardMatcher::exactMatch(str, pattern));
    }

    void testNegativeMatch_data()
    {
        QTest::addColumn<QString>("str");
        QTest::addColumn<QString>("pattern");

        QTest::newRow("*.cpp") << "abc.txt"
                               << "*.cpp";
        QTest::newRow("*Makefile* 3") << "Makefile.am"
                                      << "Makefile";
        QTest::newRow("?") << ""
                           << "?";
    }

    void testNegativeMatch()
    {
        QFETCH(QString, str);
        QFETCH(QString, pattern);
        QVERIFY(!KSyntaxHighlighting::WildcardMatcher::exactMatch(str, pattern));
    }
};

QTEST_GUILESS_MAIN(WildcardMatcherTest)

#include "wildcardmatcher_test.moc"
