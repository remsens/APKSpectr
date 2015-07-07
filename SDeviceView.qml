import QtQuick 2.0


ListView{
        delegate: Rectangle{
        width: 400
        height: 300
        color: "blue"
        SSpectrumView{
            model: spectrumData
        }
    }

}

