include(FindPackageHandleStandardArgs)


find_path(
	GLM_INCLUDE_DIR
	NAMES glm/glm.hpp
	PATHS
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	${GLM_ROOT_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include
	DOC "The directory where glm/glm.hpp resides")


find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

if (GLM_FOUND)
	set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

mark_as_advanced(GLM_INCLUDE_DIR)
