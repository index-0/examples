/* See LICENSE file for copyright and license details. */

#include <fe310/clint.h>
#include <fe310/prci.h>
#include <fe310/pwm.h>

static inline void
wait(u32 ticks)
{
	u32 next = clint_get_mtime() + ticks + 1;
	while (clint_get_mtime() < next);
}

int
main(void)
{
	const u32 freq = 240;
	u32 cmp0, cmpx, duty, hfclk, scale;

	hfclk = prci_measure_hfclk(4096);
	cmp0 = pwm_prep_cmp0(pwm1, hfclk, freq, &scale);
	pwm_cmp(pwm1, PWMCMP0, cmp0);

	pwm_cfg(pwm1, true, scale, false, true, false, false, 0, 0);
	pwm_iof(pwm1, true, false, true, true, true);

	while (1) {
		/* linear fade */
		for (duty = 0; duty <= 100; duty++) {
			cmpx = pwm_prep_cmpx(cmp0, duty);
			pwm_cmp(pwm1, PWMCMP1, cmpx); /* LED_G */
			pwm_cmp(pwm1, PWMCMP2, cmpx); /* LED_B */
			pwm_cmp(pwm1, PWMCMP3, cmpx); /* LED_R */
			wait(1024);
		}
	}

	return 0;
}
