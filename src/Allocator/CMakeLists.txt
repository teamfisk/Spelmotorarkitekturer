project(Allocator)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_library(Allocator ${SOURCE_FILES})