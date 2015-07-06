/* 
 * File:   spi.h
 * Author: noah
 *
 * Created on November 9, 2014, 11:28 AM
 */

#ifndef SPI_H
#define	SPI_H

#define SPI_TX_SUCCESS 1
#define SPI_TX_FAILURE 0
#define SPI_RX_SUCCESS 1
#define SPI_RX_FAILURE 0

void spi_setup(void);
int spi_send(char data);
int spi_recieve(char *data);

#endif	/* SPI_H */

