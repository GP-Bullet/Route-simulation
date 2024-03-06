//
//  readPcap.h
//  bishe
//
//  Created by yangcheng03 on 2021/4/13.
//
#ifndef readPcap_h
#define readPcap_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>
#include <gtk/gtk.h>


#define BUFSIZE 10240
#define STRSIZE 1024




typedef int bpf_int32;
typedef unsigned int bpf_u_int32;
typedef unsigned short  u_short;
typedef unsigned int u_int32;
typedef unsigned short u_int16;
typedef unsigned char u_int8;

//pacp文件头结构体
 
struct pcap_file_header
{
    bpf_u_int32 magic;       /* 0xa1b2c3d4 */
    u_short version_major;   /* magjor Version 2 */
    u_short version_minor;   /* magjor Version 4 */
    bpf_int32 thiszone;      /* gmt to local correction */
    bpf_u_int32 sigfigs;     /* accuracy of timestamps */
    bpf_u_int32 snaplen;     /* max length saved portion of each pkt */
    bpf_u_int32 linktype;    /* data link type (LINKTYPE_*) */
};
 
//时间戳
struct time_val
{
    int tv_sec;         /* seconds 含义同 time_t 对象的值 */
    int tv_usec;        /* and microseconds */
};
 
//pcap数据包头结构体
struct pcap_pkthdr
{
    struct time_val ts;  /* time stamp */
    bpf_u_int32 caplen; /* length of portion present */
    bpf_u_int32 len;    /* length this packet (off wire) */
};
 
//数据帧头
typedef struct FramHeader_t
{ //Pcap捕获的数据帧头
    u_int8 DstMAC[6]; //目的MAC地址
    u_int8 SrcMAC[6]; //源MAC地址
    u_short FrameType;    //帧类型
} FramHeader_t;

//IP数据报头
typedef struct IPHeader_t
{ //IP数据报头
    u_int8 Ver_HLen;       //版本+报头长度
    u_int8 TOS;            //服务类型
    u_int16 TotalLen;       //总长度
    u_int16 ID; 	    //标识
    u_int16 Flag_Segment;   //标志+片偏移
    u_int8 TTL;            //生存周期
    u_int8 Protocol;       //协议类型
    u_int16 Checksum;       //头部校验和
    u_int32 SrcIP; 	   //源IP地址
    u_int32 DstIP; 	   //目的IP地址
} IPHeader_t;

typedef struct IP_Link
{
    IPHeader_t *IP_Data;
    struct IP_Link * next;
} IP_Link;

int read_pcap(char *pcapname);

#endif /* readPcap_h */
