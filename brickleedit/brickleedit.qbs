import qbs
import qbs.FileInfo

Project {
    references: [
        FileInfo.path(path)+"/shared/bricklelib.qbs"
    ]

    Product {
        //destinationDirectory: FileInfo.joinPaths(project.sourceDirectory,"bin")
        type: "application"
        name: "brickleedit"
        cpp.includePaths: ["."]
        //cpp.staticLibraries: "GLEW"
        files: [
            "src/gui/guicontext.cpp",
            "src/gui/guicontext.h",
            "src/gui/menubar.cpp",
            "src/gui/menubar.h",
            "src/gui/newproject/newprojectdialog.cpp",
            "src/gui/newproject/newprojectdialog.h",
            "src/gui/newproject/newprojectdialog.ui",
            "src/gui/newscene/newscenedialog.cpp",
            "src/gui/newscene/newscenedialog.h",
            "src/gui/newscene/newscenedialog.ui",
            "src/gui/propertygrid/iproperty.h",
            "src/gui/propertygrid/propertyeditdelegate.cpp",
            "src/gui/propertygrid/propertyeditdelegate.h",
            "src/gui/propertygrid/propertyinfo.cpp",
            "src/gui/propertygrid/propertyinfo.h",
            "src/gui/propertygrid/propertytreedock.cpp",
            "src/gui/propertygrid/propertytreedock.h",
            "src/gui/propertygrid/propertytreedock.ui",
            "src/gui/resources/resource.qrc",
            "src/gui/scenetree/newscenenode/newscenenodedialog.cpp",
            "src/gui/scenetree/newscenenode/newscenenodedialog.h",
            "src/gui/scenetree/newscenenode/newscenenodedialog.ui",
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
            "src/project/nodeinfo.cpp",
            "src/project/nodeinfo.h",
            "src/project/nodeinfotype.h",
            "src/project/nodemanager.cpp",
            "src/project/nodemanager.h",
            "src/project/projectcontext.cpp",
            "src/project/projectcontext.h",
        ]
        Depends {
            name: "Qt";
            submodules: ["gui", "opengl", "widgets", "openglextensions"]
        }
        Depends{
            name: "bricklelib"
        }
    }
}
