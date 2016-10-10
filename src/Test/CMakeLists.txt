project(Test)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

add_executable(Test ${SOURCE_FILES})
if (UNIX)
	target_link_libraries(Test pthread)
endif(UNIX)