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
            "src/gui/guicontext.cpp",
            "src/gui/guicontext.h",
            "src/gui/newproject/newprojectdialog.cpp",
            "src/gui/newproject/newprojectdialog.h",
            "src/gui/newproject/newprojectdialog.ui",
            "src/gui/resources/resource.qrc",
            "src/gui/scenetree/scenetreedock.cpp",
            "src/gui/scenetree/scenetreedock.h",
            "src/gui/scenetree/scenetreedock.ui",
            "src/gui/toolbar.cpp",
            "src/gui/toolbar.h",
            "src/main.cpp",
            "src/gui/mainwindow.cpp",
            "src/gui/mainwindow.h",
            "src/gui/sceneglwidget.cpp",
            "src/gui/sceneglwidget.h",
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
