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
            "src/gui/animationframeeditor/animationframeeditor.cpp",
            "src/gui/animationframeeditor/animationframeeditor.h",
            "src/gui/animationframeeditor/animationframeeditor.ui",
            "src/gui/brushmanager/brushdock.cpp",
            "src/gui/brushmanager/brushdock.h",
            "src/gui/brushmanager/brushdock.ui",
            "src/gui/brushmanager/brushselection.cpp",
            "src/gui/brushmanager/brushselection.h",
            "src/gui/brushmanager/brushselection.ui",
            "src/gui/grideditor/grideditor.cpp",
            "src/gui/grideditor/grideditor.h",
            "src/gui/grideditor/grideditor.ui",
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
            "src/gui/previewimageutil.cpp",
            "src/gui/previewimageutil.h",
            "src/gui/propertygrid/iproperty.h",
            "src/gui/propertygrid/propertyeditdelegate.cpp",
            "src/gui/propertygrid/propertyeditdelegate.h",
            "src/gui/propertygrid/propertyinfo.cpp",
            "src/gui/propertygrid/propertyinfo.h",
            "src/gui/propertygrid/propertytreedock.cpp",
            "src/gui/propertygrid/propertytreedock.h",
            "src/gui/propertygrid/propertytreedock.ui",
            "src/gui/resources/resource.qrc",
            "src/gui/scene/sceneglwidget.cpp",
            "src/gui/scene/sceneglwidget.h",
            "src/gui/scene/sceneitemmovemanager.cpp",
            "src/gui/scene/sceneitemmovemanager.h",
            "src/gui/scene/sceneitemresizemanager.cpp",
            "src/gui/scene/sceneitemresizemanager.h",
            "src/gui/scene/worldcalculator.cpp",
            "src/gui/scene/worldcalculator.h",
            "src/gui/scenetree/scenetreedock.cpp",
            "src/gui/scenetree/scenetreedock.h",
            "src/gui/scenetree/scenetreedock.ui",
            "src/gui/selectionmanager/selectionmanager.cpp",
            "src/gui/selectionmanager/selectionmanager.h",
            "src/gui/spriteeditor/spriteeditor.cpp",
            "src/gui/spriteeditor/spriteeditor.h",
            "src/gui/spriteeditor/spriteeditor.ui",
            "src/gui/textureframeeditor/textureframeeditor.cpp",
            "src/gui/textureframeeditor/textureframeeditor.h",
            "src/gui/textureframeeditor/textureframeeditor.ui",
            "src/gui/texturepicker/selecteditem.h",
            "src/gui/texturepicker/texturepicker.cpp",
            "src/gui/texturepicker/texturepicker.h",
            "src/gui/toolbar.cpp",
            "src/gui/toolbar.h",
            "src/gui/treeutil.cpp",
            "src/gui/treeutil.h",
            "src/main.cpp",
            "src/gui/mainwindow.cpp",
            "src/gui/mainwindow.h",
            "src/project/nodemanager.cpp",
            "src/project/nodemanager.h",
            "src/project/projectcontext.cpp",
            "src/project/projectcontext.h",
            "style.qrc",
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
