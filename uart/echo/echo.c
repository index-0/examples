/* See LICENSE file for copyright and license details. */

#include <fe310/prci.h>
#include <fe310/uart.h>

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
	uart_rxctrl(uart1, true, WM_0);
	uart_iof(uart1, true, UART_IOF_TXRX);
}

int
main(void)
{
	u8 rx, tx;
	u32 hfclk;

	hfclk = prci_measure_hfclk(3000);

	/* for tty */
	uart0_init(hfclk);

	/* for echo */
	uart1_init(hfclk);

	while (1) {
		for (tx = '0'; tx <= '9'; tx++) {
			/* drain stale data */
			uart_purge(uart1);

			/* echo */
			uart_putc(uart1, tx);
			rx = uart_getc(uart1);

			/* pp to tty */
			uart_putc(uart0, tx);
			uart_putc(uart0, '|');
			uart_putc(uart0, rx);
			uart_putc(uart0, ' ');
		}
		uart_putc(uart0, '\r');
		uart_putc(uart0, '\n');
	}

	return 0;
}
