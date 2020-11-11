/*
 * llcan.h
 *
 *  Created on: Oct 18, 2020
 *      Author: nadim
 */

#include <linux/can.h>
#include <linux/can/raw.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>

#ifndef LLCAN_H_
#define LLCAN_H_


#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif


void printHelpCANsnd();
void printCANFrame(struct can_frame frame);
void createCANFrame(struct can_frame *frame, unsigned int canID, unsigned char len, const unsigned char * data) ;
int sendCANFrame_basic(const char* dev, struct can_frame frame);
int sendCANFrame(const char* dev, unsigned int canID, unsigned char len, const unsigned char * data);

#endif /* LLCAN_H_ */
