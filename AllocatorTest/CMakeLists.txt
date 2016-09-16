project(AllocatorTest)

find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE}
)

#file(GLOB SOURCE_FILES
#    "*.h"
#    "*.cpp"
#    "../Allocator/*.h"
#    "../Allocator/*.cpp"
#)

set(SOURCE_FILES
	"StackAllocatorTests.cpp"
	"PoolAllocatorTests.cpp"
)

set(LIBRARIES
    ${GTEST_LIBRARIES}
)

enable_testing()
add_executable(AllocatorTest ${SOURCE_FILES})
target_link_libraries(AllocatorTest ${GTEST_BOTH_LIBRARIES} pthread)
add_test(AllocatorTest AllocatorTest)