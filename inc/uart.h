#ifndef UART_H_
#define UART_H_

void abre_uart(int *uart0_filestream);

void monta_uart(int uart0_filestream);

int confere_crc(unsigned char *rx_buffer, int rx_length);

void le_temperatura(int uart0_filestream, unsigned char sub_cod, float *temp);

#endif /* UART_H_ */