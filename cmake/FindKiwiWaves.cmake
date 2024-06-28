# KiwiWaves_FOUND: if found
# KiwiWaves: imported module

include(FindPackageHandleStandardArgs)

find_path(KiwiWaves_INCLUDE_DIR NAMES KiwiWaves.h PATH_SUFFIXES KiwiWaves/include)
find_library(KiwiWaves_LIBRARY NAMES KiwiWaves PATH_SUFFIXES KiwiWaves/lib)

find_package_handle_standard_args(KiwiWaves
	FOUND_VAR KiwiWaves_FOUND
	REQUIRED_VARS KiwiWaves_LIBRARY KiwiWaves_INCLUDE_DIR
)

if (KiwiWaves_FOUND AND NOT TARGET KiwiWaves)
	add_library(KiwiWaves UNKNOWN IMPORTED)
	set_target_properties(KiwiWaves PROPERTIES
		IMPORTED_LOCATION ${KiwiWaves_LIBRARY}
		INTERFACE_INCLUDE_DIRECTORIES ${KiwiWaves_INCLUDE_DIR}
	)
endif()

mark_as_advanced(KiwiWaves_LIBRARY KiwiWaves_INCLUDE_DIR)