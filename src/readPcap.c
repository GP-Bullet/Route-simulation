/*  readPcap.c
* 
*	GP—Bullet
*	网络2101高鹏
*   2023.12.10
*/

#include "readPcap.h"
#include "forward.h"

int read_pcap(char *pcapname){
    struct pcap_pkthdr *ptk_header;
    FramHeader_t *mac_header;
    IPHeader_t *ip_header;
    IP_Link *head, *data;
 
    FILE *fp;
    int pkt_offset, i = 0;
    int ip_len, ip_proto;
 
    char my_time[STRSIZE];
    char src_ip[STRSIZE], dst_ip[STRSIZE];
    int verLen,ttl;
    
 
    //初始化
    ptk_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));
    mac_header = (FramHeader_t *)malloc(sizeof(FramHeader_t));
    ip_header = (IPHeader_t *)malloc(sizeof(IPHeader_t));
    head = (struct IP_Link *)malloc(sizeof(IP_Link));
    data = (struct IP_Link *)malloc(sizeof(IP_Link));
 
    if ((fp = fopen(pcapname, "r")) == NULL)
    {
        printf("error: can not open pcap file\n");
        exit(0);
    }
 
    //开始读数据包
    pkt_offset = 24; //pcap文件头结构 24个字节
 
    head->next = data;
    while (fseek(fp, pkt_offset, SEEK_SET) == 0) //遍历数据包
    {
        i++;
        //pcap_pkt_header 16 byte
        memset(ptk_header, 0, sizeof(struct pcap_pkthdr));
        if (fread(ptk_header, 16, 1, fp) != 1) //读pcap数据包头结构
        {
            printf("\nread end of pcap file\n");
            break;
        }
 
        pkt_offset += 16 + ptk_header->caplen;   //下一个数据包的偏移值
 
        //读取pcap包时间戳，转换成标准格式时间
        struct tm *timeinfo;
        time_t t = (time_t)(ptk_header->ts.tv_sec);
        timeinfo = localtime(&t);
 
        strftime(my_time, sizeof(my_time), "%Y-%m-%d %H:%M:%S", timeinfo); //获取时间
        //printf("%s\n", my_time);
 
        //数据帧头 14字节
        fseek(fp, 14, SEEK_CUR); //忽略数据帧头
 
        //IP数据报头 20字节
        memset(ip_header, 0, sizeof(IPHeader_t));
        
        if (fread(ip_header, sizeof(IPHeader_t), 1, fp) != 1)
        {
            printf("%d: can not read ip_header\n\n", i);
            break;
        }
 
        printf("DstIP:%d\n",ip_header->DstIP);
        inet_ntop(AF_INET, (void *)&(ip_header->SrcIP), src_ip, 16);
 
        inet_ntop(AF_INET, (void *)&(ip_header->DstIP), dst_ip, 16);
 
        stud_fwd_deal(ip_header, i, my_time);

        ip_proto = ip_header->Protocol;
        verLen = (int)(((ip_header->Ver_HLen) & 0xf0) >> 4);
        ttl = (int)ip_header->TTL;
        
        printf("第%d个IP数据包, verLen:%d, TTL:%d, ",i, verLen, ttl);
        printf("time:%s, src_ip:%s, dst_ip:%s, ip protocol:%d\n",my_time, src_ip, dst_ip, ip_proto);
	unsigned int address; 
        inet_pton(AF_INET,src_ip,&address);

        printf ("src_ip:%s,SrcIP：%d\n", src_ip, address);
        
        printf("\n");
        ip_len = ip_header->TotalLen; //IP数据报总长度

    } // end while
 
    fclose(fp);
 
    //free(file_header);
    free(ptk_header);
    free(ip_header);
    return 1;
}
