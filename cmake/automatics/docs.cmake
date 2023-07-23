# ---- Dependencies ----

find_package(Doxygen REQUIRED doxygen)
if (NOT TARGET Doxygen::doxygen)
	message(WARNING "Doxygen not found.")
	return()
endif()

set(doxyfile ${CMAKE_BINARY_DIR}/docs/Doxyfile)
configure_file("${GRM_DOCS_DIR}/Doxyfile.in" "${doxyfile}" @ONLY NEWLINE_STYLE LF)

add_custom_target(clear_docs
	COMMENT "Cleaning documentation"
	COMMAND "${CMAKE_COMMAND}" -E remove_directory
		"${GRM_DOCS_DIR}/html"
		"${GRM_DOCS_DIR}/xml"
)

set(DOXYGEN_STRIP_FROM_PATH OFF)

if (CMAKE_VERSION VERSION_LESS 3.27)
	add_custom_target(generate_docs
		COMMENT "Generating documentation"
		WORKING_DIRECTORY ${GRM_DOCS_DIR}
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		VERBATIM
	)
else()
	doxygen_add_docs(generate_docs ALL
		COMMENT "Generating documentation"
		WORKING_DIRECTORY "${GRM_DOCS_DIR}"
		CONFIG_FILE "${doxyfile}"
	)
endif()

set_target_properties(clear_docs generate_docs PROPERTIES FOLDER "documentation")

add_dependencies(generate_docs clear_docs)
