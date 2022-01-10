find_package(Lcov)
find_package(gcovr)

# decide if there is any tool to create coverage data
set(CoverageReport_FOUND FALSE)
if(Lcov_FOUND OR gcovr_FOUND)
  set(CoverageReport_FOUND TRUE)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/EnableCoverageReport.cmake")
