/*
 * multi line comment
 */

// single line comment

import QtQuick 2.0

Rectangle {
    property real myNumProp: 0.1e12;
    property alias sub.color;
    signal mySignal(int arg1, string arg2)
    color: "lightsteelblue"
    width: 320
    height: width/2

    Rectangle {
        id: sub
        width: 0x10
        height: 007
        objectName: 'single quote'
        objectName2: a ? b : c;
    }
}
