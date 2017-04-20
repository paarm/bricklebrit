import qbs

Project {
    Product {
        type: "application"
        name: "brickleedit"
        cpp.includePaths: ["."]
        files: [
            "src/main.cpp",
            "src/mainwindow/mainwindow.cpp",
            "src/mainwindow/mainwindow.h",
            "src/mainwindow/sceneglwidget.cpp",
            "src/mainwindow/sceneglwidget.h"
        ]
        Depends {
            name: "Qt";
            submodules: ["gui", "widgets", "openglextensions"]
        }
    }
}
