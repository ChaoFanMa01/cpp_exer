#include <iostream>
#include <cstdlib>
#include <mysql/mysql.h>

int
main(void) {
    MYSQL*        mysql;    //连接mysql句柄指针
    MYSQL_RES*    result;   //指向查询结果的指针
    MYSQL_ROW     row;      //按行返回的查询信息

    const char host[] = "localhost";
    const char user[] = "root";
    const char passwd[] = "0000000027";
    const char db[] = "cpp";
    unsigned int port = 0;
    unsigned long client_flag = 0;

    /*
     * prototype: MYSQL* mysql_init(MYSQL* mysql)
     *
     * 分配或初始化MySQL适合的对象。如果参数mysql是NULL指针，则该函数分配、
     * 初始化并最终返回一个新的对象；否则，参数mysql被初始化并返回其地址。
     * 如没有足够内存分配一个新MYSQL实例，则返回NULL。
     */
    if ((mysql = mysql_init(nullptr)) == nullptr) {
        /*
         * prototype: const char *mysql_error(MYSQL* mysql)
         *
         * For the connection specified by parameter mysql, mysql_error() 
         * returns a null-terminated string containing the error message
         * for the most recently invoked API function that failed. If a
         * function did not fail, the return value of mysql_error() may
         * be the previous error or an empty string to indicate no error.
         */
        std::cout << "Error: " << mysql_error(mysql) << std::endl;
        std::exit(-1);
    }

    /*
     * for detail refer to MySQL C API.
     */
    if ((mysql = mysql_real_connect(mysql, host, user, passwd, db, port, nullptr, client_flag)) == nullptr) {
        std::cout << "Error: " << mysql_error(mysql) << std::endl;
        std::exit(-1);
    }

    /*
     * prototype: int mysql_query(MYSQL* mysql, const char* stmt_str)
     *
     * Executes the SQL statement pointed by the null-terminated string stmt_str.
     * Returns 0 for success, otherwise nonzero value will be returned.
     */
    if (mysql_query(mysql, "select * from artist;")) {
        std::cout << "Error: " << mysql_error(mysql) << std::endl;
        exit(-1);
    }

    /*
     * Fetch the queried result.
     */
    if (result = mysql_store_result(mysql)) {
        // 获取结果集中总共的字段数，即列数
        int fields_n = mysql_num_fields(result);
        // 获取结果集中总共的行数
        int rows_n = mysql_num_rows(result);

        for (int i = 0; i < rows_n; ++i) {
            // 获取下一行数据
            if ((row = mysql_fetch_row(result)) < 0)
                break;
            for (int j = 0; j < fields_n; ++j)
                std::cout << row[j] << "\t\t";

            std::cout << std::endl;
        }
    }

    return 0;
}
