import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import QtPositioning 5.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2


Window{
    id: main_window
    width: 1024
    height: 768
    color: "#FF4A768F"
    visible: true
    //visibility: "FullScreen"
    signal onShootPress();
    signal onSeriesPress(var shotCount);
    signal onStopPress();
    signal onSavePress();

    GridLayout
    {
        columns: 3
        rows: 3
        anchors.fill: parent
    TimeBar
    {
        id: time_bar
        y: 360
        height: 38
        //width: 100
        fSize: 20
        color: "#88792015"
        Layout.column: 0
        Layout.row: 0
        Layout.columnSpan: 3
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.maximumHeight: 38
        Layout.minimumHeight: 38
    }

    GridView
        {
            Layout.columnSpan: 2
            id: devices_view
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: width
            cellHeight: height

            model: spectrum_model
            delegate: Rectangle
                    {
                    width: devices_view.cellWidth - 20
                    height: devices_view.cellHeight - 20
                    anchors.margins: 10
                    color: "#88A62B0F"
                    border.width: 1
                    border.color: Qt.lighter(color);
                    Text{
                        width: parent.width
                        height: 30
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: deviceName + " " + devicePosition
                        font.bold: true
                        font.pixelSize: 20
                        color: "white"
                    }

                ListView{
                    id: level2
                    anchors.fill: parent
                    anchors.margins: 10
                    anchors.topMargin: 30
                    spacing: 10
                    model:deviceRole
                    clip: true
                    onCountChanged:
                    {
                        level2.incrementCurrentIndex();
                    }

                    delegate:
                        Image{      id: underImage
                        clip: true
                                    width: level2.width
                                    height: level2.height
                                    //source: "image://apkimage/" + index
                                    Rectangle{
                                         anchors.fill: parent
                                         anchors.margins: 10
                                         color:"#99FF8935"
                                         Text{
                                             anchors.fill: parent
                                             verticalAlignment: Text.AlignTop
                                             horizontalAlignment: Text.AlignHCenter
                                             text: "Спектр №" + index + " экспозиция: " + spectrumExpo + "мс время: " + spectrumTime + " " + spectrumPosition;
                                             //text: "Спектр №" + index + " время: " + spectrumTime + " " + spectrumPosition;
                                             wrapMode: Text.WordWrap
                                             font.bold: true
                                             font.pixelSize: 16
                                             color: "white"
                                         }
                                    }
                                    Canvas
                                    {
                                        anchors.fill: parent
                                        anchors.margins: 10
                                        onPaint:
                                        {
                                            var ctx = getContext("2d");
                                            ctx.lineWidth = 1
                                            ctx.strokeStyle = "white"
                                            ctx.beginPath()
                                            ctx.moveTo(0, h - spectrumData.getYat(0) * (h / 4096))
                                            var cstep = 1//3648 / parent.width;
                                            var h = level2.height
                                            var cydata = spectrumData.getYData();
                                            for(var i=1; i < 3648; i += cstep)
                                            {
                                                cydata[i] = h-cydata[i]*(h/4096);
                                                ctx.lineTo(i *(parent.width / 3648), cydata[i]);
                                            }
                                            ctx.stroke()
                                        }
                                    }

                         } //spectrum view
                         } // subgrid spectrum
                      }// grid delegate
                 } //end of grid

    ColumnLayout
    {
    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.maximumWidth: 250
    SimpleButton{
        color: "#8876f310"
        buttonCaption: "Снимок"
        Layout.fillWidth: true
        Layout.fillHeight: true
        onButtonClicked: {
            if (exposition_mode.checked)
            {spectrum_model.onShotButPressed(exposition_edit.text);}
            else
            {spectrum_model.onShotButPressed("0")}
        }

    }
    SeriesButton{
        color: "#8876f310"
        Layout.fillWidth: true;
        Layout.fillHeight: true;
        buttonCaption: "Серия"
        onButtonClicked: {
            if (exposition_mode.checked)
            {spectrum_model.onSeriesButPressed(series_count, exposition_edit.text);}
            else
            {spectrum_model.onSeriesButPressed(series_count,"0")}
        }
    }

    RowLayout
    {
        Layout.minimumHeight: 100
    CheckBox
      {
          id: exposition_mode
          Layout.fillWidth: true
          text: "Экспозиция"
          antialiasing: true
          checked: false
          style: CheckBoxStyle{
              label:
                  Text{
                  text: exposition_mode.text
                  color: "white"
                  font.pixelSize: 20
                  font.bold: true
                  wrapMode: Text.WordWrap
              }    
          }
      }
    TextInput{
        id: exposition_edit
        text: "50"
        color: "white"
        font.pixelSize: 20
        font.bold: true
    }
    }

    SimpleButton{
        color: "#88fd7a39"
        buttonCaption: "Cохранить"
        Layout.fillWidth: true
        Layout.fillHeight: true
//        FileDialog{
//            id: f_dialog
//            selectExisting: false
//            selectMultiple: false
//            visible: false
//            nameFilters: ["Spectrum files (*.spec)"]
//            onAccepted:
//            {
//                spectrum_model.onSavePressed(fileUrl);
//        }
//        }

        onButtonClicked: {
            spectrum_model.onSavePressed("1");
            Dialog
            //f_dialog.open();
        }

    }
    SimpleButton{
        color: "#88ff0000"
        buttonCaption: "Выход"
        Layout.fillWidth: true
        Layout.fillHeight: true
        onButtonClicked: {
            Qt.quit();

        }

    }

    }

}
}
