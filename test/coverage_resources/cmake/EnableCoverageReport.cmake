# - Creates a special coverage build type and target on GCC.
#
# Defines a function ENABLE_COVERAGE_REPORT which generates the coverage target
# for selected targets. Optional arguments to this function are used to filter
# unwanted results using globbing expressions. Moreover targets with tests for
# the source code can be specified to trigger regenerating the report if the
# test has changed.
#
# ENABLE_COVERAGE_REPORT(TARGETS target... [FILTER filter...] [TESTS test targets...])
#
# To generate a coverage report first build the project with
# CMAKE_BUILD_TYPE=coverage, then call make test and afterwards make coverage.
#
# The coverage report is based on gcov. Depending on the availability of lcov
# a HTML report will be generated and/or an XML report of gcovr is found.
# The generated coverage target executes all found solutions. Special targets
# exist to create e.g. only the xml report: coverage-xml. 
#
# Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The development of this software was supported by:
#   CoR-Lab, Research Institute for Cognition and Robotics
#     Bielefeld University

FUNCTION(_CoverageReport_SET_GCC_VERSION)
    EXECUTE_PROCESS(COMMAND gcc -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    STRING(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    LIST(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    LIST(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)
    STRING(LENGTH ${GCC_MAJOR} MINOR_LEN)
    IF(${MINOR_LEN} EQUAL 1)
        SET(GCC_MINOR "0${GCC_MINOR}")
    ENDIF()
    SET(GCC_VERSION "${GCC_MAJOR}${GCC_MINOR}" PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(ENABLE_COVERAGE_REPORT)
    # argument parsing
    SET(MULTI_VALUE_ARGS FILTER TARGETS TESTS)
    CMAKE_PARSE_ARGUMENTS(ENABLE_COVERAGE_REPORT "" "" "${MULTI_VALUE_ARGS}" ${ARGN})

    SET(COVERAGE_RAW_FILE "${CMAKE_BINARY_DIR}/coverage.raw.info")
    SET(COVERAGE_FILTERED_FILE "${CMAKE_BINARY_DIR}/coverage.info")
    SET(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/coveragereport")
    SET(COVERAGE_XML_FILE "${CMAKE_BINARY_DIR}/coverage.xml")
    
    # decide if there is any tool to create coverage data
    IF(NOT CoverageReport_FOUND)
        MESSAGE(STATUS "Cannot enable coverage targets because neither lcov nor gcovr are found.")
    ENDIF()
    
    STRING(TOLOWER "${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_TYPE)
    IF(CoverageReport_FOUND AND "${LOWER_CMAKE_BUILD_TYPE}" STREQUAL "coverage")
    
        MESSAGE(STATUS "Coverage support enabled for targets: ${ENABLE_COVERAGE_REPORT_TARGETS}")
    
        # create coverage build type
        SET(CMAKE_CXX_FLAGS_COVERAGE ${CMAKE_CXX_FLAGS_DEBUG} PARENT_SCOPE)
        SET(CMAKE_C_FLAGS_COVERAGE ${CMAKE_C_FLAGS_DEBUG} PARENT_SCOPE)
        SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} coverage PARENT_SCOPE)
    
        # instrument targets

        # If we are using Clang, tell it to generate coverage data suitable for gcovr.
        IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            _CoverageReport_SET_GCC_VERSION()
            set(CLANG_COVERAGE_ARGS " -Xclang -coverage-cfg-checksum -Xclang -coverage-no-function-names-in-data -Xclang -coverage-version='${GCC_VERSION}*'")
        ENDIF()

        FOREACH(T ${ENABLE_COVERAGE_REPORT_TARGETS})
            SET_PROPERTY(TARGET ${T} APPEND_STRING PROPERTY COMPILE_FLAGS " -g --coverage")
            SET_PROPERTY(TARGET ${T} APPEND_STRING PROPERTY LINK_FLAGS " -g --coverage")
            SET_PROPERTY(TARGET ${T} APPEND_STRING PROPERTY COMPILE_FLAGS ${CLANG_COVERAGE_ARGS})
        ENDFOREACH()

        FOREACH(T ${ENABLE_COVERAGE_REPORT_TESTS})
            SET_PROPERTY(TARGET ${T} APPEND_STRING PROPERTY LINK_FLAGS " -g --coverage")
        ENDFOREACH()

        # html report
        IF (LCOV_FOUND)
        
            MESSAGE(STATUS "Enabling HTML coverage report")
    
            # set up coverage target
            
            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_RAW_FILE}
                               COMMAND ${LCOV_EXECUTABLE} -c -d ${CMAKE_BINARY_DIR} -o ${COVERAGE_RAW_FILE}
                               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                               COMMENT "Collecting coverage data"
                               DEPENDS ${ENABLE_COVERAGE_REPORT_TARGETS} ${ENABLE_COVERAGE_REPORT_TESTS}
                               VERBATIM)
            
            # filter unwanted stuff
            LIST(LENGTH ENABLE_COVERAGE_REPORT_FILTER FILTER_LENGTH)
            IF(${FILTER_LENGTH} GREATER 0)
                SET(FILTER COMMAND ${LCOV_EXECUTABLE} -r ${COVERAGE_FILTERED_FILE} ${ENABLE_COVERAGE_REPORT_FILTER} -o  ${COVERAGE_FILTERED_FILE})
            ELSE()
                SET(FILTER "")
            ENDIF()
            
            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_FILTERED_FILE}
                               COMMAND ${LCOV_EXECUTABLE} -e ${COVERAGE_RAW_FILE} "${CMAKE_SOURCE_DIR}*"  -o ${COVERAGE_FILTERED_FILE}
                               ${FILTER}
                               DEPENDS ${COVERAGE_RAW_FILE}
                               COMMENT "Filtering recorded coverage data for project-relevant entries"
                               VERBATIM)
            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_REPORT_DIR}
                               COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_REPORT_DIR}
                               COMMAND ${GENHTML_EXECUTABLE} --legend --show-details -t "${PROJECT_NAME} test coverage" -o ${COVERAGE_REPORT_DIR} ${COVERAGE_FILTERED_FILE}
                               DEPENDS ${COVERAGE_FILTERED_FILE}
                               COMMENT "Generating HTML coverage report in ${COVERAGE_REPORT_DIR}"
                               VERBATIM)
                               
            ADD_CUSTOM_TARGET(coverage-html
                              DEPENDS ${COVERAGE_REPORT_DIR})
                              
        ENDIF()
        
        # xml coverage report
        IF(GCOVR_FOUND)
        
            MESSAGE(STATUS "Enabling XML coverage report")

            # filter unwanted stuff
            SET(GCOVR_FILTER "")
            FOREACH(F ${ENABLE_COVERAGE_REPORT_FILTER})
                # Convert the pattern to a regexp using the same rules
                # as lcov's transform_pattern() function.
                STRING(REPLACE "\\" "\\\\" F ${F})
                STRING(REPLACE "/" "\\/" F ${F})
                STRING(REPLACE "^" "\\^" F ${F})
                STRING(REPLACE "\$" "\\\$" F ${F})
                STRING(REPLACE "(" "\\(" F ${F})
                STRING(REPLACE ")" "\\)" F ${F})
                STRING(REPLACE "[" "\\[" F ${F})
                STRING(REPLACE "]" "\\]" F ${F})
                STRING(REPLACE "{" "\\{" F ${F})
                STRING(REPLACE "}" "\\}" F ${F})
                STRING(REPLACE "." "\\." F ${F})
                STRING(REPLACE "," "\\," F ${F})
                STRING(REPLACE "|" "\\|" F ${F})
                STRING(REPLACE "+" "\\+" F ${F})
                STRING(REPLACE "!" "\\!" F ${F})

                STRING(REPLACE "*" ".*" F ${F})
                STRING(REPLACE "?" "." F ${F})

                SET(GCOVR_FILTER ${GCOVR_FILTER} -e "^${F}\$")
            ENDFOREACH()

            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_XML_FILE}
                               COMMAND "${GCOVR_EXECUTABLE}" -x -r "${CMAKE_SOURCE_DIR}" ${GCOVR_FILTER} -o "${COVERAGE_XML_FILE}"
                               WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                               COMMENT "Generating coverage XML report"
                               VERBATIM)
                               
            ADD_CUSTOM_TARGET(coverage-xml
                              DEPENDS ${COVERAGE_XML_FILE})
        
        ENDIF()
        
        # provide a global coverage target executing both steps if available
        SET(GLOBAL_DEPENDS "")
        IF(LCOV_FOUND)
            LIST(APPEND GLOBAL_DEPENDS ${COVERAGE_REPORT_DIR})
        ENDIF()
        IF(GCOVR_FOUND)
            LIST(APPEND GLOBAL_DEPENDS ${COVERAGE_XML_FILE})
        ENDIF()
        IF(LCOV_FOUND OR GCOVR_FOUND)
            ADD_CUSTOM_TARGET(coverage
                              DEPENDS ${GLOBAL_DEPENDS})
        ENDIF()
                          
    ENDIF()

    # This gets rid of any stale .gcda files. Run this if a running a binary causes lots of
    # messages about about a "merge mismatch for summaries".
    ADD_CUSTOM_TARGET(clean-coverage COMMAND find ${CMAKE_BINARY_DIR} -name '*.gcda' | xargs rm -f)

ENDFUNCTION()
