## 第8章 运输层  

#### 网络进程通信需解决的关键问题
➢进程标识（寻址）->和谁对话？
参与通信的是哪台主机的哪个进程通信？
➢相互作用模式->如何对上话？
分布式进程之间怎样进行关联与互动？
➢协议识别->用什么语言对话
通过什么样的规则实现进程之间通信？
➢质量及成本的考量(Extra＋) ->要怎样的对话质量
提供什么样的服务质量以及相应的开销？



#### 多进程对IP的复用实现多进程并发通信

端口



#### 可靠交付





TCP连接的过程CS 结构



UDP简单 端口

![image-20240106210828969](/home/bullet/snap/typora/86/.config/Typora/typora-user-images/image-20240106210828969.png)



IP层仅仅校验了首部

![image-20240106211048732](/home/bullet/snap/typora/86/.config/Typora/typora-user-images/image-20240106211048732.png)



#### TCP

#### 报文格式

变长首部

序号确认号窗口标志位

![image-20240106212730256](/home/bullet/snap/typora/86/.config/Typora/typora-user-images/image-20240106212730256.png)

flag   urgent 紧急指针 



发送窗口序号是发送指针的位置，确认号和发送窗口队尾相关确认号回来之后就把队尾往前推了，

队首ack加窗口字段，发到哪指针指到





![image-20240106224528989](/home/bullet/snap/typora/86/.config/Typora/typora-user-images/image-20240106224528989.png)

#### 流量控制

![E67252C023D960A830E169E4F13A5A6D](/home/bullet/Documents/net/E67252C023D960A830E169E4F13A5A6D.jpg)

![EFABD53843579AB185D9329745B6F32E](/home/bullet/Documents/net/EFABD53843579AB185D9329745B6F32E.jpg)

#### 连接管理

和数链层的电路交换 滑动窗口的连接不一样，

逻辑连接，虚连接，需要进行参数协商

协商： 双方的初始序号，双方的窗口值，链路通信状态的可能性

#### 为什么二次握手不行，

需要请求相应确认的过程，TCP 下面复杂的IP网络导致很多不确定性



#### 三次握手过程

![9C46A8698CB1609A7A6F3A0DEED08E12](/home/bullet/Documents/net/9C46A8698CB1609A7A6F3A0DEED08E12.jpg)

#### 拥塞控制

![BAF4B773D3247FF3DA1FDB3D80884B9E](/home/bullet/Documents/net/BAF4B773D3247FF3DA1FDB3D80884B9E.jpg)

![image-20240107123406748](/home/bullet/snap/typora/86/.config/Typora/typora-user-images/image-20240107123406748.png)

![image-20240107163206334](/home/bullet/snap/typora/86/.config/Typora/typora-user-images/image-20240107163206334.png)

 

![6D004F882C978086811891CDECF11A11](/home/bullet/Documents/net/6D004F882C978086811891CDECF11A11.jpg)



**######## 实验部分**
