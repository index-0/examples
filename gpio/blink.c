/* See LICENSE file for copyright and license details. */

#include <fe310/aon.h>
#include <fe310/gpio.h>

#define PINS GPIO_PIN(19) | GPIO_PIN(21) | GPIO_PIN(22)
#define RTCFQ 32768

static void
wait(u32 s)
{
	u64 next = rtc_get_count() + RTCFQ * s + 1;
	while (rtc_get_count() < next);
}

int
main(void)
{
	/* enable rtc */
	rtc_cfg(true, 0);

	gpio_cfg(GPIO_OUTPUT, PINS);

	while (1) {
		wait(1);
		gpio_set(PINS);
		wait(1);
		gpio_clr(PINS);
	}

	return 0;
}
