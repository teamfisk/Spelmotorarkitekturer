project(STUFFGenerator)

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

include_directories(
	${Boost_INCLUDE_DIRS}
)

file(GLOB SOURCE_FILES
	"*.h"
	"*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

add_executable(STUFFGenerator ${SOURCE_FILES})
target_link_libraries(STUFFGenerator
	${Boost_LIBRARIES}
)