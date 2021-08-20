#ifndef RAMROD_CONNECTION_H
#define RAMROD_CONNECTION_H

#include <string>

namespace sql {
  class Connection;
  namespace mysql {
    class MySQL_Driver;
  }
}

namespace ramrod {
  class connection
  {
  public:
    connection();
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

    int error_code();

  private:
    sql::mysql::MySQL_Driver *driver_;
    sql::Connection *connection_;
  };
} // namespace ramrod

#endif // RAMROD_CONNECTION_H
