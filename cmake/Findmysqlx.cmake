# - Try to find MySQLx libraries

if (mysqlx_LIBRARIES AND mysqlx_INCLUDE_DIR)
  # in cache already
  set(mysqlx_FOUND TRUE)
else (mysqlx_LIBRARIES AND mysqlx_INCLUDE_DIR)

  find_path(mysqlx_ALL_INCLUDE_DIR
    NAMES mysqlx
    PATHS $ENV{HOME}/bin/mysql/include
    NO_DEFAULT_PATH
  )

  find_library(mysqlx_LIBMYSQLCONN8
    NAMES mysqlcppconn8
    PATHS $ENV{HOME}/bin/mysql/lib64
    PATH_SUFFIXES debug release
    NO_DEFAULT_PATH
  )

  if (mysqlx_LIBMYSQLCONN8)
    set(mysqlx_FOUND TRUE)
  endif()

  if (mysqlx_FOUND)
    set(mysqlx_INCLUDE_DIRS ${mysqlx_ALL_INCLUDE_DIR})
    set(mysqlx_LIBRARIES ${mysqlx_LIBMYSQLCONN8})
  endif (mysqlx_FOUND)

  if (mysqlx_FOUND)
    if (NOT mysqlx_FIND_QUIETLY)
      message(STATUS "Found MySQLx: ${mysqlx_LIBRARIES}, ${mysqlx_INCLUDE_DIRS}")
    endif (NOT mysqlx_FIND_QUIETLY)
  else (mysqlx_FOUND)
    if (mysqlx_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libmysqlconn8")
    endif (mysqlx_FIND_REQUIRED)
  endif (mysqlx_FOUND)

endif (mysqlx_LIBRARIES AND mysqlx_INCLUDE_DIR)
