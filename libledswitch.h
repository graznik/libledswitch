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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bcm2835.h>
#ifdef DEBUG
#include <syslog.h>
#endif

#define PIN RPI_GPIO_P1_07 /* GPIO 4 */

#define MODE_N    0b10001
#define MODE_P    0b01011
#define POWER     0b00001
#define SPEED_N   0b01101
#define SPEED_P   0b01111
#define LIGHT     0b00100
#define BRIGHT_N  0b00110
#define BRIGHT_P  0b00101
#define PC100     0b00111
#define PC50      0b01000
#define PC25      0b01001

struct encoder{
	char *preamble;
	uint pulse_len;
};

void send_codeword(char *, int);
void send_0(void);
void send_1(void);
void send_sync(void);
void transmit(int, int);
int  xxx_init(struct encoder *);
int  led_ctrl(struct encoder *, char *);
char *byte_to_binary(uint);
int  led_send(uint, uint);
