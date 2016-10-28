project(Test)

find_package(Boost REQUIRED COMPONENTS system filesystem thread chrono timer program_options)

include_directories(
	${Boost_INCLUDE_DIRS}
)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

add_executable(Test ${SOURCE_FILES})
if (UNIX)
	target_link_libraries(Test pthread)
endif(UNIX)

target_link_libraries(Test 
	ResourceManager
	${Boost_LIBRARIES}
)