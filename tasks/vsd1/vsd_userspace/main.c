#include <stdio.h>
#include <sys/ioctl.h>
#include "vsd_ioctl.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define err(ret_code, msg) printf("%s\n", msg); \
 						ret = ret_code; \
						goto finally;

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;

	const char * vsd_dev_path = "/dev/vsd";
	int vsd_descriptor = open(vsd_dev_path, O_RDONLY);

	if (vsd_descriptor == -1) {
		err(EXIT_FAILURE, "Failed to open device\n")
	}

	if (argc < 2) {
		err(EXIT_FAILURE, "Not enough arguments\n")
	}

	const char * get_cmd = "size_get";
	const char * set_cmd = "size_set";

	if (strcmp(argv[1], get_cmd) == 0) {
		vsd_ioctl_get_size_arg_t size_wrapper;

		if (ioctl(vsd_descriptor, VSD_IOCTL_GET_SIZE, &size_wrapper)) {
			err(EXIT_FAILURE, "Failed to get size\n")
		}

		printf("%lu\n", size_wrapper.size);

	} else if (strcmp(argv[1], set_cmd) == 0) {
		if (argc < 3) {
			err(EXIT_FAILURE, "Not enough arguments\n")			
		}

		int size = atoi(argv[2]);

		vsd_ioctl_set_size_arg_t size_wrapper = {.size = size};

		if (ioctl(vsd_descriptor, VSD_IOCTL_SET_SIZE, &size_wrapper)) {
			err(EXIT_FAILURE, "Failed to set size\n")
		}
	} else {
		printf("Command: %s\n", argv[1]);
		err(EXIT_FAILURE, "No such command\n")
	}

finally:
	close(vsd_descriptor);
	return ret;
}
