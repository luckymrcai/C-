# C++视频点播服务器端
## 1.项目描述
- 视频点播器是实现了一款基于RTSP流媒体视频点播的播放器功能。该项目由视频点播客户端、视频点播服务器、大并发压力测试程序三大部分构成，项目综合了《linux高级应用开发技术》和《linux基础开发》的知识体系；
- 采用大并发通信框架思想和自定义协议，基于TCP Socket封装了包含通信组件、动态数组以及线程池等技术的通信框架。以及基于RTSP的流媒体播放，基于FFMpeg的媒体数据的编解码功能。
- 服务端主要进行复杂视频列表数据的并发处理。
- 客户端从服务端获取视频列表，调用了FFMpeg进行解码进行播放。视频数据从Live555服务器获取。
## 2.开发环境
- CentOS 7.0、sqlite3、C++
## 3.技术要点
- socket、epoll、多进程、多线程、线程池、信号量、互斥锁、条件变量、高并发
## 4.服务器功能
### 4.1.接入功能
- 响应个客户端的接入请求
- 完成登录验证功能
- 管理和登记各客户端的信息
- 如果2分钟内未登陆，则关闭连接，清楚客户端信息
### 4.2.业务功能
- 用户登陆
- 响应客户端请求视频列表
- 响应客户端请求用户信息、用户视频播放记录
- 记录用户播放视频信息、播放时长的位置
- 心跳实现
### 4.3.实时日志
实现启动该日志进程后能够查看当前连接情况等信息，关闭该进程后不影响后台业务正常处理。实时刷新日志信息如下：
- 有效连接数
- 接收数据包
- 发送数据包
- 用户登录业务数
- 获取视频列表业务数
- 获取用户视频点播记录业务数
- 上传视频点播记录业务数
- 上传视频点播时长记录业务数
### 4.4.全部日志
实现日志信息写入文件。包括启动日志，登陆日志数据包，交易数据包，接收数据包，心态数据包，相关日志信息写入文本日志。文件每天生成一个。
时间：yyyy-mm-dd hh:mm:ss
功能：登陆
类型：发送/接收
用户id：xxxxxx
发送数据包（十六进制信息）：（16个字节一行）
01 92 92 13 21 31 23 32 12 32 32 33 32 52 32 52
01 92 92 13 21 31 23 32 12 32 32 33 32 52 32 52
01 92 92 13 21 31 23 32 12 32 32 33 32 52 32 52

时间：yyyy-mm-dd hh:mm:ss
功能：上传播放记录
类型：发送/接收
用户id：xxxxxx
01 92 92 13 21 31 23 32 12 32 32 33 32 52 32 52
01 92 92 13 21 31 23 32 12 32 32 33 32 52 32 52
01 92 92 13 21 31 23 32 12 32 32 33 32 52 32 52

## 5.视频点播服务器总体框架图
 ![image](https://github.com/luckymrcai/C-CPP-Server/blob/master/%E8%A7%86%E9%A2%91%E7%82%B9%E6%92%AD%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%80%BB%E4%BD%93%E6%A1%86%E6%9E%B6.jpg)
### 5.1.前置服务器
- 监听：创建TCPServer，使用epoll进行监听
- 创建新连接：对客户端的连接请求，创建新的Socket连接
- 应答：前置服务对共享内存的应答包进行监听，读取应答包，发送给指定的客户端
- 定时器扫描客户端：扫描客户端连接，断开无效的客户端连接，并清理数据，释放内存
### 5.2.解析包模块
- 解析包线程池：创建包含20个线程的线程池，用来解析数据包的数据
- 解析包回调函数：解析数据包数据，并将正确解析的业务包数据写入共享内存
### 5.3.共享内存
- 共享内存：用来保护客户端发送的业务包、服务端返回的应答包。前置服务器监听共享内存中的应答包，并读取。后置服务器监听客户端发送的业务包，并读取
### 5.4.后置服务器
- 读取业务包：读取共享内存中的业务包
### 5.5.业务处理模块
- 业务处理线程池：创建包含20个线程的线程池，用来处理业务包的数据
- 业务回调函数：根据业务包的类型，进行相应的业务处理
- 数据库：根据业务的需要对数据库进行读写访问
## 6.通信程序关键细节设计
### 6.1.断线重连处理
- 在设备开机后，设备与服务器保持长连接。由于线路等原因中间过程可能发生“断线”；此时，如果服务器监测到，则需要主动关闭socket连接，并清除保存在客户端信息；如果客户端侦测到，则在UI界面上提示网络断线，但不能重启设备端，各工作单元不能释放，而是等待网络重新连上后继续工作，因此，客户端需要定时发起连接。
### 6.2.心跳实现
- 由于线路等原因，中间过程可能发生“断线”，服务器和设备端程序都无法侦测到。为了能够及时发现断线，而启动断线重连机制，所以，在客户端应该能定时发生测试的“心跳包”。同时，为了减轻服务器的压力，服务器对应各个客户端发来的心跳包，就是丢弃；服务器维持一个客户端的心跳时长，当收到客户端的心跳包，该心跳时长则清零；则间隔时间内未收到，则心跳时长累计，一旦到了规定的时长，即服务器端很久没有收到心跳了，则服务器主动判断客户端已经断线，主动关闭socket连接
- 另外一个解决方案就是打开TCP协议的keep-alive选项，设定发送间隔，让协议栈来侦测线路（协议栈本身完成“心跳”），一旦发现线路中断，则会SOCKET API函数通知应用程序。
### 6.3.不完整包检测和处理
- 客户端发送数据包瞬间，线路发送中断，则服务器端可能就收到了不完整包，在后续又会收到完整包。因此，服务器需要对数据包的完整性进行验证，为了完成数据包的完整性检测，建议在数据包前或后添加包头或包尾标志。
### 6.4.粘包检测和处理
- 在协议栈的TCP缓冲区内，可能同时存在两笔业务或心跳包等等的数据。需要区分。解决方案：包头+包体
### 6.5.重包处理
- 服务器端需要有一套机制来判断客户端发来的数据包是否已经收到过，如果已经收到过，就直接丢弃。解决方案：通信包的包头带有每个包的流水号
## 7使用方法
- yum install sqlite-devel #安装sqlite环境
- make
