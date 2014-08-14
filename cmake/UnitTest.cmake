include(NewExecutable)

function(OPENMEEG_TEST TEST_NAME TEST_PROG)
    unset(DEFAULT)
    unset(DEPENDS)
    PARSE_ARGUMENTS("DEPENDS" "" "DEFAULT" ${ARGN})
    set(TEST_PATH ${INSTALL_BIN_DIR})
    if (UNIX AND NOT APPLE)
        set(TEST_PATH ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    add_test(${TEST_NAME} ${TEST_PATH}/${TEST_PROG} ${DEFAULT})
    foreach (DEPENDENCY ${DEPENDS})
        set_property(TEST ${TEST_NAME} APPEND PROPERTY DEPENDS ${DEPENDENCY})
    endforeach()
endfunction()

function(OPENMEEG_UNIT_TEST TEST_NAME)
    PARSE_ARGUMENTS("SOURCES;LIBRARIES;PARAMETERS;DEPENDS" "" "DEFAULT" ${ARGN})
    NEW_EXECUTABLE(${TEST_NAME} SOURCES ${SOURCES} LIBRARIES ${LIBRARIES})
    #NEW_EXECUTABLE(${TEST_NAME} ${ARGN} EXCLUDE_FROM_ALL)
    #ADD_DEPENDENCIES(TARGET test ${TEST_NAME})
    if (WIN32 OR APPLE)
        set(INSTALLED_TESTS ${INSTALLED_TESTS} ${TEST_NAME} PARENT_SCOPE)
        install(TARGETS ${TEST_NAME}
                RUNTIME DESTINATION ${INSTALL_BIN_DIR}
                LIBRARY DESTINATION ${INSTALL_LIB_DIR}
                ARCHIVE DESTINATION ${INSTALL_LIB_DIR})
    endif()
    if (DEPENDS)
        set(DEPENDS DEPENDS ${DEPENDS})
    endif()
    OPENMEEG_TEST(${TEST_NAME} ${TEST_NAME} ${PARAMETERS} ${DEPENDS})
endfunction()
