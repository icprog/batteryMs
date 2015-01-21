/*********************************************************
 *
 * (UCT)
 *
 * File Description
 *
 *   $Revision: 2460 $
 *   $Author: huhai $
 *   $Date: 2014-07-31 15:15:29 +0800 (周四, 31 七月 2014) $
 *
 *********************************************************/
#include "stdlib.h"
#include "stdio.h"
#include "public/types.h"
#include "public\public.h"
#include "public/drivers.h"
#include "mgclib/mgcstring.h"

int
printf_array_hex(const char *hexarray, int len)
{
  int i = 0;
  for (i = 0; i < len; i++)
    {
      (void) mprintf("%02x ", hexarray[i] & 0x0ff);
    }
  return 0;
  //printf("\r\n");
}

void
printf_ch_hex(char ch)
{
  (void) mprintf("%02x ", ch & 0x0ff);
}

void
uart_send(const UCHAR *data, UINT16 len)
{

  UINT16 i = 0;
  for (i = 0; i < len; i++)
    {

      debug_uart_send(data[i]);
    }
}
void
debug_uart_send_str(char *data)
{
  while (*data)
    {
      debug_uart_send((UINT8) *data++);
    }
}
