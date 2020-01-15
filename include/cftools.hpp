/*
* Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com> 
* 
* This file is part of CPEditor.
*  
* CPEditor is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* I will not be responsible if CPEditor behaves in unexpected way and
* causes your ratings to go down and or loose any important contest.
* 
* Believe Software is "Software" and it isn't immune to bugs.
* 
*/


#ifndef CFTOOLS_HPP
#define CFTOOLS_HPP

#include <QProcess>
#include <BaseFiles.hpp>
#include <MessageLogger.hpp>
namespace Network
{
class CFTools : public QObject, private Core::Base::Files
{
    Q_OBJECT

  public:
    CFTools(int index, MessageLogger* logger);

    void submit(QString url, QString lang);
    void killProcess();

    static bool check();
    ~CFTools();

private slots:
    void onReadReady();

  private:
    QProcess *cftool = nullptr;
    MessageLogger* log;
    int index;
};
} // namespace Network
#endif // CFTOOLS_HPP
