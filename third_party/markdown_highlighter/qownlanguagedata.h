/*
 * Copyright (c) 2019-2020 Waqar Ahmed -- <waqar.17a@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 */
#ifndef QOWNLANGUAGEDATA_H
#define QOWNLANGUAGEDATA_H

#include <QMultiHash>

/* ------------------------
 * TEMPLATE FOR LANG DATA
 * -------------------------
 *
 * loadXXXData, where XXX is the language
 * keywords are the language keywords e.g, const
 * types are built-in types i.e, int, char, var
 * literals are words like, true false
 * builtin are the library functions
 * other can contain any other thing, for e.g, in cpp it contains the
 preprocessor

    static const QMultiHash<char, QLatin1String> xxx_keywords = {
    };

    static const QMultiHash<char, QLatin1String> xxx_types = {
    };

    static const QMultiHash<char, QLatin1String> xxx_literals = {
    };

    static const QMultiHash<char, QLatin1String> xxx_builtin = {
    };

    static const QMultiHash<char, QLatin1String> xxx_other = {
    };

*/

/**********************************************************/
/* C/C++ Data *********************************************/
/**********************************************************/
void loadCppData(QMultiHash<char, QLatin1String> &typess, QMultiHash<char, QLatin1String> &keywordss,
                 QMultiHash<char, QLatin1String> &builtins, QMultiHash<char, QLatin1String> &literalss,
                 QMultiHash<char, QLatin1String> &others);

/**********************************************************/
/* Shell Data *********************************************/
/**********************************************************/
void loadShellData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                   QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                   QMultiHash<char, QLatin1String> &other);

/**********************************************************/
/* JS Data *********************************************/
/**********************************************************/
void loadJSData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                QMultiHash<char, QLatin1String> &other);

/**********************************************************/
/* PHP Data *********************************************/
/**********************************************************/
void loadPHPData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other);

/**********************************************************/
/* QML Data *********************************************/
/**********************************************************/
void loadQMLData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other);

/**********************************************************/
/* Python Data *********************************************/
/**********************************************************/
void loadPythonData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                    QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                    QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   Rust DATA      ***********************************/
/********************************************************/
void loadRustData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   Java DATA      ***********************************/
/********************************************************/
void loadJavaData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   C# DATA      *************************************/
/********************************************************/
void loadCSharpData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                    QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                    QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   Go DATA      *************************************/
/********************************************************/
void loadGoData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   V DATA      **************************************/
/********************************************************/
void loadVData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
               QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
               QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   SQL DATA      ************************************/
/********************************************************/
void loadSQLData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   JSON DATA      ***********************************/
/********************************************************/
void loadJSONData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   CSS DATA      ***********************************/
/********************************************************/
void loadCSSData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   Typescript DATA  *********************************/
/********************************************************/
void loadTypescriptData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                        QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                        QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   YAML DATA  ***************************************/
/********************************************************/
void loadYAMLData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   VEX DATA  ****************************************/
/********************************************************/
void loadVEXData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                 QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                 QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   CMake DATA  **************************************/
/********************************************************/
void loadCMakeData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                   QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                   QMultiHash<char, QLatin1String> &other);

/********************************************************/
/***   Make DATA  ***************************************/
/********************************************************/
void loadMakeData(QMultiHash<char, QLatin1String> &types, QMultiHash<char, QLatin1String> &keywords,
                  QMultiHash<char, QLatin1String> &builtin, QMultiHash<char, QLatin1String> &literals,
                  QMultiHash<char, QLatin1String> &other);

#endif
