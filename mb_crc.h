/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                   Copyright (c) 2016, All rights reserved                  *
 ******************************************************************************
 * File name  : mb_crc.h                                                      *
 *                                                                            *
 * Description: 						                                      *
 *                                                                            *
 * Author     : Jancd                                                         *
 *                                                                            *
 * Version    : v0.00.01                                                      *
 *                                                                            *
 * modification history                                                       *
 * --------------------                                                       *
 * Version    : v0.00.01, Jancd, C, 2016-11-20                                *
 *                                                                            *
 *                                                                            *
******************************************************************************/
#ifndef _MB_CRC_H_
#define _MB_CRC_H_

#include "config.h"

/******************************************************************************
** Function Name    : mb_chk_crc16
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint16 mb_chk_crc16(puint8 pbuf, uint16 length);

/******************************************************************************
** Function Name    : mb_chk_sum
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint16 mb_chk_sum(puint8 pbuf, uint16 length);

#endif
/******************************************************************************
**                               end of file 
******************************************************************************/
                                                                               
