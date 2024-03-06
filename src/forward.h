//
//  forward.h
//  bishe
//
//  Created by yangcheng03 on 2021/4/13.
//

#ifndef forward_h
#define forward_h

#include "readPcap.h"

//TTL为0
#define FORWARD_TTLERROR 0

//Direct delivery直接交付
#define FORWARD_DIRECT_DELIVERY 1

//Indirect delivery间接交付
#define FORWARD_INDIRECT_DELIVERY 2

//
#define FORWARD_NOROUTE 3



typedef struct route_msg
{
    	char dest[STRSIZE];//目的网络
	char mask[STRSIZE];//掩码
    	unsigned int masklen; //掩码长度
    	char nexthop[STRSIZE];//下一跳
} route_msg;

static route_msg route_table[100];
static int table_size = 0;

void route_Init();

void route_add(route_msg *proute);

int stud_fwd_deal(IPHeader_t *pBufferm,int number, char *my_time);

int fwd_DiscardPkt(IPHeader_t *pBuffer,int type,int number,char *my_time,char *next_Hop);


#endif /* forward_h */
