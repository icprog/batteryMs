/*********************************************************
*
* ��оԴ΢�������޹�˾(UCT)
*
* File Description
*
*   $Revision: 2342 $
*   $Author: huhai $
*   $Date: 2014-07-29 09:49:28 +0800 (星期�? 29 七月 2014) $
*
*********************************************************/
#include "mgclib/mgcstring.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdarg.h>
#include <string.h>

#define MAXLINE 300    //��󻺳�

static char outbuf[MAXLINE];
void mprintf(const char *format, ... ) {
  int i;
  va_list args;
  va_start(args, format);  
  (void)memset( outbuf, 0, MAXLINE);
  i = vsprintf(outbuf, format, args);
  va_end(args);
  debug_uart_send_str(outbuf);
  //return i;
  (void)i;
}


 
void print2bank( const char *fmt, ... ) {
  (void)fmt;
  //va_list args;
  //va_start(args, fmt);
  //(void)vsprintf(outbuf, fmt, args);
//  va_end(args);
  //debug_uart_send_str(outbuf);

}
