import QtQuick 2.0

Repeater {
   id: spectrum_data_view
   //qspectrummodel
   anchors.margins: 3
   clip: true
   antialiasing: true
   delegate: Rectangle {
                width: 1
                height: 1
                x: xValue  / (3648.0 / spectrum_data_view.width)
                y: spectrum_data_view.height - (yValue / (4096.0 / spectrum_data_view.height))
                        }
         }

