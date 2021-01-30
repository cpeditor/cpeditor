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

#ifndef TESTCASESCOPYPASTER_HPP
#define TESTCASESCOPYPASTER_HPP

#include "Util/Singleton.hpp"
#include <QStringList>

namespace Widgets
{
class TestCases;
}

class TestCasesCopyPaster : public Util::Singleton<TestCasesCopyPaster>
{
  public:
    /**
     * @brief copy checked test cases in *testcases*
     * @param testcases the TestCases to be copied from
     */
    void copy(Widgets::TestCases *testcases);

    /**
     * @brief append copied test cases to *testcases*
     * @param testcases the TestCases to be pasted to
     */
    void paste(Widgets::TestCases *testcases) const;

  private:
    QStringList inputs, expecteds;
};

#endif // TESTCASESCOPYPASTER_HPP
