/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                   Copyright (c) 2016, All rights reserved                  *
 ******************************************************************************
 * File name  : Mb_func.h                                                     *
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
#ifndef _MB_FUNC_H_
#define _MB_FUNC_H_

#include "config.h"

#define MB_FUNC_HANDLERS_MAX                    (8)


/* Function Code: 0x01 */
#define MB_PDU_FUNC_RC_SIZE        (0x04)
#define MB_PDU_FUNC_RC_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_RC_REGCNT_MIN  (0x01)
//#define MB_PDU_FUNC_RC_REGCNT_MAX  (0x7D0)
#define MB_PDU_FUNC_RC_REGCNT_MAX  (0x30)

/* Request offset define */
#define MB_PDU_FUNC_RC_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RC_NCOIL_OFF    (MB_PDU_DATA_OFF + 2)
/* Reply offset define */
#define MB_PDU_FUNC_RC_NBYTE_OFF   (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RC_VALUE_OFF   (MB_PDU_DATA_OFF + 1)

/* Function Code: 0x02 */
#define MB_PDU_FUNC_RDI_SIZE        (0x04)
#define MB_PDU_FUNC_RDI_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_RDI_REGCNT_MIN  (0x01)
//#define MB_PDU_FUNC_RDI_REGCNT_MAX  (0x7D0)
#define MB_PDU_FUNC_RDI_REGCNT_MAX  (0x70)
/* Request offset define */
#define MB_PDU_FUNC_RDI_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RDI_NCOIL_OFF    (MB_PDU_DATA_OFF + 2)
/* Reply offset define */
#define MB_PDU_FUNC_RDI_NBYTE_OFF   (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RDI_VALUE_OFF   (MB_PDU_DATA_OFF + 1)

/* Function Code: 0x03 */
#define MB_PDU_FUNC_RHR_SIZE        (0x04)
#define MB_PDU_FUNC_RHR_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_RHR_REGCNT_MIN  (0x01)
//#define MB_PDU_FUNC_RHR_REGCNT_MAX  (0x7D)
#define MB_PDU_FUNC_RHR_REGCNT_MAX  (0x70)
/* Request offset define */
#define MB_PDU_FUNC_RHR_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RHR_NREG_OFF    (MB_PDU_DATA_OFF + 2)
/* Reply offset define */
#define MB_PDU_FUNC_RHR_NBYTE_OFF   (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RHR_VALUE_OFF   (MB_PDU_DATA_OFF + 1)

/* Function Code: 0x04 */
#define MB_PDU_FUNC_RIR_SIZE        (0x04)
#define MB_PDU_FUNC_RIR_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_RIR_REGCNT_MIN  (0x01)
//#define MB_PDU_FUNC_RIR_REGCNT_MAX  (0x7D)
#define MB_PDU_FUNC_RIR_REGCNT_MAX  (0x30)
/* Request offset define */
#define MB_PDU_FUNC_RIR_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RIR_NREG_OFF    (MB_PDU_DATA_OFF + 2)
/* Reply offset define */
#define MB_PDU_FUNC_RIR_NBYTE_OFF   (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_RIR_VALUE_OFF   (MB_PDU_DATA_OFF + 1)

/* Function Code: 0x05 */
#define MB_PDU_FUNC_WSC_SIZE        (0x04)
#define MB_PDU_FUNC_WSC_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_WSC_REGCNT_MIN  (0x01)
#define MB_PDU_FUNC_WSC_REGCNT_MAX  (0x7D)
/* Request offset define */
#define MB_PDU_FUNC_WSC_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_WSC_VALUE_OFF    (MB_PDU_DATA_OFF + 2)
/* Reply offset define */
#define MB_PDU_FUNC_WSC_NBYTE_OFF   (MB_PDU_DATA_OFF)

/* Function Code: 0x06 */
#define MB_PDU_FUNC_WSR_SIZE        (0x04)
#define MB_PDU_FUNC_WSR_ADDR_SIZE   (0x02)
/* Request & Reply offset define */
#define MB_PDU_FUNC_WSR_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_WSR_VALUE_OFF   (MB_PDU_DATA_OFF + 2)

/* Function Code: 0x0F */
#define MB_PDU_FUNC_WMC_SIZE        (0x06)
#define MB_PDU_FUNC_WMC_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_WMC_REGCNT_MIN  (0x01)
#define MB_PDU_FUNC_WMC_REGCNT_MAX  (0x7B0)
/* Request offset define */
#define MB_PDU_FUNC_WMC_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_WMC_NREG_OFF    (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_WMC_NBYTE_OFF    (MB_PDU_DATA_OFF + 4)
#define MB_PDU_FUNC_WMC_VALUE_OFF    (MB_PDU_DATA_OFF + 5)
/* Reply offset define */

/* Function Code: 0x10 */
#define MB_PDU_FUNC_WMR_SIZE_MIN    (0x07)
#define MB_PDU_FUNC_WMR_ADDR_SIZE   (0x02)
#define MB_PDU_FUNC_WMR_NREG_MIN    (0x01)
#define MB_PDU_FUNC_WMR_NREG_MAX    (0x78)

#define MB_PDU_FUNC_WMR_BYTE_SIZE   (0x01)
/* Request & Reply */
#define MB_PDU_FUNC_WMR_ADDR_OFF    (MB_PDU_DATA_OFF)
#define MB_PDU_FUNC_WMR_NREG_OFF    (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_WMR_BYTE_OFF    (MB_PDU_DATA_OFF + 4)
#define MB_PDU_FUNC_WMR_VALUE_OFF   (MB_PDU_DATA_OFF + 5)


/* Function Code: 0x42 */
#define MB_PDU_FUNC_PRC_SIZE_MIN    (0x02)  /* The min num of PRC package */
#define MB_PDU_FUNC_PRC_BYTE_SIZE   (0x01)
#define MB_PDU_FUNC_PRC_CMD_SIZE    (0x01)
/* Request & Reply offset define */
#define MB_PDU_FUNC_PRC_BYTE_OFF    (MB_PDU_DATA_OFF) 
#define MB_PDU_FUNC_PRC_CMD_OFF     (MB_PDU_DATA_OFF + 1)
#define MB_PDU_FUNC_PRC_DATA_OFF    (MB_PDU_DATA_OFF + 2)

/* Function Code: 0x55 */
#define MB_PDU_FUNC_SOE_SIZE        (0x00)
#define MB_SOE_UPLOAD_NUM           (0x08)

extern st_func g_mb_func[];
//st_func g_mb_func[];


/******************************************************************************
** Function Name    : mb_func_RC
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RC(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_RDI
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RDI(puint8 pdu_buf, puint16 pdu_len);

uint8 mb_func_RHRn(uint8 port, uint16 reg_addr,uint16 reg_num);

/******************************************************************************
** Function Name    : mb_func_RHR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RHR(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_RIR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RIR(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_RIR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WSC(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_WSR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WSR(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_WSR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WMC(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_WMR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WMR(puint8 pdu_buf, puint16 pdu_len);

/******************************************************************************
** Function Name    : mb_func_SOE
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_SOE(puint8 pdu_buf, puint16 pdu_len);

#endif
/******************************************************************************
**                               end of file 
******************************************************************************/

A simple implementation of the Modbus protocol written in C. Supports RTU, TCP mode also multiport and CRC check sum.