/*  readRoute.c
* 
*	GP—Bullet
*	网络2101高鹏
*   2023.12.10
*/



#include "readRoute.h"

int masklen;
char dest[100],nexthop[100];

char buf[1024];


char *getMask(int masklen){
	static char mask[1024];
	if(masklen == 32){
		strcpy(mask, "255.255.255.255");
		return mask;	
	} else if(masklen == 24){
		strcpy(mask, "255.255.255.0");
		return mask;
	} else if(masklen == 16){
		strcpy(mask, "255.255.0.0");
		return mask;
	} else if(masklen == 8){
		strcpy(mask, "255.0.0.0");
		return mask;
	} else if(masklen == 0){
		strcpy(mask, "0.0.0.0");
		return mask;
	}

	strcpy(mask, "掩码长度不是0,8,16,24,32之一");
	return mask;
}

int read_Route(char *routeName)
{
	FILE *fp1 = fopen(routeName,"r");
	//FILE *fp2 = fopen("route_table","a");
	if(fp1 == NULL ){
		puts("不能打开文件！");
		return 0;		
	}
/*
	while(!feof(fp1)){
		fscanf(fp1,"%s",c);
		printf("%s\n",c);
		
	}
*/
	
	int len;
	while(fgets(buf,1024,fp1) != NULL){
		len = strlen(buf);
		buf[len - 1] = '\0';
		
		//判断路由表前缀为Ｃ、Ｌ的

		if(buf[0] == 'C' || buf[0] == 'L'){
			printf("%s\n",buf);
			char *p = strtok(buf," ");
			while(p = strtok(NULL, " ")){
				sscanf(p,"%[0-9.]/%d",dest,&masklen);				
			}
			printf("dest：%s,masklen:%d\n",dest,masklen);
			if(dest != NULL){
				struct route_msg *msg;
				msg = (struct route_msg *)malloc(sizeof(struct route_msg)); 
				strcpy(msg->dest, dest);
				strcpy(msg->mask, getMask(masklen));
				msg->masklen = masklen;
				strcpy(msg->nexthop,"直接交付");
				route_add(msg);
				printf("msg->dest:%s,msg->mask:%s,msg->masklen:%d,msg->nexthop:%s",msg->dest,msg->mask,msg->masklen,msg->nexthop);	
			}
					
			
		} else if(buf[0] == 'S'){
			printf("%s\n",buf);
			char temp1[1024],temp2[1024],temp3[1024];
			sscanf(buf,"%s %[0-9.]/%d %s %s %[0-9.]",temp1,dest,&masklen,temp2,temp3,nexthop);
			printf("temp1:%s,dest:%s,masklen:%d,temp2:%s,nexthop:%s\n",temp1,dest,masklen,temp2,nexthop);
			if(nexthop != NULL){
				struct route_msg *msg;
				msg = (struct route_msg *)malloc(sizeof(struct route_msg)); 
				strcpy(msg -> dest, dest);
				strcpy(msg -> mask, getMask(masklen));
				msg -> masklen = masklen;
				strcpy(msg -> nexthop,nexthop);
				route_add(msg);	
				printf("msg->dest:%s,msg->mask:%s,msg->masklen:%d,msg->nexthop:%s",msg->dest,msg->mask,msg->masklen,msg->nexthop);
			}
		}		
	}
	printf("\n");
	fclose(fp1);
	return 1;
	
}





