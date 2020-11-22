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

#include "Core/TestCasesCopyPaster.hpp"
#include "Core/EventLogger.hpp"
#include "Widgets/TestCases.hpp"

void TestCasesCopyPaster::copy(Widgets::TestCases *testcases)
{
    LOG_INFO("Copy");
    inputs = testcases->inputs();
    expecteds = testcases->expecteds();
}

void TestCasesCopyPaster::paste(Widgets::TestCases *testcases) const
{
    LOG_INFO("Paste");
    for (int i = 0; i < inputs.count() && i < expecteds.count(); ++i)
    {
        testcases->addTestCase(inputs[i], expecteds[i]);
    }
}
