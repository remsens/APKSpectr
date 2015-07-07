import qbs

Project
{
CppApplication {
    type: "application" // To suppress bundle generation on Mac
    name: "APKSpectr"
    consoleApplication: false
    Depends {name: "cpp"}
    Depends {name: "Qt.core"}
    Depends {name: "Qt.quick"}
    Depends {name: "Qt.qml"}
    Depends {name: "Qt.widgets"}
    Depends {name: "Qt.serialport"}
    Depends {name: "Qt.positioning"}
    Depends {name: "Qt.testlib"}
    Depends {name: "Qt.network"}
    Depends {name: "Qt.gui"}
    cpp.libraryPaths: ["./lib"]
    cpp.staticLibraries: ["ftd2xx"]
    cpp.includePaths:["./include"]
    cpp.defines: ["QT_QML_DEBUG"]
    destinationDirectory: qbs.buildVariant == "release" ? sourceDirectory + "/bin/release": sourceDirectory + "/bin/debug"

    files: [
        "MainWin.qml",
        "SDeviceView.qml",
        "SSpectrumView.qml",
        "SeriesButton.qml",
        "SimpleButton.qml",
        "TimeBar.qml",
        "main.cpp",
        "qapkengine.cpp",
        "qapkengine.h",
        "qapkimageloader.cpp",
        "qapkimageloader.h",
        "qapkimageprovider.cpp",
        "qapkimageprovider.h",
        "qdevicedata.cpp",
        "qdevicedata.h",
        "qdevicedatamodel.cpp",
        "qdevicedatamodel.h",
        "qspecmodule.cpp",
        "qspecmodule.h",
        "qspectrumdevices.cpp",
        "qspectrumdevices.h",
        "qspectrummodel.cpp",
        "qspectrummodel.h",
    ]

}
}

