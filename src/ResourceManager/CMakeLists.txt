project(ResourceManager)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_library(ResourceManager ${SOURCE_FILES})
