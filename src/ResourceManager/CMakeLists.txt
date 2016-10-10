project(ResourceManager)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_executable(ResourceManager ${SOURCE_FILES})
