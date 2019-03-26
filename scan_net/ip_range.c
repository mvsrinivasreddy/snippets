#include<stdio.h>
#include<string.h>
int main()
{
	char wanip[16]="10.0.0.15";
	char *startpt;
	char iprange[16]="";
	unsigned char values[4];
	//sscanf( wanip, "%hhu.%hhu.%hhu.%hhu",&values[0], &values[1], &values[2],&values[3] );
	sscanf( wanip, "%hhu.%hhu.%hhu",&values[0], &values[1], &values[2]);
		int k=0;
		while (k <3){
			printf("d:%hhu\n",values[k]);
			k++;
		}
	
	startpt= strtok(wanip,".");
	printf("%s\n", iprange);
	int i=0;
	while (i<3){
	 i++;
	 strcat(iprange,startpt);
	 strcat(iprange,".");
	 startpt  = strtok(NULL,".");
	}
	printf("%s", iprange);
}
