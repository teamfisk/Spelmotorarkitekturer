if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(assimp_ARCHITECTURE "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(assimp_ARCHITECTURE "32")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	
if(WIN32)
	set(assimp_ROOT_DIR CACHE PATH "assimp root directory")

	# Find path of each library
	find_path(assimp_INCLUDE_DIR
		NAMES
			assimp/anim.h
		HINTS
			${assimp_ROOT_DIR}/include
	)

	if(MSVC12)
		set(assimp_MSVC_VERSION "vc120")
	elseif(MSVC14)	
		set(assimp_MSVC_VERSION "vc140")
	endif(MSVC12)
	
	if(MSVC12 OR MSVC14)
	
		find_path(assimp_LIBRARY_DIR
			NAMES
				assimp-${assimp_MSVC_VERSION}-mt.lib
			HINTS
				${assimp_ROOT_DIR}/lib${assimp_ARCHITECTURE}
		)
		
		find_library(assimp_LIBRARY_RELEASE				assimp-${assimp_MSVC_VERSION}-mt.lib 			PATHS ${assimp_LIBRARY_DIR})
		find_library(assimp_LIBRARY_DEBUG				assimp-${assimp_MSVC_VERSION}-mtd.lib			PATHS ${assimp_LIBRARY_DIR})
		
		set(assimp_LIBRARY 
			optimized 	${assimp_LIBRARY_RELEASE}
			debug		${assimp_LIBRARY_DEBUG}
		)
		
		set(assimp_LIBRARIES ${assimp_LIBRARY})
	
		FUNCTION(assimp_COPY_BINARIES TargetDirectory)
			ADD_CUSTOM_TARGET(AssimpCopyBinaries
				COMMAND ${CMAKE_COMMAND} -E copy ${assimp_ROOT_DIR}/bin${assimp_ARCHITECTURE}/assimp-${assimp_MSVC_VERSION}-mtd.dll 	${TargetDirectory}/Debug/assimp-${assimp_MSVC_VERSION}-mtd.dll
				COMMAND ${CMAKE_COMMAND} -E copy ${assimp_ROOT_DIR}/bin${assimp_ARCHITECTURE}/assimp-${assimp_MSVC_VERSION}-mt.dll 		${TargetDirectory}/Release/assimp-${assimp_MSVC_VERSION}-mt.dll
			COMMENT "Copying Assimp binaries to '${TargetDirectory}'"
			VERBATIM)
		ENDFUNCTION(assimp_COPY_BINARIES)
	
	endif()
	
else(WIN32)

	find_path(
	  assimp_INCLUDE_DIRS
	  NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
	  PATHS /usr/local/include/
	)

	find_library(
	  assimp_LIBRARIES
	  NAMES assimp
	  PATHS /usr/local/lib/
	)

	if (assimp_INCLUDE_DIRS AND assimp_LIBRARIES)
	  SET(assimp_FOUND TRUE)
	ENDIF (assimp_INCLUDE_DIRS AND assimp_LIBRARIES)

	if (assimp_FOUND)
	  if (NOT assimp_FIND_QUIETLY)
		message(STATUS "Found asset importer library: ${assimp_LIBRARIES}")
	  endif (NOT assimp_FIND_QUIETLY)
	else (assimp_FOUND)
	  if (assimp_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find asset importer library")
	  endif (assimp_FIND_REQUIRED)
	endif (assimp_FOUND)
	
endif(WIN32)