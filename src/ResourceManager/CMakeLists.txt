project(ResourceManager)

file(GLOB SOURCE_FILES
    "*.h"
    "*.cpp"
)
source_group("" FILES ${SOURCE_FILES})

file(GLOB SOURCE_FILES_STUFF
    "STUFF/*.h"
    "STUFF/*.cpp"
)
source_group(STUFF FILES ${SOURCE_FILES_STUFF})
set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_FILES_STUFF})

add_library(ResourceManager 
	${SOURCE_FILES}
)