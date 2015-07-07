import QtQuick 2.4

Rectangle {
    id: s_button
    signal buttonClicked()
    property string buttonCaption: "button"
    property int fontSize: 20
    border.color:  Qt.lighter(color);
    border.width: 1
    Text{
        anchors.fill: parent
        text: buttonCaption
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: Qt.lighter(parent.color)
//        elide: Text.ElideRight
//        fontSizeMode: Text.Fit
        font.pixelSize: fontSize
        font.bold: true
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked: {
            buttonClicked()
        }
    }

}

