#include <signal.h>
#include <poll.h>

#include "llcan.h"

static volatile int stop_main = 0;

void printHelp() {
	printf("usage: ./canrcv <CAN-IF> \n"
			"Example:\n"
			"\t./canrcv vcan0 \n");
}

void handle_signal() {
	printf("received signal ... stopping\n");
	// stop_main = 1;
	stopCANReceive();

}

int main (int argc, char* argv[]) {
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	char dev[16];

	if (argc < 2) {
		printHelp();
		return -1;
	} else {
		strcpy(dev, argv[1]);
	}

	startCANReceive(dev,printCANFrame);

#if 0
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

	struct can_frame frame;

	struct pollfd fd[1];
	int poll_result;
	fd[0].fd = skt;
	fd[0].events = POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;


	while (!stop_main) {
		poll_result = poll(fd, 1, -1);
		if (poll_result > 0) {
			int bytes_read = read( skt, &frame, sizeof(frame) );
			if (bytes_read > 0)
				printCANFrame(frame);
			else
				printf("Error while reading CAN frame\n");
		}
	}
#endif
	return 0;
}

