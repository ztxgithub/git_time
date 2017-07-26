# 基础知识

## shell命令相关的

- UTC/GMT：世界标准时间

- CST ：北京时间(China Standard Time) ，比世界标准时间早8小时，记为UTC+8

- 关系: CST = UTC/GMT +8 小时

## shell命令

- 显示该系统的当前时区

```shell

	> date -R  (以某种格式输出，其实里面包含了时区信息)
	Sat, 22 Apr 2017 16:58:38 +0800
	
	> date
	Sat Apr 22 17:38:34 CST 2017

```

- 将该系统的当前时区时间转化为UTC

```shell

	> date -u
	Sat Apr 22 09:39:06 UTC 2017

```

## 系统函数相关

- time_t:数据类型

```c

	time_t == long int
	
	含义：从UTC(coordinated universal time)时间1970年1月1日00时00分00秒(也称为Linux系统的Epoch时间)到当前时刻的秒数。  
	由于time_t类型长度的限制，它所表示的时间不能晚于2038年1月19日03时14分07秒(UTC)

```


- 设置系统的时区（将系统已某个特定的时区显示）

```shell

	1.确保 /usr/share/zoneinfo/ 目录下面有对应时区信息（例如/usr/share/zoneinfo/Asia/Shanghai）
	2.要在开机自启时 TZ='Asia/Shanghai'; export TZ

```

# 相关系统函数

- 从1970年1月1日00时00分00秒至今所经过的秒数

```c

	time_t time(time_t *t);
	
	含义：从 1970-01-01 00:00:00 +0000 (UTC)到现在系统的秒数
	参数：t：当 t 为非空时，其 *t 保存有1970-01-01 00:00:00 +0000 (UTC)到现在系统的秒数。
	返回值：
	       成功: 返回1970-01-01 00:00:00 +0000 (UTC)到现在系统的秒数
	       失败: -1.

```

- 获取系统的时间和设置系统时间

```c

	 #include <sys/time.h>
	
    struct timeval {
               time_t      tv_sec;     /* seconds */
               suseconds_t tv_usec;    /* microseconds */　１us = 10−6 s
           };
           
           
    int gettimeofday(struct timeval *tv, struct timezone *tz);
           
    int settimeofday(const struct timeval *tv, const struct timezone *tz);
    
    返回值:
        0:成功
        -1:失败 
    
    注意:
        struct timezone *tz 被废弃,一般置为NULL
        
```

- 时间日期的格式控制函数

```c

      size_t strftime(char *str, size_t maxsize, 
                      const char *format, const struct tm *timeptr)
                
      描述:
          strftime将一个tm结构格式化为一个字符串,strftime函数对timeptr指向的tm结构
          所代表的时间和日期进行格式编排，其结果放在字符串s中。该字符串的长度被设置为
          maxsize个字符.格式字符串format用来对写入字符串的字符进行控制，
          它包含着将被传送到字符串里去的普通字符以及编排时间和日期格式的转换控制符
          
      参数:
        str:被赋值的头指针
        maxsize：字符串被赋值的最大值
        
      返回值：
        成功:返回有效字节数,最大是maxsize-1
        失败:0(个数为0)
        
          char c_time[40];
          memset(c_time, 0, 40);
          time_t t = time(NULL);
          struct tm now_time;
          localtime_r(&t, &now_time);
          int cnt = strftime(c_time, 40 , "%Y-%m-%d %H:%M:%S",  &now_time);
          
          运行结果：
            c_time[2017-07-25 20:28:09]
            cnt[19]

```
- 将时间戳(time_t)转化为struct tm *格式
```c

    struct tm *localtime_r(const time_t *timep, struct tm *result);
                
      描述:
          将时间戳(time_t)转化为struct tm *格式,保存在result中
          
      参数:
        timep:传入参数
        result：传出参数,保存的结果
        
      返回值：


```

- mktime