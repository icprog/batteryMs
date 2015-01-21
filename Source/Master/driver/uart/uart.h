#ifndef _UART_H_
#define _UART_H_
int scia_init(void);
void test_SCIA(void);
void SCIA_putChar(UINT8 ch);
int scia_send( const char *data,  UINT8 datalen);

#endif

