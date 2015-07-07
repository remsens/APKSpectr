import QtQuick 2.4
import QtQuick.Layouts 1.1

SimpleButton{

    property int series_count: series_edit.text == "" ? 1:series_edit.text
    id: simple_button
    ColumnLayout{

        anchors.fill: parent
    TextInput {
        id: series_edit
        Layout.alignment: Qt.AlignLeft
        Layout.fillWidth: true
        text: qsTr("1")
        color: Qt.lighter(simple_button.color)
        font.pixelSize: simple_button.fontSize
    }
    }


}

