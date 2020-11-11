
#include <string.h>
#include <stdio.h>
#include "llcan.h"



int parseArgsforCansnd(int argc, char *argv[],
	unsigned int *canID, unsigned char *len, unsigned char *can_data, char *dev)
{
	if (argc < 5)
	{
		printHelpCANsnd();
		return -1;
	}
	else
	{
		strcpy(dev, argv[1]);
		sscanf(argv[2], "%X", canID);
		sscanf(argv[3], "%hhd", len);
		if ((argc - *len) != 4)
		{
			printHelpCANsnd();
			return -1;
		}
	}
	for (int i = 0; i < *len; i++)
	{
		sscanf(argv[4 + i], "%hhX",
			   &can_data[i]);
	}
	return 0;
}
int main(int argc, char *argv[])
{
	unsigned int canID;
	unsigned char len, arg_data[8];
	char dev[16];
	if (parseArgsforCansnd(argc, argv, &canID, &len, arg_data, dev))
	{
		printf("error parsing command line arguments\n");
	}
	sendCANFrame(dev, canID, len,
				 arg_data);
	return 0;
}
