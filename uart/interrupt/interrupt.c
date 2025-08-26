/* See LICENSE file for copyright and license details. */

#include <fe310/plic.h>
#include <fe310/prci.h>
#include <fe310/uart.h>

static void
uart1_irq(void)
{
	usize i;
	/* empty fifo and print to tty */
	for (i = 0; i < 8; i++)
		uart_putc(uart0, uart_getc(uart1));
}

static void
uart0_init(u32 hfclk)
{
	uart_baudrate(uart0, hfclk, 115200);
	uart_txctrl(uart0, true, UART_STOP_BITS_ONE, WM_0);
	uart_iof(uart0, true, UART_IOF_TX);
}

static void
uart1_init(u32 hfclk)
{
	uart_baudrate(uart1, hfclk, 115200);
	uart_txctrl(uart1, true, UART_STOP_BITS_ONE, WM_0);
	uart_rxctrl(uart1, true, WM_7);
	uart_ie(uart1, false, true);
	uart_iof(uart1, true, UART_IOF_TXRX);
	uart_purge(uart1);
}

int
main(void)
{
	u32 hfclk = prci_measure_hfclk(3000);
	usize i;

	/* for tty */
	uart0_init(hfclk);

	/* for filling */
	uart1_init(hfclk);

	plic_threshold(PLIC_PRIORITY_0);
	plic_hook(PLIC_IRQ_UART1, uart1_irq);
	plic_priority(PLIC_IRQ_UART1, PLIC_PRIORITY_1);
	plic_ie(true, PLIC_IRQ_BIT(PLIC_IRQ_UART1));

	while (1)
		/* a..z */
		for (i = 0; i <= 25; i++)
			uart_putc(uart1, i + 97);

	return 0;
}
