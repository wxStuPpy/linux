#include <func.h>
#include <mysql/mysql.h>

int main() {
    // 初始化连接
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql) {
        fprintf(stderr, "mysql_init failed\n");
        return 1;
    }

    // 连接数据库
    MYSQL *conn = mysql_real_connect(mysql, "localhost", "root", "123456", 
                                    "test", 0, NULL, 0);
    if (!conn) {
        fprintf(stderr, "Connection failed: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return 1;
    }

    // 使用参数化查询（防SQL注入）
    const char *insert_query = "INSERT INTO pet (name, age, sex, birth) VALUES (?, ?, ?, ?)";
    MYSQL_STMT *stmt = mysql_stmt_init(mysql);
    if (!stmt) {
        fprintf(stderr, "stmt init failed: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return 1;
    }

    if (mysql_stmt_prepare(stmt, insert_query, strlen(insert_query))) {
        fprintf(stderr, "prepare failed: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(mysql);
        return 1;
    }

    // 绑定参数
    const char *name = "Daisy";
    int age = 1;
    const char *sex = "F";
    const char *birth = "2024-03-15";

    MYSQL_BIND bind[4] = {0};
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)name;
    bind[0].buffer_length = strlen(name);

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &age;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void *)sex;
    bind[2].buffer_length = strlen(sex);

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = (void *)birth;
    bind[3].buffer_length = strlen(birth);

    if (mysql_stmt_bind_param(stmt, bind)) {
        fprintf(stderr, "bind failed: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(mysql);
        return 1;
    }

    // 执行插入
    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "execute failed: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(mysql);
        return 1;
    }

    // 获取影响行数（INSERT不需要store_result）
    printf("Affected rows: %lu\n", mysql_stmt_affected_rows(stmt));

    // 清理资源
    mysql_stmt_close(stmt);
    mysql_close(mysql);
    return 0;
}