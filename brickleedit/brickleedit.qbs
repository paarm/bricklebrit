import qbs
import qbs.FileInfo

Project {
    references: [
        FileInfo.path(path)+"/shared/bricklelib.qbs"
    ]

    Product {
        destinationDirectory: FileInfo.joinPaths(project.sourceDirectory,"bin")
        type: "application"
        name: "brickleedit"
        cpp.includePaths: ["."]
        cpp.staticLibraries: "GLEW"
        files: [
            "src/main.cpp",
            "src/mainwindow/mainwindow.cpp",
            "src/mainwindow/mainwindow.h",
            "src/mainwindow/sceneglwidget.cpp",
            "src/mainwindow/sceneglwidget.h",
            "src/project/projectcontext.cpp",
            "src/project/projectcontext.h",
        ]
        Depends {
            name: "Qt";
            submodules: ["gui", "widgets", "openglextensions"]
        }
        Depends{
            name: "bricklelib"
        }
    }
}
