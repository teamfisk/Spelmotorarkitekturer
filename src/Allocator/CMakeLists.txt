project(Allocator)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

add_library(Allocator ${SOURCE_FILES})