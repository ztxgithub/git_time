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
               suseconds_t tv_usec;    /* microseconds */　１us = 10^−6 s
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
            
      注意:
        时间格式(format)可以根据　> man date 命令来查看 

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

-mktime函数: 将struct tm *格式转化为时间戳(time_t)

```c

    time_t mktime(struct tm *timeptr);
                
      描述:
          将参数timeptr所指的tm结构数据转换成从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数
          
      参数:
        timeptr:传入参数 struct tm结构体
        
      返回值：
            成功: 时间戳
            失败：-1
            
      注意：
        在转化为时间戳时 tm.tm_wday 和 tm.tm_yday 没有意义会被忽略,其他的tm结构体的值则有用
```

### 定时函数

- setitimer函数

```c

    struct itimerval {  
        struct timeval it_interval; /* next value */  
        struct timeval it_value;    /* current value */  
    };  
      
    struct timeval {  
        time_t      tv_sec;         /* seconds */  
        suseconds_t tv_usec;        /* microseconds */  １us = 10^−6 s
    }; 
    
    int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value); 
        
        描述：
            settimer工作机制是，先对it_value倒计时,当it_value为零时触发信号,然后it_value值重置为it_interval,
            继续对it_value倒计时,
            一直这样循环下去.基于此机制，setitimer既可以用来延时执行,也可定时执行.
        参数：
            which：类型
                    ITIMER_REAL：以系统真实的时间来计算,它送出SIGALRM信号(一般选这个)
                    ITIMER_VIRTUAL：以该进程在用户态下花费的时间来计算,它送出SIGVTALRM信号。
                    ITIMER_PROF：以该进程在用户态下和内核态下所费的时间来计算,它送出SIGPROF信号
            
            new_value：　it_interval为计时间隔，it_value为延时时长(即成功调用setitimer函数,延长it_value时间才发送信号)
            old_value：　通常用不上,设置为 NULL,它是用来存储上一次setitimer调用时设置的new_value值。
        返回值：
                0:成功
                -1:失败
                
        注意：
            setitimer和sleep会冲突的！因为它们都使用了信号ITIMER_REAL

```

### timerfd 定时器

- 概要

```shell
    1. timerfd 是 Linux 为用户程序提供的一个定时器接口。这个接口基于文件描述符，通过文件描述符的可读事件进行超时通知，
       因此能够被用于select/poll的应用场景
```

- timerfd_create 函数

```c
    int timerfd_create(int clockid, int flags);
    
    描述:
        timerfd_create函数创建一个定时器对象，同时返回一个与之关联的文件描述符
        
    参数:
        clockid: 标识指定的时钟计数器
                 CLOCK_REALTIME: 系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,
                                 中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
                                 
                 CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
                 
        flags：  TFD_NONBLOCK: 非阻塞模式
                 TFD_CLOEXEC: 表示当程序执行exec函数时本fd将被系统自动关闭,表示不传递
                 
    返回值:
        文件描述符(定时器对象)
```

- timerfd_settime 函数

```c
    int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, 
                        struct itimerspec *old_value);
                        
    描述:
         timerfd_settime() 用于设置新的超时时间，并开始计时,能够启动和停止定时器;
            
    参数:
        fd: 参数 fd 是 timerfd_create 函数返回的文件句柄
                                        
        flags：  1:设置的是绝对时间（TFD_TIMER_ABSTIME 表示绝对定时器）
                0: 相对时间
                
        new_value: 传入参数,指定定时器的超时时间以及超时间隔时间
        old_value: 传出参数(被赋值)如果old_value不为NULL, old_vlaue被赋值为之前定时器设置的超时时间
                 
    返回值:
        文件描述符(定时器对象)    
                       
    注意:
        1.
                struct itimerval {  
                    struct timeval it_interval; /* next value, it_interval不为0则表示是周期性定时器*/  
                    struct timeval it_value;    /* current value, Initial expiration 初始过期时间*/  
                };  
            
                (1). itimerval.it_value 和 itimerval.it_interval都为 0 表示停止定时器
                (2) 如果 itimerval.it_interval 的两个时间域都为零，则表示定时器只工作一次，
                    即到达初始过期时间后(itimerval.it_value)就停止工作，非周期性
```

- timerfd_gettime 函数

```c 
    int timerfd_gettime(int fd, struct itimerspec *curr_value);
    
     描述:
         timerfd_gettime() 函数获取距离下次超时剩余的时间
            
    参数:
        fd: 参数 fd 是 timerfd_create 函数返回的文件句柄
                                       
        curr_value: 传出参数(被赋值) 
                    curr_value.it_value 字段表示距离下次超时的时间，如果值为0，表示计时器已经解除
                    curr_value.it_value永远是一个相对值，无论TFD_TIMER_ABSTIME是否被设置
                    curr_value.it_interval 定时器间隔时间
                 
    返回值:
        文件描述符(定时器对象)    
                           
```

-  clock_gettime 函数

```shell
    int clock_gettime(clockid_t clk_id, struct timespec *tp);
    
     描述:
         clock_gettime() 函数获取时间信息
            
    参数:
        clk_id: 标识指定的时钟计数器
                    CLOCK_REALTIME: 系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,
                                    中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
                    CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
                    CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间
                    CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间
                                       
        tp: 传出参数(被赋值) 
                  
                 
    返回值:
        0: 成功
              
    注意:
        1.clock_gettime 可以用做获取当前的系统时间(clk_id 设置为 CLOCK_REALTIME)
    
```

- 注意

```shell
    1.可以用 read 函数读取计时器的超时次数, 
            uint64_t exp = 0;
            read(fd, &exp, sizeof(uint64_t)); // 其中 fd 为 timerfd_create 函数返回的文件句柄
            
    2. 例子:
            (1) 采用相对时间
                    
                    struct itimerspec new_value;
                    
                    new_value.it_value.tv_sec = 2;
                    new_value.it_value.tv_nsec = 0;
                    new_value.it_interval.tv_sec = 1;
                    new_value.it_interval.tv_nsec = 0;
                    
                    tmfd = timerfd_create(CLOCK_MONOTONIC, 0);
                    ret = timerfd_settime(tmfd, 0, &new_value, NULL);
                    
            (2) 采用绝对时间
                     struct timespec now;
                     clock_gettime(CLOCK_REALTIME, &now);
                     
                      struct itimerspec new_value;
                     new_value.it_value.tv_sec = now.tv_sec + atoi(argv[1]); // it_value指的是第一次到期的时间
                     new_value.it_value.tv_nsec = now.tv_nsec; 
                     new_value.it_interval.tv_sec = 4; 
                     
                     fd = timerfd_create(CLOCK_REALTIME, 0); // 构建了一个定时器
                     timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL)
```