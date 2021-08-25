#ifndef RAMROD_MYSQL_CONNECTION_H
#define RAMROD_MYSQL_CONNECTION_H

#include <cppconn/connection.h>  // for Connection (ptr only), ConnectOption...
#include <string>                // for string, allocator

namespace sql::mysql {
  class MySQL_Driver;
  class MySQL_Connection;
}

namespace ramrod::mysql {
  class result;
  class statement;

  class connection
  {
  public:
    connection();
    /**
     * @brief Open a new connection to the MySQL server
     *
     * Check if the connection is valid and active by using is_valid()
     *
     * @param hostname Can be either a host name or an IP address. The local host is assumed
     *                 when passing the null value or the string "localhost" to this
     *                 parameter. When possible, pipes will be used instead of the TCP/IP
     *                 protocol. The TCP/IP protocol is used if a host name and port number
     *                 are provided together e.g. localhost:3308.
     *
     *                 Prepending host by p: opens a persistent connection.
     *                 mysqli_change_user() is automatically called on connections opened
     *                 from the connection pool.
     * @param username The MySQL user name.
     * @param password If not provided or null, the MySQL server will attempt to authenticate
     *                 the user against those user records which have no password only. This
     *                 allows one username to be used with different permissions (depending
     *                 on if a password is provided or not).
     * @param database If provided will specify the default database to be used when
     *                 performing queries.
     * @param port     Specifies the port number to attempt to connect to the MySQL server.
     * @param socket   Specifies the socket or named pipe that should be used.
     *                    Note: Specifying the socket parameter will not explicitly determine
     *                          the type of connection to be used when connecting to the MySQL
     *                          server. How the connection is made to the MySQL database is
     *                          determined by the hostname parameter.
     */
    connection(const std::string &hostname,
               const std::string &username,
               const std::string &password = "",
               const std::string &database = "",
               const int port = -1, const std::string &socket = "");
    /**
      * @brief Closes the connection and frees used memory
      */
    ~connection();
    /**
     * @brief Closes a previously opened database connection
     *
     * Open non-persistent MySQL connections and result sets are automatically closed
     * when their objects are destroyed. Explicitly closing open connections and freeing
     * result sets is optional. However, it's a good idea to close the connection as soon
     * as the script finishes performing all of its database operations, if it still has
     * a lot of processing to do after getting the results.
     *
     * @return `true` on success or `false` on failure.
     */
    bool close();
    /**
     * @brief Indicates if the connection is closed
     *
     * @return `true` if is closed or never opened, `false` otherwise
     */
    bool closed();
    /**
     * @brief Obtaining the client character set
     *
     * @return String containing the current client character set
     */
    std::string charset();
    /**
     * @brief Sets the client character set
     *
     * Sets the character set to be used when sending data from and to the database server.
     * NOTE: it must be done before connecting, if done after, you need to connect again
     * using reconnect()
     *
     * @param charset   The desired character set.
     * @param reconnect It will automatically close the connection and open it again
     *
     * @return `true` on success or `false` if you need to reload the connection
     *          using reconnect() or connection was never opened.
     */
    bool charset(const std::string &charset, const bool reconnect = false);
    /**
     * @brief Open a new connection to the MySQL server
     *
     * @param hostname Can be either a host name or an IP address. The local host is assumed
     *                 when passing the null value or the string "localhost" to this
     *                 parameter. When possible, pipes will be used instead of the TCP/IP
     *                 protocol. The TCP/IP protocol is used if a host name and port number
     *                 are provided together e.g. localhost:3308.
     *
     *                 Prepending host by p: opens a persistent connection.
     *                 mysqli_change_user() is automatically called on connections opened
     *                 from the connection pool.
     * @param username The MySQL user name.
     * @param password If not provided or null, the MySQL server will attempt to authenticate
     *                 the user against those user records which have no password only. This
     *                 allows one username to be used with different permissions (depending
     *                 on if a password is provided or not).
     * @param database If provided will specify the default database to be used when
     *                 performing queries.
     * @param port     Specifies the port number to attempt to connect to the MySQL server.
     * @param socket   Specifies the socket or named pipe that should be used.
     *                    Note: Specifying the socket parameter will not explicitly determine
     *                          the type of connection to be used when connecting to the MySQL
     *                          server. How the connection is made to the MySQL database is
     *                          determined by the hostname parameter.
     *
     * @return `true` on success or `false` on failure.
     */
    bool connect(const std::string &hostname,
                 const std::string &username,
                 const std::string &password = "",
                 const std::string &database = "",
                 const int port = -1, const std::string &socket = "");
    /**
     * @brief  Initializes a statement and returns an object
     *
     * @return An statement object or `nullptr` if the connection is closed or error with driver
     */
    ramrod::mysql::statement create_statement();
    /**
     * @brief Escapes special characters reserved for mysql
     *
     * @param string String containing characters that may be placed directly into a SQL
     *
     * @return A string will all special characters escaped
     */
    std::string escape_string(const std::string &string);
    /**
     * @brief Alias of create_statement()
     */
    ramrod::mysql::statement init_statement();
    /**
     * @brief Indicates if the connection is valid
     *
     * @return `true` if the connection is valid, `false` if is invalid or connection not opened
     */
    bool is_valid();

