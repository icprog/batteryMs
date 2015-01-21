/*********************************************************
 *
 * (UCT)
 *
 * File Description
 *
 *   $Revision: 3404 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:15 +0800 (周一, 01 九月 2014) $
 *
 *********************************************************/

#include "public/public.h"
#include <string.h>
#include <stdlib.h>
//
//void
//delayus(UINT16 n)
//{
//  while (n--)
//    ;
//} 

/*
 * �����?Ӳ���汾
 * 4�ֽ�	1�ֽ�
 */
UINT8
ver_hardware_get(void)
{
  UINT8 ver = HARDWARE_VER;
  return ver;
}

static UINT32
ver_software_get(void)
{
  char svn_ver[] = BMS_VERSION;
  char str_mask[] = "$Revision: ";
  int ver_svn_numb = 0;
  char *ptr;
  ptr = strstr(svn_ver, str_mask);

  if (ptr != NULL)
    {
      ver_svn_numb = atoi(ptr + strlen(str_mask));
    }

  return (UINT32)ver_svn_numb;
}

static UINT8 g_simulator_mode = 0xFF;
int
set_simulator_mode(UINT8 mode)
{
  g_simulator_mode = mode;
  return (0);
}
int
get_simulator_mode(void)
{
  return (g_simulator_mode);
}

void MCU_Reset(void)
{
#ifdef _USE_BM03
            __disable_fault_irq();      // STM32 软复�?            
            NVIC_SystemReset();
#endif
}

