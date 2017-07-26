#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <cstring>

int main()
{
    /*时间格式化字符*/
    char c_time[40];
    memset(c_time, 0, 40);
    time_t t = time(NULL);
    struct tm now_time;
    localtime_r(&t, &now_time);
    int cnt = strftime(c_time, 40 , "%Y-%m-%d %H:%M:%S",  &now_time);
    printf("c_time[%s]\n", c_time);
    printf("cnt[%d]\n", cnt);


    return 0;
}