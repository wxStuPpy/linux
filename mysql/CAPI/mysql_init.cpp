#include<func.h>
#include<mysql/mysql.h>

int main(void){
    MYSQL *mysql=mysql_init(nullptr);
    MYSQL *cret=mysql_real_connect(mysql,"localhost","root","123456","test",0,nullptr,0);
    if(cret==nullptr){
        fprintf(stderr,"mysql_real_connect:%s\n",mysql_error(mysql));
        return 1;
    }

    const char *insert_query = "INSERT INTO pet (name, age, sex, birth) VALUES ('Daisy', 1, 'F', '2024-03-15')";
    int qret=mysql_query(mysql,insert_query);
    if(qret!=0){
        fprintf(stderr,"mysql_query:%s\n",mysql_error(mysql));
        return 1;
    }
    mysql_close(mysql);
    return 0;
}