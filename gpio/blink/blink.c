/* See LICENSE file for copyright and license details. */

#include <fe310/clint.h>
#include <fe310/gpio.h>

#include "hifive1.h"

#define LEDS LED_R | LED_G | LED_B
#define RTCFQ 32768

static void
wait(void)
{
	u64 next = clint_get_mtime() + RTCFQ / 2 + 1;
	while (clint_get_mtime() < next);
}

int
main(void)
{
	gpio_cfg(GPIO_OUTPUT, LEDS);

	while (1) {
		wait();
		gpio_set(LEDS);
		wait();
		gpio_clr(LEDS);
	}

	return 0;
}
