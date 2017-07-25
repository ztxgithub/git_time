# 网络编程 

## 相关知识

- INADDR_ANY:监听所有本地ip地址, 是绑定地址0.0.0.0上的监听, 能收到任意一块网卡的连接；
```c

    const char LocalIP[] = "192.168.0.100";

　　SOCKADDR_IN Local;

　　Local.sin_addr.s_addr = inet_addr(LocalIP); 

　　bind(socket, (LPSOCKADDR)&Local, sizeof(SOCKADDR_IN)

```
- INADDR_LOOPBACK, 也就是绑定地址LOOPBAC, 往往是127.0.0.1, 只能收到127.0.0.1上面的连接请求

```c
        sa.sin.sin_family = AF_INET;
        sa.sin.sin_port = htons(0);
        sa.sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* 127.0.0.1 */
```
[INADDR_ANY相关资料](http://www.cnblogs.com/pengdonglin137/p/3309505.html)

- 网络进程通信
```c
    （ip地址，协议，端口）就可以标识网络的进程了，网络中的进程通信就可以利用这个标志与其它进程进行交互
     socket即是一种特殊的文件，一些socket函数就是对其进行的操作（读/写IO、打开、关闭）
```

 

    
## 接口函数

- 将主机序(小端序)转化为网络序(大端序)
```c

    uint16_t htons(uint16_t hostshort);

```

- in_addr_t inet_addr(const char *cp); 将ip字符串转化为网络字节序(大端)

- in_addr_t inet_network(const char *cp); 将ip字符串转化为主机字节序(小端)

- int inet_aton(const char * cp,struct in_addr *inp); 将ip字符串转化为网络字节序(大端)

```c

   函数说明 inet_aton()用来将参数cp所指的网络地址字符串转换成网络序，然后存于参数inp所指的in_addr结构中。
   结构in_addr定义如下
   struct in_addr
   {
       unsigned long int s_addr;
   };
   返回值 成功则返回非0值，失败则返回0。

```

- char * inet_ntoa(struct in_addr in); 将网络字节序(大端)转化为ip字符串

```c

    函数说明 inet_ntoa()用来将参数in所指的网络二进制的数字转换成网络地址，然后将指向此网络地址字符串的指针返回。
    返回值 成功则返回字符串指针，失败则返回NULL。

```

- uint32 ntohl(uint32 netlong); 将32位网络字符顺序转换成主机字符顺序

- uint16 ntohs(uint16 netshort); 将16位网络字符顺序转换成主机字符顺序

- uint16 htons(uint16 hostshort); 将16位主机字符顺序转换成网络字符顺序

- uint32_t htonl(uint32_t hostlong);; 将32位主机字符顺序转换成网络字符顺序

注意：
    ntohl,ntohs,htons,htonl这些函数不能用于float参数
    
[参考资料](http://man7.org/linux/man-pages/man3/endian.3.html)

- socket()函数

```c

    int socket(int domain, int type, int protocol)
    
    描述:
        socket()用于创建一个socket描述符（socket descriptor），它唯一标识一个socket.
    参数:
        domain:协议族(family)
            AF_INET:IP(ipv4地址32位)与端口号（16位的）的组合
            AF_UNIX:用一个绝对路径名作为地址(进程通信协议)
            
        type：指定socket类型,有SOCK_STREAM,SOCK_DGRAM
              除了指定套接字类型,它可能包括以下任何值的按位or，以修改socket（）的行为
              SOCK_NONBLOCK和SOCK_CLOEXEC
        protocol：协议类型(一般写０)
            0:自动选择type类型对应的默认协议
            IPPROTO_TCP:TCP传输协议
            IPPTOTO_UDP:UDP传输协议
            
    返回:
        成功则返回socket的描述符
        失败返回-1
        错误代码：
            1、EPROTONOSUPPORT: 参数domain 指定的类型不支持参数type 或protocol 指定的协议
            2、ENFILE: 核心内存不足, 无法建立新的socket 结构
            3、EMFILE: 进程文件表溢出, 无法再建立新的socket
            4、EACCESS: 权限不足, 无法建立type 或protocol 指定的协议
            5、ENOBUFS/ENOMEM: 内存不足
            6、EINVAL: 参数domain/type/protocol 不合法
            
    注意：并不是上面的type和protocol可以随意组合的，如SOCK_STREAM不可以跟IPPROTO_UDP组合。
         当protocol为0时，会自动选择type类型对应的默认协议。
         当我们调用socket创建一个socket时，返回的socket描述字它存在于协议族（address family，AF_XXX）空间中，
         但没有一个具体的地址。如果想要给它赋值一个地址，就必须调用bind()函数，
         否则就当调用connect()、listen()时系统会自动随机分配一个端口。
         
```

- bind()函数

```c

    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
    
    描述:
        把一个地址族中的特定地址赋给socket.
        例如对应AF_INET、AF_INET6就是把一个ipv4或ipv6地址和端口号组合赋给socket.
        通常服务器在启动的时候都会绑定一个众所周知的地址如ip地址+端口号,用于提供服务,
        客户就可以通过它来接连服务器；而客户端就不用指定,有系统自动分配一个端口号和自身的ip地址组合。
        这就是为什么通常服务器端在listen之前会调用bind(),
        而客户端就不会调用,而是在connect()时由系统随机生成一个。
    参数:
        sockfd: socket套接字
        addr：指向要绑定给sockfd的协议地址。这个地址结构根据地址创建socket时的地址协议族的不同而不同，
            如ipv4对应的是
            struct sockaddr_in {
                sa_family_t    sin_family; /* address family: AF_INET */
                in_port_t      sin_port;   /* port in network byte order */
                struct in_addr sin_addr;   /* internet address */
            };
            
            /* Internet address. */
            struct in_addr {
                uint32_t       s_addr;     /* address in network byte order */
            };
            
            #define UNIX_PATH_MAX    108
            
            struct sockaddr_un { 
                sa_family_t sun_family;               /* AF_UNIX */ 
                char        sun_path[UNIX_PATH_MAX];  /* pathname */ 
            };
            
        addrlen：地址的长度
            
    返回:
        成功: 0 
        失败返回-1
        错误代码：
            1、EPROTONOSUPPORT: 参数domain 指定的类型不支持参数type 或protocol 指定的协议
            2、ENFILE: 核心内存不足, 无法建立新的socket 结构
            3、EMFILE: 进程文件表溢出, 无法再建立新的socket
            4、EACCESS: 权限不足, 无法建立type 或protocol 指定的协议
            5、ENOBUFS/ENOMEM: 内存不足
            6、EINVAL: 参数domain/type/protocol 不合法
            
    注意：
    许多时候内核会我们自动绑定一个地址，然而有时用 户可能需要自己来完成这个绑定的过程，以满足实际应用的需要，
    最典型的情况是一个服务器进程需要绑定一个众所周知的地址或端口以等待客户来连接
         
```

- getsockname()函数

```c

    int getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen)
    
    描述:
        在客户端程序中,调用该getsockname函数必须要已经连接上服务器才有意义,因为客户端通过
        connect函数连接上服务器后,系统会自动分配端口号给客户端,想要知道客户端的ip和port则
        需要调用getsockname函数.
    参数:
        sockfd: socket套接字
        localaddr：该指针指向的内容会被赋值
            
        addrlen：地址的长度,被修改
            
    返回:
        成功: 0 
        失败返回-1
            
    注意：
         
```
- getpeername()函数

```c

    int getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);
    
    描述:
        在TCP的服务器端中,accept成功后，通过getpeername()函数来获取当前连接的客户端的IP地址和端口号
    参数:
        sockfd: socket套接字
        peeraddr：对端的ip和port
            
        addrlen：地址的长度,被修改
            
    返回:
        成功: 0 
        失败返回-1
            
    注意：
        getpeername只有在链接建立以后才调用，否则不能正确获得对方地址和端口，
        所以他的参数描述字一般是链接描述字而非监听套接口描述字。
        对于客户端来说，在调用socket时候内核还不会分配IP和端口，此时调用getsockname不会获得正确的端口和地址
        （当然链接没建立更不可能调用getpeername），当然如果调用了bind 以后可以使用getsockname。
        想要正确的到对方地址（一般客户端不需要这个功能），则必须在链接建立以后，
        同样链接建立以后，此时客户端地址和端口就已经被指定，此时是调用getsockname的时机
         
```

- listen()函数

```c

    int listen(int sockfd, int backlog);
    
    描述:
        作为一个服务器，在调用socket()、bind()之后就会调用listen()来监听这个socket
    参数:
        sockfd: socket套接字,是socket函数的返回值,listen函数的传入参数
        backlog: 相应socket可以排队的最大连接个数
            
    返回:
        成功: 0 
        失败返回-1
        EADDRINUSE：已经有其他的sockfd监听了相同的端口,该套接字没有绑定(ip和地址),
                    或者绑定的端口是临时端口,被用做其他的功能
        EBADF:sockfd is not a valid file descriptor.
        ENOTSOCK: sockfd does not refer to a socket
        EOPNOTSUPP: The socket is not of a type that supports the listen()
                                 operation.
            
    注意：
       用listen函数时,其sockfd的type一定是SOCK_STREAM
         
```

- connect()函数

```c

    int connect(int sockfd, const struct sockaddr* server_addr, socklen_t addrlen)
    
    描述:
         用于客户端建立tcp连接，发起三次握手过程。
    参数:
        sockfd: socket套接字
        server_addr: 相应socket可以排队的最大连接个数
            
    返回:
        成功: 0 
        失败返回-1
        EADDRINUSE：已经有其他的sockfd监听了相同的端口,该套接字没有绑定(ip和地址),
                    或者绑定的端口是临时端口,被用做其他的功能
        EBADF:sockfd is not a valid file descriptor.
        ENOTSOCK: sockfd does not refer to a socket
        EOPNOTSUPP: The socket is not of a type that supports the listen()
                                 operation.
            
    注意：
       用listen函数时,其sockfd的type一定是SOCK_STREAM
         
```