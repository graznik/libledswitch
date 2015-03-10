#include "../../libledswitch.h"

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage: %s DEV GROUP SOCKET DATA\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* FIXME: Check user input */
	led_send(atoi(argv[1]), atoi(argv[2]));

	return 0;
}