    bool options(const int );
    /**
     * @brief Prepares an SQL statement for execution
     *
     *  Prepares the SQL query, and returns a statement handle to be used for further
     *  operations on the statement. The query must consist of a single SQL statement.
     *
     *  The statement template can contain zero or more question mark (?) parameter
     *  markers⁠—also called placeholders. The parameter markers must be bound to
     *  application variables using `bind_param()` before executing the statement.
     *
     * @param sql  The query, as a string. It must consist of a single SQL statement.
     *
     *             The SQL statement may contain zero or more parameter markers represented
     *             by question mark (?) characters at the appropriate positions.
     *
     *             Note: The markers are legal only in certain places in SQL statements.
     *             For example, they are permitted in the `VALUES()` list of an `INSERT`
     *             statement (to specify column values for a row), or in a comparison
     *             with a column in a `WHERE` clause to specify a comparison value.
     *
     *             However, they are not permitted for identifiers (such as table or
     *             column names), or to specify both operands of a binary operator such
     *             as the = equal sign. The latter restriction is necessary because it
     *             would be impossible to determine the parameter type. In general,
     *             parameters are legal only in Data Manipulation Language (DML)
     *             statements, and not in Data Definition Language (DDL) statements.
     *
     * @return A statement object
     */
    ramrod::mysql::statement prepare(const std::string &sql);
    /**
     * @brief Performs a query on the database
     *
     * Performs a query against the database.
     *
     * @param query The query string.
     *
     *              ## Warning
     *              ### Security warning: SQL injection
     *
     *              If the query contains any variable input then parameterized prepared
     *              statements should be used instead. Alternatively, the data must be
     *              properly formatted and all strings must be escaped.
     *
     * @return A result object
     */
    ramrod::mysql::result query(const std::string &query);
    /**
     * @brief Indicates if the database is read only
     *
     * @return `true` if read only or connection not opened, `false` if is writable
     */
    bool read_only();
    /**
     * @brief Making the database read only or writable
     *
     * @param set_read_only `true` to make read only, `false` otherwise
     *
     * @return `true` on success, `false` if the connection is closed or error with driver
     */
    bool read_only(const bool set_read_only);
    /**
     * @brief Reconnects if the connection has gone down
     *
     * @return `true` on success, `false` if the connection is closed or error with driver
     */
    bool reconnect();
    /**
     * @brief Obtaining the default database for database queries
     *
     * @return  The database name.
     */
    std::string schema();
    /**
     * @brief Selects the default database for database queries
     *
     * Selects the default database to be used when performing queries against the
     * database connection.
     *
     *    Note: This function should only be used to change the default database for the
     *    connection. You can select the default database with 4th parameter in connect().
     *
     * @param database The database name.
     *
     * @return `true` on success or `false` on failure.
     */
    bool schema(const std::string &schema);
    /**
     * @brief Alias of schema(const std::string&)
     */
    bool select_db(const std::string &database);

    operator sql::Connection*();

  private:
    sql::mysql::MySQL_Driver *driver_;
    sql::mysql::MySQL_Connection *connection_;
    sql::ConnectOptionsMap options_;

    ramrod::mysql::statement *statement_;
  };
} // namespace ramrod::mysql

#endif // RAMROD_MYSQL_CONNECTION_H
