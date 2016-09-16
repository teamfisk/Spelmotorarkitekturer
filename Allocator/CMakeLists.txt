project(Allocator)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_executable(Allocator ${SOURCE_FILES})