project(Test)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_executable(Test ${SOURCE_FILES})