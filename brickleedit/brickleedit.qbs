import qbs

Project {
    Product {
        type: "application"
        name: "brickleedit"
        cpp.includePaths: ["."]
        files: [
            "main.cpp",
            "mainwindow.cpp",
            "mainwindow.h",
            "mainwindow.ui",
            "myopenglwidget.cpp",
            "myopenglwidget.h"
        ]
        Depends {
            name: "Qt";
            submodules: ["gui", "widgets", "openglextensions"]
        }
    }
}
