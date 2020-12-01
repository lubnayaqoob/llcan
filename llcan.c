/*
 * llcan.c
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

#include <poll.h>

#include "llcan.h"

static volatile int stop_receiving = 0;

ssize_t
write_with_retry (int fd, const void* buf, size_t size)
{
	ssize_t ret;
	while (size > 0) {
		do
		{
			ret = write(fd, buf, size);
		} while ((ret < 0) && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK));
		if (ret < 0)
			return ret;
		size -= ret;
		buf += ret;
	}
	return 0;
}

void printHelpCANsnd() {
	printf("usage: ./cansnd <CAN-IF> <CAN-ID> <Data-length> <DATA>\n"
			"Examples:\n"
			"\t./cansnd vcan0 0x123 8 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08\n"
			"\t./cansnd vcan0 0x123 4 0x01 0x02 0x03 0x04 \n");
}

void printCANFrame(struct can_frame frame) {
	unsigned char len = frame.can_dlc;
	printf("%X:\t[%d]\t",
			frame.can_id,
			len
	);
	//printf("printing frames");
	for (int i=0;i<len;i++) {
		printf("%0X  ",frame.data[i]);
	}
	/*
	if (len < 8) {
		printf("|\t");
		for(int i=len; i<8; i++){
			printf("%#x  ",frame.data[i]);
		}
	}
	 */
	printf("\n");
}

void createCANFrame(struct can_frame *frame, unsigned int canID, unsigned char len, const unsigned char * data) {
	frame->can_id = canID;
	frame->can_dlc = len;
	for (int i = 0; i < len ; i++) {
		frame->data[i] = data [i];
	}
}

int sendCANFrame_basic(const char* dev, struct can_frame frame) {
	int skt = socket( PF_CAN, SOCK_RAW, CAN_RAW );
	struct ifreq ifr;
	strcpy(ifr.ifr_name, dev);

	if (0 > ioctl(skt, SIOCGIFINDEX, &ifr) ) {
		printf("Error while opening the device %s\n", dev);
		return -1;
	}

	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind( skt, (struct sockaddr*)&addr, sizeof(addr) );

	int bytes_sent = write_with_retry( skt, &frame, sizeof(frame) );

	if(bytes_sent < 0) {
		printf("Error while sending - %d\t errno: %d\n", bytes_sent, errno);
	}
	return close(skt);
}

int sendCANFrame(const char* dev, unsigned int canID, unsigned char len, const unsigned char * data) {
	struct can_frame frame;
	createCANFrame(&frame, canID, len, data);
	return sendCANFrame_basic(dev, frame);
}

void startCANReceive(const char* dev, void (*fn) (struct can_frame frame)) {
	int skt = socket( PF_CAN, SOCK_RAW, CAN_RAW );
	struct ifreq ifr;
	strcpy(ifr.ifr_name, dev);
	if (0 > ioctl(skt, SIOCGIFINDEX, &ifr) ) {
		printf("Error while opening the device %s\n", dev);
		return;
	}
	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind( skt, (struct sockaddr*)&addr, sizeof(addr) );
	struct can_frame frame;
	struct pollfd fd[1];
	int poll_result;
	fd[0].fd = skt;
	fd[0].events = POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;
	while (!stop_receiving) {
		poll_result = poll(fd, 1, -1);
		if (poll_result > 0) {
			int bytes_read = read( skt, &frame, sizeof(frame) );
			if (bytes_read > 0)
				fn(frame);
			else
				printf("Error while reading CAN frame\n");
		}
	}
	close(skt);
}

void stopCANReceive() {
	stop_receiving=1;
}
