﻿#cmake version
cmake_minimum_required(VERSION 3.5...3.15)

if(NOT TARGET ${MySQL_wrapper_LIBRARIES})
  project(MySQL_wrapper
    VERSION 1.0.0
    DESCRIPTION "MySQL and MySQLx php-styled library"
    LANGUAGES CXX C
  )

  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

  # mysql libraries
  find_package(mysql REQUIRED)
  #find_package(mysqlx REQUIRED)


  #header files
  set(HPP_FILES
    include/ramrod/mysql/connection.h
    include/ramrod/mysql/param.h
    include/ramrod/mysql/parameter.h
    include/ramrod/mysql/parameter_metadata.h
    include/ramrod/mysql/result.h
    include/ramrod/mysql/result_metadata.h
    include/ramrod/mysql/result_stmt.h
    include/ramrod/mysql/statement.h
    include/ramrod/mysql/types.h
    include/ramrod/mysql/warning.h
  )

  #source files
  set(CPP_FILES
    src/ramrod/mysql/connection.cpp
    src/ramrod/mysql/param.cpp
    src/ramrod/mysql/parameter.cpp
    src/ramrod/mysql/parameter_metadata.cpp
    src/ramrod/mysql/result.cpp
    src/ramrod/mysql/result_metadata.cpp
    src/ramrod/mysql/result_stmt.cpp
    src/ramrod/mysql/statement.cpp
    src/ramrod/mysql/warning.cpp
  )

  set(MySQL_wrapper_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL ""
  )
  set(MySQL_wrapper_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "")

  #add every file to the library
  add_library(${PROJECT_NAME})

  target_sources(${PROJECT_NAME}
    PRIVATE
      ${CPP_FILES}
      ${HPP_FILES}
  )

  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

  set_target_properties(${PROJECT_NAME}
    PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )

  target_include_directories(${PROJECT_NAME}
    PRIVATE
      include
      ${mysql_INCLUDE_DIRS}
      ${mysqlx_INCLUDE_DIRS}
  )

  #indicates which libraries to use in the executable
  target_link_libraries(${PROJECT_NAME}
    ${mysql_LIBRARIES}
    ${mysqlx_LIBRARIES}
  )

  if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME}
      PRIVATE
      -Wall
      -Wextra
      -Wundef
      -pedantic
      -Wconversion
      -Wcast-align
      -Wfloat-equal
      -Wpointer-arith
      -Wsign-conversion
      -Wunreachable-code
      -Wunused-function
      -Wignored-qualifiers
    )

    ##########################
    ## include-what-you-use ##
    ##########################

    # TODO: remove iwyu?
    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
      find_program(iwyu_path NAMES include-what-you-use iwyu)
      if(NOT iwyu_path)
        message(WARNING "Could not find the program include-what-you-use, but not essential")
      else()
        message(STATUS "include-what-you-use found: ${iwyu_path}")
        get_filename_component(PARENT_DIR ${iwyu_path} DIRECTORY)
        get_filename_component(G_PARENT_DIR ${PARENT_DIR} DIRECTORY)
        set(iwyu_share_path ${G_PARENT_DIR}/share/include-what-you-use)
        message("iwyu share path: ${iwyu_share_path}")
        set(iwyu_path_and_options
          ${iwyu_path}
          -Xiwyu
          --mapping_file=${iwyu_share_path}/boost-all.imp
          -Xiwyu
          --mapping_file=${iwyu_share_path}/boost-all-private.imp
        )
        set_target_properties(
          ${PROJECT_NAME}
          PROPERTIES
          CXX_INCLUDE_WHAT_YOU_USE
          "${iwyu_path_and_options}"
        )
      endif(NOT iwyu_path)
    endif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
  endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif(NOT TARGET ${MySQL_wrapper_LIBRARIES})
