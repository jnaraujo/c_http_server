cmake_minimum_required(VERSION 3.0.0)
project(c_http_server VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(src)

set(headers
  src/Router/Router.h
  src/Router/HtmlFileFolderRead.h
  src/Server/Server.h
)

set(sources
  src/Router/Router.c
  src/Router/HtmlFileFolderRead.C
  src/Server/Server.c
)

add_executable(c_http_server
  src/main.c
  ${headers}
  ${sources}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
