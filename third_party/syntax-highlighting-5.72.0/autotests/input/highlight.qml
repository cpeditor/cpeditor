import Qt 4.6

// some random qml snippets stolen from the qt docs

Rectangle {
    important: true
    propertyAsdf:
    id: container
    signalA: bla
    property string label
    signal clicked
    radius: 5; border.color: "black"
    color: mouse.pressed ? "steelblue" : "lightsteelblue"
    gradient: Gradient {
            GradientStop { position: mouse.pressed ? 1.0 : 0.0; color: "steelblue" }
            GradientStop { position: mouse.pressed ? 0.0 : 1.0; color: "lightsteelblue" }
    }
    MouseRegion { id: mouse; anchors.fill: parent; onClicked: container.clicked() }
    Text { anchors.fill: parent; text: container.label; anchors.centerIn: parent }
}

Rectangle {
    Script {
        function calculateMyHeight() {
            return Math.max(otherItem.height, thirdItem.height);
        }
    }

    anchors.centerIn: parent
    width: Math.min(otherItem.width, 10)
    height: calculateMyHeight()
    color: { if (width > 10) "blue"; else "red" }
}

Rectangle {
    default property color innerColor: "black"
    property color innerColor: "black"
    property alias text: textElement.text
    property alias aliasXYZ: testElement.text
    signal bar
    signal bar(var blah, string yxcv)
    width: 240; height: 320;
    width: 100; height: 30; source: "images/toolbutton.sci"
    ListView {
        anchors.fill: parent
        model: contactModel
        delegate: Component {
            Text {
                text: modelData.firstName + " " + modelData.lastName
            }
        }
    }
}

Item {
    function say(text) {
        console.log("You said " + text);
    }
}
