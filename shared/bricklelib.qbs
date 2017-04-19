import qbs
import qbs.FileInfo

Project {
    StaticLibrary {
        destinationDirectory: FileInfo.joinPaths(project.sourceDirectory,"bin")
        name: "bricklelib"
        cpp.cppFlags: "-std=c++11"
        files: [
            "src/bricklelib.h",
            "src/core/core.h",
            "src/core/jsonparser/jsonparser.h",
            "src/core/jsonparser/jsonparserbase.cpp",
            "src/core/jsonparser/jsonparserbase.h",
            "src/core/thirdparty/simplejson/JSON.cpp",
            "src/core/thirdparty/simplejson/JSON.h",
            "src/core/thirdparty/simplejson/JSONValue.cpp",
            "src/core/thirdparty/simplejson/JSONValue.h",
            "src/core/tree/tree.h",
            "src/core/tree/node.h",
            "src/core/tree/node.cpp",
            "src/core/tree/nodeparser.cpp",
            "src/core/tree/nodeparser.h",
            "src/core/tree/property/property.h",
            "src/core/tree/property/propertybase.h",
            "src/core/tree/property/propertybase.cpp",
            "src/core/tree/property/property_string.h",
            "src/core/tree/property/property_rect.h",
            "src/core/tree/property/property_point.h",
            "src/core/tree/property/property_int.h",
            "src/core/tree/property/property_float.h"
        ]
        Depends {
            name: "cpp"
        }
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: ['src']
        }
    }
}
