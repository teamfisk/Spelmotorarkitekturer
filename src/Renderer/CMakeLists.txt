project(Renderer)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
add_definitions(-DGLEW_STATIC)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)
find_package(assimp REQUIRED)

include_directories(
    ${INCLUDE_PATH}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	${assimp_INCLUDE_DIRS}
)

set(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
	${PNG_LIBRARIES}
	${assimp_LIBRARIES}
)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

add_executable(Renderer ${SOURCE_FILES})
target_link_libraries(Renderer
	Allocator
	ResourceManager
    ${LIBRARIES}
)
