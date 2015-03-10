/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "libledswitch.h"

/**
 * Send a code word
 * @param codeword   /^[10S]*$/  -> see get_codeword
 * @param ntimes     Number of times to send the code word
 */
void send_codeword(char *codeword, int ntimes)
{
	int n;

	for (n = 0; n < ntimes; n++) {
		int i = 0;

		while (codeword[i] != '\0') {
			switch(codeword[i]) {
			case '0':
				send_0();
				break;
			case '1':
				send_1();
				break;
			}
			i++;
		}
		send_sync();
	}
}

/**
 * Sends a Tri-State "0" Bit
 *            _
 * Waveform: | |___
 */
void send_0(void)
{
	transmit(1, 3);
}
/**
 * Sends a Tri-State "1" Bit
 *            ___
 * Waveform: |   |_
 */
void send_1(void)
{
	transmit(3, 1);
}

/**
 * Sends a "Sync" Bit
 *            _
 * Waveform: | |_______________________________
 */

void send_sync(void)
{
	transmit(1, 31);
}

void transmit(int nhigh, int nlow)
{
	/*
	 * FIXME: 350 is the pulse length in us.
	 * This should be a parameter in the future,
	 * depending on the encoder chip within the
	 * remote control.
	 */
	bcm2835_gpio_write(PIN, HIGH);
	delayMicroseconds(440 * nhigh);
	bcm2835_gpio_write(PIN, LOW);
	delayMicroseconds(440 * nlow);
}

/**
 * Configure struct for the XXX encoder
 * @param xxx     Pointer to a xxx instance
 */
int xxx_init(Encoder *xxx)
{
	xxx->preamble = "1010110001100000000";
	xxx->pulse_len = 440;

	return 0;
}

/**
 * Emulate an encoder chip
 * @param *enc   Pointer to a encoder instance
 * @param *data  Data to send
 */
int socket_ctrl(Encoder *enc, char *data)
{
	/* Calculate code word size */
	size_t s = strlen(enc->preamble) + strlen(data) + 1;

	/* Generate the code word */
	char codeword[s];

	snprintf(codeword, s, "%s%s", enc->preamble, data);

#ifdef DEBUG
		syslog(LOG_NOTICE, "codeword: %s\n", codeword);
#endif

	/* Initialize the IO pin */
	if(!bcm2835_init()) {
		fputs("Cannot init bcm2835\n", stdout);
		return -1;
	}
	/* Set the pin to be an output */
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

	/* Send the codeword */
	send_codeword(codeword, 10);

	return 0;
}

char *byte_to_binary(uint x)
{
	static char b[9];
	b[0] = '\0';

	int z;
	for (z = 16; z > 0; z >>= 1) {
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}

int led_send(uint dev, uint data)
{
	Encoder encoder;
	char *codeword;

#ifdef DEBUG
	syslog(LOG_NOTICE, "dev: %d, data: %d", dev, data);
#endif

	switch (dev) {
	case 0:
		xxx_init(&encoder);
		break;
	default:
		syslog(LOG_ERR, "Received unknown socket type %d", dev);
		return EXIT_FAILURE;
	}

	codeword = byte_to_binary(data);
	socket_ctrl(&encoder, codeword);

	return EXIT_SUCCESS;
}
