cmake_minimum_required(VERSION 2.6)

project(SPI)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall")

include_directories(lib/)

add_library(mcp3008-lib lib/MCP3008.cpp)
target_include_directories(mcp3008-lib PUBLIC "/usr/local/include/")
target_include_directories(mcp3008-lib PUBLIC "/usr/local/lib/")
target_link_libraries(mcp3008-lib pigpio)

add_executable(
	mcp-test src/mcp-test.cpp
)
target_link_libraries(mcp-test mcp3008-lib)

add_dependencies(mcp-test mcp3008-lib)

INSTALL(TARGETS mcp3008-lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
INSTALL(FILES lib/MCP3008.h DESTINATION include)
