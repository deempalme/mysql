# - Try to find MySQLx libraries

if (mysql_LIBRARIES AND mysql_INCLUDE_DIR)
  # in cache already
  set(mysql_FOUND TRUE)
else (mysql_LIBRARIES AND mysql_INCLUDE_DIR)

  find_path(mysql_INCLUDE_DIR mysql.h
    PATHS
      /usr/include
      /usr/include/mysql
      /usr/local/include
      /usr/local/include/mysql
      /usr/local/mysql/include
    DOC "Specify the directory containing mysql.h."
  )

  find_library(mysql_LIBMYSQL
    NAMES mysql libmysql mysqlclient
    PATHS
      /usr/lib
      /usr/lib/mysql
      /usr/local/lib
      /usr/local/lib/mysql
      /usr/local/mysql/lib
    DOC "Specify the mysql library here."
  )

  find_library(mysql_LIBCPPCONN
    NAMES mysqlcppconn libmysqlcppconn
    PATHS
      /usr/lib
      /usr/lib/mysql
      /usr/local/lib
      /usr/local/lib/mysql
      /usr/local/mysql/lib
    DOC "Specify the cppconn mysql library here."
  )

  find_library(mysql_EXTRA_LIBRARIES
    NAMES z zlib
    PATHS
      /usr/lib
      /usr/local/lib
    DOC "If more libraries are necessary to link in a MySQL client, specify them here.")

  if (mysql_LIBMYSQL AND mysql_LIBCPPCONN)
    set(mysql_FOUND TRUE)
  endif()

  if (mysql_FOUND)
    set(mysql_INCLUDE_DIRS ${mysql_INCLUDE_DIR})
    set(mysql_LIBRARIES
      ${mysql_LIBMYSQL}
      ${mysql_LIBCPPCONN}
      #${mysql_EXTRA_LIBRARIES}
    )
  endif (mysql_FOUND)

  if (mysql_FOUND)
    if (NOT mysql_FIND_QUIETLY)
      message(STATUS "Found MySQL: ${mysql_LIBRARIES}, ${mysql_INCLUDE_DIRS}")
    endif (NOT mysql_FIND_QUIETLY)
  else (mysql_FOUND)
    if (mysql_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find mysql")
    endif (mysql_FIND_REQUIRED)
  endif (mysql_FOUND)

endif (mysql_LIBRARIES AND mysql_INCLUDE_DIR)
