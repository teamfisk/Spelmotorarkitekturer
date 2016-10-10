project(Renderer)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_executable(Renderer ${SOURCE_FILES})
