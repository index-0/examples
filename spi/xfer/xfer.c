/* See LICENSE file for copyright and license details. */

#include <fe310/prci.h>
#include <fe310/spi.h>

static void
spi_init(volatile Spi *spi, u32 baudrate)
{
	spi_baudrate(spi, prci_measure_hfclk(4096), baudrate);
	spi_fmt(spi, SPI_PROTO_S, SPI_ENDIAN_MSB, SPI_DIR_RX, 8);
	spi_sckmode(spi, SPI_SCKMODE_0);
	spi_csid(spi, SPI_CS0);

	spi_iof_cs(spi, true, SPI_IOF_CS0);
	spi_iof_dq(spi, true, SPI_IOF_DQS);
}

int
main(void)
{
	u8 txbuf[5] = "hello";
	u8 rxbuf[5], c;

	spi_init(spi1, 100000);

	/* send txbuf and ignore rx */
	spi_xferb(spi1, txbuf, NULL, 5);

	/* send single char and ignore rx */
	spi_xfer(spi1, '\n');

	/* send 0xFF and receive rxbuf */
	spi_xferb(spi1, NULL, rxbuf, 5);

	/* send txbuf and receive rxbuf */
	spi_xferb(spi1, txbuf, rxbuf, 5);

	/* send and receive single char */
	c = spi_xfer(spi1, '\n');

	return c;
}
