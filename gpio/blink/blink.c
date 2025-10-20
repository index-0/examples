/* See LICENSE file for copyright and license details. */

#include <fe310/delay.h>
#include <fe310/periph/gpio.h>

#include "hifive1.h"

#define LEDS LED_R | LED_G | LED_B

int
main(void)
{
	gpio_cfg(GPIO_OUTPUT, LEDS);

	while (1) {
		delay_ms(500);
		gpio_set(LEDS);
		delay_ms(500);
		gpio_clr(LEDS);
	}

	return 0;
}
