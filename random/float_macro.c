#include <stdio.h>

#define BAUDRATE			19200
#define CHAR_SIZE			11
#define TIME_PER_BIT			(1/BAUDRATE)*(1.00)
#define	TIME_PER_CHAR			(TIME_PER_BIT)*(CHAR_SIZE)		//Charater Time
//#define SILENT_INTERVAL			(TIME_PER_CHAR)*(3.5)
#define SILENT_INTERVALL			(TIME_PER_CHAR)*(3.5)
//#define SILENT_INTERVAL			(0.0021)
#define TIMEOUT_INTERVAL		(TIME_PER_CHAR)*(1.5)


int main()
{
	float 	time;	
	printf("%f, %f, %f, %f\n", TIME_PER_BIT, TIME_PER_CHAR, SILENT_INTERVALL, TIMEOUT_INTERVAL);
	printf("%f\n",(1/19200));
	time = ((float)1/19200);
	printf("%f\n",time);
	return 0;
}
