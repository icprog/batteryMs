/*********************************************************
*
* 晟芯源微电子有限公司(UCT)
*
* File Description
*
*   $Revision: 2342 $
*   $Author: huhai $
*   $Date: 2014-07-29 09:49:28 +0800 (鏄熸湡浜? 29 涓冩湀 2014) $
*
*********************************************************/
#include "mgclib/mgcstring.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdarg.h>
#include <string.h>

#define MAXLINE 300    //最大缓冲

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
