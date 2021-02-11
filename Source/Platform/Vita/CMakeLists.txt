
include("${VITASDK}/share/vita.cmake" REQUIRED)

list(APPEND EXECUTABLE_SOURCES
	"../Util/AccessConfig.c"
	"../Util/AccessData.c"
	"../Util/AccessEeprom.c"
	"../Util/Eeprom.c"
	"../Util/EepromData.c"
	"../Util/HwData.c"
	"../Util/ini.c"
	"../Util/Render.c"
	"../Util/RomData.c"

	"AccessDisplay.c"
)
list(APPEND EXECUTABLE_HEADERS
	"../Util/AccessConfig.h"
	"../Util/AccessData.h"
	"../Util/AccessEeprom.h"
	"../Util/Color.h"
	"../Util/EepromData.h"
	"../Util/HwData.h"
	"../Util/HwSound.h"
	"../Util/ini.h"
	"../Util/Macros.h"
	"../Util/Render.h"

	"PlatformSupport/Config.h"
	"PlatformSupport/HwData.h"
	"PlatformSupport/PlatformTypes.h"
	"PlatformSupport/PlatformUpdate.h"

	"AccessDisplay.h"
)

set(VITA_APP_NAME "${EXECUTABLE_NAME}")
set(VITA_TITLEID "JOEY00004")
set(VITA_VERSION "1.00")

## Flags and includes for building
# Note that we make sure not to overwrite previous flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Optional. You can specify more param.sfo flags this way.
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable("${EXECUTABLE_NAME}" ${EXECUTABLE_SOURCES} ${EXECUTABLE_HEADERS})
source_group("Header Files" FILES ${EXECUTABLE_HEADERS})
target_include_directories("${EXECUTABLE_NAME}" PUBLIC "../.." "PlatformSupport")

target_link_libraries(${EXECUTABLE_NAME}
    SDL2
    PhysFS
)
## Create Vita files
vita_create_self(${EXECUTABLE_NAME}.self ${EXECUTABLE_NAME})
# The FILE directive lets you add additional files to the VPK, the syntax is
# FILE src_path dst_path_in_vpk. In this case, we add the LiveArea paths.
vita_create_vpk(${EXECUTABLE_NAME}.vpk ${VITA_TITLEID} ${EXECUTABLE_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
# if(VCPKG_TOOLCHAIN)
# 	set_property(
# 		TARGET "${EXECUTABLE_NAME}"
# 		PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
# 		${CMAKE_SOURCE_DIR}
# 	)

# 	set_property(
# 		DIRECTORY "${CMAKE_SOURCE_DIR}"
# 		PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME}
# 	)
# endif()


# find_package(PkgConfig)
# if(PKG_CONFIG_FOUND)
# 	function(add_pkg_module EXE PKG)
# 		string(TOUPPER ${PKG} PKG_PREFIX)
# 		pkg_check_modules(${PKG_PREFIX} REQUIRED ${PKG})
# 		target_compile_options(${EXE} PRIVATE "${${PKG_PREFIX}_CFLAGS}")
# 		target_compile_options(${EXE} PRIVATE "${${PKG_PREFIX}_CFLAGS_OTHER}")
# 		target_include_directories(${EXE} SYSTEM PRIVATE "${${PKG_PREFIX}_INCLUDE_DIRS}")
# 		target_link_directories(${EXE} PRIVATE "${${PKG_PREFIX}_LIBRARY_DIRS}")
# 		target_link_libraries(${EXE} PRIVATE "${${PKG_PREFIX}_LIBRARIES}")
# 		target_link_options(${EXE} PRIVATE "${${PKG_PREFIX}_LDFLAGS}")
# 		target_link_options(${EXE} PRIVATE "${${PKG_PREFIX}_LDFLAGS_OTHER}")
# 	endfunction()

# 	add_pkg_module("${EXECUTABLE_NAME}" sdl2)
# 	find_package(PhysFS REQUIRED)
# 	target_link_libraries(${EXECUTABLE_NAME}
# 		PRIVATE
# 			${PHYSFS_LIBRARY}
# 	)
# 	target_include_directories(${EXECUTABLE_NAME}
# 		PRIVATE
# 			${PHYSFS_INCLUDE_DIR}
# 	)
# elseif(VCPKG_TOOLCHAIN)
# 	set(VCPKG_CRT_LINKAGE dynamic)
# 	set(VCPKG_LIBRARY_LINKAGE dynamic)

# 	find_package(SDL2 REQUIRED)
# 	find_package(PhysFS REQUIRED)
# 	target_link_libraries(${EXECUTABLE_NAME}
# 		PRIVATE
# 			SDL2::SDL2
# 			SDL2::SDL2main
# 			${PHYSFS_LIBRARY}
# 	)
# endif()
