/* See LICENSE file for copyright and license details. */

#include <fe310/clint.h>
#include <fe310/gpio.h>

#define PINS GPIO_PIN(19) | GPIO_PIN(21) | GPIO_PIN(22)
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
	gpio_cfg(GPIO_OUTPUT, PINS);

	while (1) {
		wait(1);
		gpio_set(PINS);
		wait(1);
		gpio_clr(PINS);
	}

	return 0;
}
