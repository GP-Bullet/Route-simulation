/*  forward.c
* 
*	GP—Bullet
*	网络2101高鹏
*   2023.12.10
*/


#include "forward.h"
#include "readPcap.h"


void route_Init(){}

void route_add(route_msg *proute)
{
    	
	strcpy(route_table[table_size].dest, proute -> dest);

	strcpy(route_table[table_size].mask, proute -> mask);
	
	route_table[table_size].masklen = proute -> masklen;

	strcpy(route_table[table_size].nexthop, proute -> nexthop);

	table_size++;

}



int fwd_DiscardPkt(IPHeader_t *pBuffer,int type,int number,char *my_time,char *next_Hop)
{
	FILE *fp = NULL;
	char *filename0 = NULL, *filename1 = NULL, *filename2 = NULL;
	char *filename3 = NULL, *filenameD = NULL;

	char src_ip[STRSIZE], dst_ip[STRSIZE];
	int verLen,ttl;
	
	//时间戳
	struct tm *newtime;
	char outfile[128];
	time_t t1;
	time(&t1);
	newtime = localtime(&t1);
	strftime(outfile, 128, "%Y-%m-%d %H:%M:%S.txt", newtime);
	//printf("outfile:%s\n", outfile);

	char file0[20] = "TTL_error_";
	char file1[20] = "Direct_";
	char file2[20] = "Indirect_";
	char file3[20] = "Noroute_";
	char fileD[20] = "Default_";
	
	filename0 = (char *)malloc(strlen(file0) + strlen(outfile));
	sprintf(filename0, "%s%s", file0, outfile);
	//printf("filename0: %s\n",filename0);

	filename1 = (char *)malloc(strlen(file1) + strlen(outfile));
	sprintf(filename1, "%s%s", file1, outfile);
	//printf("filename1: %s\n",filename1);
	
	filename2 = (char *)malloc(strlen(file2) + strlen(outfile));
	sprintf(filename2, "%s%s", file2, outfile);
	//printf("filename2: %s\n",filename2);
	

	filename3 = (char *)malloc(strlen(file3) + strlen(outfile));
	sprintf(filename3, "%s%s", file3, outfile);
	//printf("filename3: %s\n",filename3);
	
	filenameD = (char *)malloc(strlen(fileD) + strlen(outfile));
	sprintf(filenameD, "%s%s", fileD, outfile);
	//printf("filenameD: %s\n",filenameD);	

	if( type == 0){
		fp = fopen(filename0, "a+");
		printf("%s\n",filename0);
		verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

		ttl = (int)pBuffer->TTL;

		inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);

		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

		fprintf(fp,"number:%d\t\t源地址：%s\t\t目的地址：%s\t\tTTL:%d\n",number,src_ip,dst_ip,ttl);
		
		//fwrite(pBuffer, sizeof(struct IPHeader_t), 1, fp);
	} else if(type == 1) {
		fp = fopen(filename1, "a+");
			printf("%s\n",filename1);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\t下一跳：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,next_Hop,ttl);
			//fwrite(pBuffer, sizeof(struct IPHeader_t), 1, fp);	
	}else if(type == 2) {
		fp = fopen(filename2, "a+");
			printf("%s\n",filename2);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\t下一跳：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,next_Hop,ttl);
			//fwrite(pBuffer, sizeof(struct IPHeader_t), 1, fp); 
	} else if(type == 3){
		fp = fopen(filename3,"a+");
			printf("%s\n",filename3);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,ttl);
			//fwrite(pBuffer, sizeof(struct IPHeader_t), 1, fp);
	}else {
		fp = fopen(filenameD, "a+");
			printf("%s\n",filenameD);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,ttl);

			//fwrite(pBuffer, sizeof(struct IPHeader_t), 1, fp);
	}

	fclose(fp);				
}


//查找路由表,确定下一步处理流程(丢弃/处理/转发)
int stud_fwd_deal(IPHeader_t *pBuffer,int number,char *my_time)
{
    	//获取目的地址，ttl,校验和
	unsigned int dest_address = pBuffer -> DstIP;
    	unsigned int ttl = (int)pBuffer -> TTL;

	
	unsigned int IPAddress,maskAddress;

	
    	//检验TTL，如果为0则丢弃：
    	if(ttl <= 0)
    	{
		printf("TTL error 数据包丢弃！\n");
        	fwd_DiscardPkt(pBuffer,FORWARD_TTLERROR, number, my_time, NULL);
        	return 1;
    	}

	int i;
	//如果目的地址是本机，接收
	//查找路由表，下一跳为直接交付
	for(i = 0;i < table_size; i++)
	{
/*
		char destAddress[STRSIZE];
		unsigned int address;
		printf("dest:%s\n",route_table[i].dest);
		strcpy(destAddress, route_table[i].dest);
		
		inet_pton(AF_INET,destAddress,&address);
		printf("dest_address:%d,address:%d\n",dest_address,address);
*/

		//对传入的IP数据包的目的地址与路由表的掩码做与操作得出目的网络，在路由表里遍历
		printf("路由表第%d个： dest:%s,nexthop:%s\n",i,route_table[i].dest, route_table[i].nexthop);

		inet_pton(AF_INET, route_table[i].dest, &IPAddress);
		inet_pton(AF_INET, route_table[i].mask, &maskAddress);
		
		char string[1024] = "直接交付";
		if((strcmp(route_table[i].nexthop,string) == 0) && IPAddress == (dest_address & maskAddress))
		{
			printf("直接交付\n");
			fwd_DiscardPkt(pBuffer,FORWARD_DIRECT_DELIVERY, number, my_time, route_table[i].nexthop);
			return 0;
		}
	}
	unsigned int max_masklen = 0;
    	char nexthop[STRSIZE];

//    如果目的地址不是本机
//    按照最长匹配查找路由表获取下一跳

    	for(i = 0;i < table_size;i++)
   	{
       		unsigned int masklen = route_table[i].masklen;

		inet_pton(AF_INET, route_table[i].dest, &IPAddress);
		inet_pton(AF_INET, route_table[i].mask, &maskAddress);
	
		printf("IPAddress:%d,maskAddress:%d,dest_address & maskAddress:%d\n",IPAddress, maskAddress, (dest_address & maskAddress));
	
        	if (route_table[i].masklen > max_masklen && IPAddress == (dest_address & maskAddress))
        	{
            		max_masklen = masklen; 
	   		strcpy(nexthop, route_table[i].nexthop);
			printf("route_table[table_size].nexthop:%s,nexthop:%s\n",route_table[i].nexthop,nexthop);
        	} 
    	}

    //查找失败
    if(max_masklen == 0)
    {
        printf("路由表内无路由\n");
        fwd_DiscardPkt(pBuffer, FORWARD_NOROUTE, number, my_time, NULL);
        return 1;
    }

	printf("间接发送，下一跳地址：%s\n",nexthop);
    	fwd_DiscardPkt(pBuffer,FORWARD_INDIRECT_DELIVERY, number, my_time, nexthop);
    	return 0;

    //查找成功，设置ttl
//    pBuffer->TTL = ttl - 1;
    //计算校验和
//    ((unsigned short *)pBuffer)[5] = 0;
//    unsigned int sum = 0;
//    for (i = 0; i < 10; i++)
//    {
//        sum += ntohs(pBuffer[i]);
//        sum = (sum >> 16) + (sum & 0xffff);
//    }
//    ((unsigned short *)pBuffer)[5] = htons((~(unsigned short)sum));
    //发送
//    printf("发送\n");
	
}
