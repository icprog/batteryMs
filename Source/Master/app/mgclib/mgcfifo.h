#include "stdlib.h"
#include "public/public.h"
#ifndef _MGCFIFO_H_
#define _MGCFIFO_H_
/*
 * 閺堬拷銇囩�褰掑櫤UCHAR 娑擃亜鍘撶槐? * 濮ｅ繋閲滈崗鍐閺堬拷銇囨稉绡HAR鐎涙濡�
 */
#pragma pack(1)
struct mgcfifo
{
    UINT16 fifo_size;    
    UCHAR element_size; 
    UCHAR volatile mutex;
    UINT16 volatile head;  
    UINT16 volatile tail; 
    UINT16 volatile nvalid;
    UCHAR *buff;  
};//14 BYTE

/*鐠侊紕鐣婚棁锟筋渽閻㈠疇顕惃鍕敄闂傛潙銇囩亸锟�*/
#define MGCFIFO_BUFF_CALCULATE(elements_size, fifo_size) (sizeof(struct mgcfifo) + (fifo_size)*(elements_size) )

/********************************************************************************
 * mgcfifo_alloc()
 * ret閿熸枻鎷穒f >0 fifo size((buffsize-sizeof(struct mgcfifo)/elements), else <=0 err;
 * pfifo: if success,  point to the fifo, else NULL;
 * buff: the buff, where user must alloc themsleves;
 * buffsize: sizeof buff
 * elementsize: sizeof elements
 * you must finish this func before use the fifo, and before interrupt
 * by mgc 20140708
 ********************************************************************************/
int
mgcfifo_alloc(struct mgcfifo **pfifo, char *buff, UINT16 buffsize,
    UCHAR elementsize);

int
mgcfifo_in(struct mgcfifo *pfifo, const char *srcbuff); //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹涓轰竴閿熸枻鎷積lements
int
mgcfifo_out(struct mgcfifo *pfifo, char *destbuff); //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹涓轰竴閿熸枻鎷�elements
int
mgcfifo_size(struct mgcfifo *pfifo);
int
mgcfifo_nvalid(struct mgcfifo *pfifo);

#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
