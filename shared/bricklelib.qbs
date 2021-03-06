import qbs
import qbs.FileInfo

Project {
    StaticLibrary {
        destinationDirectory: FileInfo.joinPaths(project.sourceDirectory,"bin")
        name: "bricklelib"
        cpp.cppFlags: "-std=c++11"
        //cpp.staticLibraries: "opengl32"
        cpp.staticLibraries: ["opengl32", "libglew32"]
        cpp.includePaths: ['src/core/thirdparty']
        files: [
            "src/bricklelib.h",
            "src/core/core.h",
            "src/core/camera/camera.cpp",
            "src/core/camera/camera.h",
            "src/core/jsonparser/jsonparser.h",
            "src/core/jsonparser/jsonparserbase.cpp",
            "src/core/jsonparser/jsonparserbase.h",
            "src/core/math/glmwrapper.cpp",
            "src/core/math/glmwrapper.h",
            "src/core/math/intersection.cpp",
            "src/core/math/intersection.h",
            "src/core/project/nodemanager.cpp",
            "src/core/project/nodemanager.h",
            "src/core/project/projectcontext.cpp",
            "src/core/project/projectcontext.h",
            "src/core/texture/btexturepng.cpp",
            "src/core/texture/btexturepng.h",
            "src/core/texture/texturemanager.cpp",
            "src/core/texture/texturemanager.h",
            "src/core/texture/textureutil.cpp",
            "src/core/texture/textureutil.h",
            "src/core/thirdparty/simplejson/JSON.cpp",
            "src/core/thirdparty/simplejson/JSON.h",
            "src/core/thirdparty/simplejson/JSONValue.cpp",
            "src/core/thirdparty/simplejson/JSONValue.h",
            "src/core/thirdparty/lodepng/lodepng.h",
            "src/core/thirdparty/lodepng/lodepng.cpp",
            "src/core/tree/drawnode.cpp",
            "src/core/tree/drawnode.h",
            "src/core/tree/node.h",
            "src/core/tree/node.cpp",
            "src/core/tree/node2d.cpp",
            "src/core/tree/node2d.h",
            "src/core/tree/nodeidgenerator.cpp",
            "src/core/tree/nodeidgenerator.h",
            "src/core/tree/property/property.h",
            "src/core/tree/property/property_bool.cpp",
            "src/core/tree/property/property_bool.h",
            "src/core/tree/property/property_float.cpp",
            "src/core/tree/property/property_frameref.cpp",
            "src/core/tree/property/property_frameref.h",
            "src/core/tree/property/property_int.cpp",
            "src/core/tree/property/property_list.cpp",
            "src/core/tree/property/property_list.h",
            "src/core/tree/property/property_point.cpp",
            "src/core/tree/property/property_rect.cpp",
            "src/core/tree/property/property_ref.cpp",
            "src/core/tree/property/property_ref.h",
            "src/core/tree/property/property_string.cpp",
            "src/core/tree/property/propertybase.h",
            "src/core/tree/property/propertybase.cpp",
            "src/core/tree/property/property_string.h",
            "src/core/tree/property/property_rect.h",
            "src/core/tree/property/property_point.h",
            "src/core/tree/property/property_int.h",
            "src/core/tree/property/property_float.h",
            "src/core/tree/property/propertyinstance.cpp",
            "src/core/tree/tree.h",
            "src/core/utils/dirutil.cpp",
            "src/core/utils/dirutil.h",
            "src/core/utils/ignoreparam.h",
        ]
        Depends {
            name: "cpp"
        }
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: ['src', 'src/core/thirdparty']
        }
    }
}
