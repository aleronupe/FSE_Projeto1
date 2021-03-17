#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "../inc/crc16.h"

void monta_options_uart(int uart0_filestream){
    struct termios options;
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcgetattr(uart0_filestream, &options);
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

int main(int argc, const char * argv[]) {

    ////////////// UART///////////////////////
    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
        printf("Erro - Não foi possível iniciar a UART.\n");
      
    monta_options_uart(uart0_filestream);

    unsigned char msg_TI[20] = {0x01, 0x23, 0xC1, 0x0, 0x8, 0x4, 0x0};
    short crc_TI = calcula_CRC(msg_TI, 7);
    memcpy(&msg_TI[7], crc_TI, 2);
    print("%s\n", msg_TI);

    unsigned char msg_TP[20] = {0x01, 0x23, 0xC2, 0x0, 0x8, 0x4, 0x0};
    short crc_TP = calcula_CRC(msg_TP, 7);
    memcpy(&msg_TP[7], crc_TP, 2);
    print("%s\n", msg_TP);
    
    // if (uart0_filestream != -1)
    // {
    //     printf("Escrevendo caracteres na UART ...");
    //     int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    //     if (count < 0)
    //     {
    //         printf("UART TX error\n");
    //     }
    //     else
    //     {
    //         printf("escrito.\n");
    //     }
    // }

    // sleep(1);

    // //----- CHECK FOR ANY RX BYTES -----
    // if (uart0_filestream != -1)
    // {
    //     // Read up to 255 characters from the port if they are there
    //     unsigned char rx_buffer[256];
    //     int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
    //     if (rx_length < 0)
    //     {
    //         printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
    //     }
    //     else if (rx_length == 0)
    //     {
    //         printf("Nenhum dado disponível.\n"); //No data waiting
    //     }
    //     else
    //     {
    //         //Bytes received
    //         rx_buffer[rx_length] = '\0';
    //         printf("%i Bytes lidos : %s\n", rx_length, rx_buffer);
    //     }
    // }

    close(uart0_filestream);
   return 0;
}
