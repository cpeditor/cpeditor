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


#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <type_traits>

namespace Util
{

template <typename T, typename D = T> class Singleton
{
    static_assert(std::is_base_of<T, D>::value, "T should be a base type for D");

  public:
    static T &instance()
    {
        static D inst;
        return inst;
    }

  private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;    
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;
};

} // namespace Util

#endif // SINGLETON_HPP
