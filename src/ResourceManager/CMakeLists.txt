project(ResourceManager)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

add_library(ResourceManager ${SOURCE_FILES})
