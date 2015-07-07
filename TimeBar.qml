import QtQuick 2.0

Rectangle {

    id: timeBar

    property int fSize: 12
    property string currentValueName: ""

    border.color: Qt.lighter(color)
    border.width: 1

    Text
    {
        anchors.fill: parent
        font.pixelSize: fSize
        font.bold: true
        color: "#FFFFFFFF"
        text:currentValueName
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    Timer
        {
            interval: 1000
            running: true
            repeat: true
            onTriggered:
            {
                currentValueName = new Date().toLocaleString();
            }
        }
}


