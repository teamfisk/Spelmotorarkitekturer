project(Test)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)

add_executable(Test ${SOURCE_FILES})
target_link_libraries(Test pthread)