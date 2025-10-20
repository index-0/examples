/* See LICENSE file for copyright and license details. */

#include <fe310/core/plic.h>
#include <fe310/periph/gpio.h>
#include <fe310/periph/prci.h>
#include <fe310/periph/uart.h>

#include "hifive1.h"

static void
pin3_isr(void)
{
	const char risemsg[] = "gpio13: rise\n";
	const char fallmsg[] = "gpio13: fall\n";

	/* on rise */
	if ((gpio_get_ip(GPIO_INT_RISE) & PIN_3) != 0) {
		uart_putb(uart0, risemsg, sizeof(risemsg));
		/* clear ip */
		gpio_ip(GPIO_INT_RISE, PIN_3);
	}

	/* on fall */
	if ((gpio_get_ip(GPIO_INT_FALL) & PIN_3) != 0) {
		uart_putb(uart0, fallmsg, sizeof(fallmsg));
		/* clear ip */
		gpio_ip(GPIO_INT_FALL, PIN_3);
	}
}

static void
uart0_init(void)
{
	uart_baudrate(uart0, prci_measure_hfclk(3000), 115200);
	uart_txctrl(uart0, true, UART_STOP_BITS_ONE, WM_0);
	uart_iof(uart0, true, UART_IOF_TX);
}

int
main(void)
{
	/* for tty */
	uart0_init();

	gpio_cfg(GPIO_INPUT_PULLUP, PIN_3);

	gpio_ie(GPIO_INT_RISE, true, PIN_3);
	gpio_ie(GPIO_INT_FALL, true, PIN_3);

	plic_threshold(PLIC_PRIORITY_0);

	plic_hook(PLIC_IRQ_PIN(PIN_3), pin3_isr);
	plic_priority(PLIC_IRQ_PIN(PIN_3), PLIC_PRIORITY_1);
	plic_ie(true, PLIC_IRQ_PIN_BIT(PIN_3));

	/* wait for interrupt */
	while (1) __asm__ volatile("wfi");

	return 0;
}
