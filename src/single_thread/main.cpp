#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
	struct timeval StartTime;
	struct timeval EndTime;
	double TimeUse = 0;
 
	gettimeofday(&StartTime, NULL);  //测量开始
	char* pTest = NULL;
	//要测量的程序代码
	for (int i = 0; i < 100000; i++)
	{
		pTest = (char*)malloc(sizeof(char) * 10);
		free(pTest);
		pTest = NULL;
	}
	
	gettimeofday(&EndTime, NULL);   //测量结束
 
	TimeUse = 1000000 * (EndTime.tv_sec - StartTime.tv_sec) + EndTime.tv_usec - 
StartTime.tv_usec;
	printf("%f\n", TimeUse);
	//TimeUse /= 1000;  //测量结果，毫秒级别
 
	return 0;
}