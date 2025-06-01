/* See LICENSE file for copyright and license details. */

#include <fe310/uart.h>

void
uart_init(volatile Uart *uart)
{
	uart_baudrate(uart, 115200);
	uart_txctrl(uart, true, UART_STOP_BITS_ONE, WM_0);
	uart_iof(uart, true, UART_IOF_TX);
}

void
uart_deinit(volatile Uart *uart)
{
	/* `tis but a hack: there is no tx fifo empty flag in the fe310 */
	uart_set_txwm_ie(uart, true);
	uart_set_txctrl_wm(uart, WM_1);
	while (!uart_is_txwm_ip(uart));

	uart_iof(uart, false, UART_IOF_TX);
	uart_set_txctrl_tx_en(uart, false);
}

int
main(void)
{
	u8 str[] = "hello uart\r\n";
	usize i, j;

	uart_init(uart0);

	for (i = 0; i < 1023; i++)
		for (j = 0; j < sizeof(str) - 1; j++)
			uart_putc(uart0, str[j]);

	uart_deinit(uart0);

	return 0;
}
