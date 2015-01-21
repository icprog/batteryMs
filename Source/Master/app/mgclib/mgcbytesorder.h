/*********************************************************
 *
 *(UCT)
 *
 * File Description
 *
 *   $Revision: 2484 $
 *   $Author: huhai $
 *   $this file is used for uart-rs232 communication between BMS-BOARD and PC
 *   $Date: 2014-08-01 11:05:59 +0800$
 *
 *********************************************************/
#ifndef _BYTES_ORDER_H_
#define _BYTES_ORDER_H_
#include "public/public.h"
#include <string.h>
#include "mgclib/mgcstring.h"
#include "mgclib/mgcprintf.h"
/******************************************************************************** 
 * host bytes order to rs232 bytes order
 * surpport for bit16 or bit32  cpu
 * test for 16bits cpu
 * useage:
 *     define the serial bytes order by  macro SERIAL_BYTES_ORDER
 *     this functions can automatic get the orders of the host 
 * by mgc 20140805
 ********************************************************************************/
//enum SERIAL_BYTES_ORDERS
//{
#define  BYTES_LITTLE_FIRST  'l'
#define  BYTES_BIG_FIRST  'b'
//};

#define SERIAL_BYTES_ORDER  BYTES_BIG_FIRST

typedef union bytesorder
{
  UCHAR order[sizeof(int)];
  UCHAR c;
} u_order;
#ifdef _USE_BM01
static const u_order g_order =
  { 'l', 'b'};
#endif
#define _USE_SYS_32BIT 1
#ifdef _USE_SYS_32BIT
static const u_order g_order =
  { 'l','0','0','b'};
#endif


#define GetHostBytesOrder()  ( g_order.c == 'l' ? 'l':'b' )

//#pragma INLINE
static void
BYTESORDER_H2S_16(UINT16 H_16, UCHAR *S_8)
{
  UINT8 i = 0;
  UINT8 *addr = (UCHAR *) &H_16;
  if (GetHostBytesOrder() == 'l')
    {
      for (i = 0; i < 2; i++)
        {
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
          S_8[1 - i] = addr[i];
#else
          S_8[i] = addr[i];
#endif
        }
    }
  else
    {
      for (i = 0; i < 2; i++)
        {
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
          S_8[i] = addr[i];
#else
          S_8[1-i] = addr[i];
#endif
        }
    }
}

//#pragma INLINE
static UINT16
BYTESORDER_S2H_16(const UCHAR *S_8)
{
  if (GetHostBytesOrder() == 'l')
    {
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
      return (UINT16)(S_8[0] * 256 + S_8[1]);
#else
      return S_8[1]*256 + S_8[0];
#endif
    }
  else
    {
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
      return (UINT16)(S_8[0] * 256 + S_8[1]);
#else
      return S_8[1]*256 + S_8[0];
#endif        
    }

}

//#pragma INLINE
static void
BYTSORDER_H2S_32(UINT32 H_32, UCHAR *S_8)
{
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
  S_8[0] = (UCHAR) ((H_32) >> (3 * 8) & 0xff);
  S_8[1] = (UCHAR) ((H_32) >> (2 * 8) & 0xff);
  S_8[2] = (UCHAR) ((H_32) >> (1 * 8) & 0xff);
  S_8[3] = (UCHAR) (((H_32) >> (0 * 8)) & 0xff);
#else
  S_8[3] = (UCHAR)((H_32)>>(3*8)&0xff);
  S_8[2] = (UCHAR)((H_32)>>(2*8)&0xff);
  S_8[1] = (UCHAR)((H_32)>>(1*8)&0xff);
  S_8[0] = (UCHAR)(((H_32)>>(0*8))&0xff);
#endif

}

//#pragma INLINE
static UINT32
BYTSORDER_S2H_32(const UCHAR *S_8)
{
  UINT32 H_32 = 0;
  UINT8 *addr = (UCHAR *) &H_32;
  UINT8 i = 0;
  if (GetHostBytesOrder() == 'l')
    {
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
      for (i = 0; i < 4; i++)
        {
          addr[i] = S_8[3 - i];
        }
#else
      (void)memcpy( addr, S_8, 4);
#endif
    }
  else
    {
#if SERIAL_BYTES_ORDER == BYTES_BIG_FIRST
      (void) memcpy(addr, S_8, 4);
#else
      for(i=0; i<4; i++)
        {
          addr[i] = S_8[3-i];
        }
#endif
    }
  return H_32;
}

//#pragma INLINE
static INT16
test_bytesorder(void)
{
  UINT32 m_32 = 0x12345678;
  UINT16 m_16 = 0;
  UINT8 s_4[] =
    { 0x12, 0x34, 0x56, 0x78 };

  mprintf(
      (const char*) "******************************test bytesorder start!******************************\r\n");
  mprintf("i have a array:");
  (void) printf_array_hex((const char*) s_4, sizeof(s_4));
  mprintf("\r\n");

  mprintf("i have a u32:%lx\r\n", m_32);

  mprintf("mem of u32 is:");
  (void) printf_array_hex((const char*) &m_32, sizeof(m_32));
  mprintf("\r\n");

  mprintf("GetHostBytesOrder():%c\r\n", GetHostBytesOrder());
  mprintf("serial bytes is:");
  (void) printf_array_hex((const char*) s_4, sizeof(s_4));
  mprintf("\r\n");

  mprintf("BYTSORDER_S2H_32 test!\r\n");
  m_32 = BYTSORDER_S2H_32(s_4);
  mprintf("bytes order convert from serial to host32 result is:0x%lx\r\n",
      m_32);
  mprintf("mem of u32 is:");
  (void) printf_array_hex((const char*) &m_32, sizeof(m_32));
  mprintf("\r\n");

  mprintf("BYTSORDER_H2S_32 test! u32:0x%lx!\r\n", m_32);
  BYTSORDER_H2S_32(m_32, s_4);
  mprintf("convert result is:");
  (void) printf_array_hex((const char*) s_4, 4);
  mprintf("\r\n");

  mprintf("BYTESORDER_S2H_16 test!\r\n");
  m_16 = BYTESORDER_S2H_16(s_4);
  mprintf("bytes order convert from serial to host16 result is:0x%x\r\n", m_16);
  mprintf("mem of u16 is:");
  (void) printf_array_hex((const char*) &m_16, sizeof(m_16));
  mprintf("\r\n");

  mprintf("BYTESORDER_H2S_16 test! u16:0x%x!\r\n", m_16);
  BYTESORDER_H2S_16(m_16, s_4);
  mprintf("convert result is:");
  (void) printf_array_hex((const char*) s_4, 2);
  mprintf("\r\n");

  mprintf(
      "******************************test bytesorder over!******************************\r\n");
  return 0;
}

#endif

