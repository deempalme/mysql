## Installing MySQL

Download the source files from [mysql githubs's page](https://github.com/mysql/mysql-connector-cpp.git) and uncompress them, after that execute the following commands:

```sh
git clone https://github.com/mysql/mysql-connector-cpp.git
# Go into the uncompressed folder
cd mysql-connector-cpp
# get the latest version, in this case 8.0
git checkout 8.0
# Make a build folder
mkdir build
cd build
# Configuring the project
cmake .. -DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_INSTALL_PREFIX="$ENV{HOME}/bin/mysql" \
-DWITH_BOOST="/usr/lib/x86_64-linux-gnu/cmake/Boost-1.71.0" \
-DWITH_SSL="/usr/lib/x86_64-linux-gnu" \
-DMYSQL_CONFIG_EXECUTABLE="/usr/bin/mysql_config"
# Building the project using 8 CPU threads
cmake --build . --config Debug -j 8
# Or building and installing the project using 8 CPU threads
cmake --build . --target install --config Debug -j 8
```

more at: [https://dev.mysql.com/doc/connector-cpp/8.0/en/](https://dev.mysql.com/doc/connector-cpp/8.0/en/)

### Getting the version of your `Boost` library

Simply run any CMakeLists that has boost's libraries in it and it will give oyu the current version and folder path in your system:

```cmake
-- Found Boost: /usr/lib/x86_64-linux-gnu/cmake/Boost-1.71.0/BoostConfig.cmake (found version "1.71.0") found components: system chrono filesystem thread
```

### Getting the version of your SSL library

You can also run any CMakeLists that has OpenSSL's libraries in it and it will give oyu the current version and folder path in your system:

```cmake
-- Found OpenSSL: /usr/lib/x86_64-linux-gnu/libcrypto.so (found version "1.1.1f") 
```

### Getting the MySQL's directory folders

```sh
# Change <user> for your username
mysql -u<user> -p -e 'SHOW VARIABLES WHERE Variable_Name LIKE "%dir"'
```