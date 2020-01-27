/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                   Copyright (c) 2016, All rights reserved                  *
 ******************************************************************************
 * File name  : mb.h                                                          *
 *                                                                            *
 * Description: Modbus RTU protocol stack                                     *
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
#ifndef _MB_H_
#define _MB_H_

#include "config.h"

/* Address range defined in Modbus protocol */
#define MB_ADDR_BROADCAST   (0)     /* Modbus broadcast address */
#define MB_ADDR_MIN         (1)     /* Smallest possible slave address */
#define MB_ADDR_MAX         (247)   /* Biggest possible slave address */

/* PDU defined in Modbus protocol */
#define MB_ADU_SIZE_MIN     (4)     /* Minimum size of a Modbus RTU frame */
#define MB_ADU_SIZE_MAX     (256)   /* Maximum size of a Modbus RTU frame */
#define MB_ADU_SIZE_CRC     (2)     /* Size of CRC field in PDU */
#define MB_ADU_ADDR_OFF     (0)     /* Offset of slave address in Ser-PDU */
#define MB_ADU_PDU_OFF      (1)     /* Offset of Modbus-PDU in Ser-PDU */

#define MB_PDU_SIZE_MAX     (253)   /* Maximum size of a PDU */
#define MB_PDU_SIZE_MIN     (1)     /* Function Code */
#define MB_PDU_FUNC_OFF     (0)     /* Offset of function code in PDU */
#define MB_PDU_DATA_OFF     (1)     /* Offset for response data in PDU */

/* Function code defined in Modbus protocol */
typedef enum
{
    MB_FUNC_NONE,                               /* 0x00 */
    MB_FUNC_READ_COILS,                         /* 0x01 */
    MB_FUNC_READ_DISCRETE_INPUTS,               /* 0x02 */
    MB_FUNC_READ_HOLDING_REGISTER,              /* 0x03 */
    MB_FUNC_READ_INPUT_REGISTER,                /* 0x04 */
    MB_FUNC_WRITE_SINGLE_COIL,                  /* 0x05 */
    MB_FUNC_WRITE_SINGLE_REGISTER,              /* 0x06 */
    MB_FUNC_DIAG_READ_EXCEPTION,                /* 0x07 */
    MB_FUNC_DIAG_DIAGNOSTIC,                    /* 0x08 */
    
    MB_FUNC_DIAG_GET_COM_EVENT_CNT = 0x0b,      /* 0x0b */
    MB_FUNC_DIAG_GET_COM_EVENT_LOG,             /* 0x0c */
    MB_FUNC_WRITE_MULTIPLE_COILS = 0x0f,        /* 0x0f */
    MB_FUNC_WRITE_MULTIPLE_REGISTERS,           /* 0x10 */
    MB_FUNC_OTHER_REPORT_SLAVEID,               /* 0x11 */
    MB_FUNC_READ_FILE_RECORD = 0x14,            /* 0x14 */
    MB_FUNC_WRITE_FILE_RECORD,                  /* 0x15 */
    MB_FUNC_WRITE_MASK_REGISTER,                /* 0x16 */
    MB_FUNC_READWRITE_MULTIPLE_REGISTERS,       /* 0x17 */

    MB_FUNC_READ_DEVICE_ID = 0x2b,              /* 0x2b */
    
    /* User define function codes, 65~72 & 100~110 */    
    MB_FUNC_PRC  = 0x42,          /* 0x42 */
    MB_FUNC_PLP  = 0x43,          /* 0x43 */
    
    MB_FUNC_READ_SOE_INFO       = 0x55,

    MB_FUNC_ERROR = 0x80                        /* 0x80 */
}eMBFunctionCode;

typedef enum
{
    MB_REG_READ,        /*!< Read register values and pass to protocol stack. */
    MB_REG_WRITE        /*!< Update register values. */
} eMBRegisterMode;

typedef enum
{
    MB_ENOERR,          /*!< no error. */
    MB_ENOREG,          /*!< illegal register address. */
    MB_EINVAL,          /*!< illegal argument. */
    MB_EPORTERR,        /*!< porting layer error. */
    MB_ENORES,          /*!< insufficient resources. */
    MB_EIO,             /*!< I/O error. */
    MB_EILLSTATE,       /*!< protocol stack in illegal state. */
    MB_ETIMEDOUT        /*!< timeout error occurred. */
} eMBErrorCode;

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    MB_EX_NONE = 0x00,
    MB_EX_ILLEGAL_FUNCTION = 0x01,
    MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MB_EX_ILLEGAL_DATA_VALUE = 0x03,
    MB_EX_SLAVE_DEVICE_FAILURE = 0x04,
    MB_EX_ACKNOWLEDGE = 0x05,
    MB_EX_SLAVE_BUSY = 0x06,
    MB_EX_MEMORY_PARITY_ERROR = 0x08,
    MB_EX_GATEWAY_PATH_FAILED = 0x0A,
    MB_EX_GATEWAY_TGT_FAILED = 0x0B
} eMBException;


/** <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+------------------------------------------+
 *  | TID | PID | Length | UID  |Code | Data                               |
 *  +-----------+---------------+------------------------------------------+
 *  |     |     |        |      |                                           
 * (2)   (3)   (4)      (5)    (6)                                          
 *
 * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte) 
 * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
 * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
 * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
 * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
 *
 * (1)  ... Modbus TCP/IP Application Data Unit
 * (1') ... Modbus Protocol Data Unit                                      */

#define MB_TCP_TID          (0x00)
#define MB_TCP_PID          (0x02)
#define MB_TCP_LEN          (0x04)
#define MB_TCP_UID          (0x06)
#define MB_TCP_FUNC         (0x07)

#define MB_TCP_PROTOCOL_ID  (0)   /* 0 = Modbus Protocol */
#define MB_TCP_PSEUDO_ADDR  (255)

typedef struct
{        
    /* config information, read only */
    uint8   addr;     /* Slave address defined in Modbus protocol */
    uint8   port;           /* The number of COM port */
    uint16  baudrate;       /* UART baudrate */
    uint8   parity;         /* UART parity type */

	/* ETH configuration */
	uint8	SKTn;
	
    /* Modbus protocol, ADU = ADDR + PDU + CRC */
    /* ADDR */
    uint8   req_addr;       /* request address */
    /* PDU */
    puint8  pdu_buf;        /* PDU start memory address */
    uint16  pdu_len;        /* PDU length */
    
    uint8   	code;      /* function code defiened in Modbus protocal */   
	st_func_ptr	function;
}st_mb_blk, *st_mb_blk_ptr;

extern st_mb_blk    g_mb_blk[];
extern st_mb_blk   g_mbt_blk[];


/******************************************************************************
** Function Name    : mb_com_recv
** Descriptions     : modbus receive in link layer 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_com_recv(uint8 port);

/******************************************************************************
** Function Name    : mb_com_send
** Descriptions     : modbus send in link layer
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_com_send(uint8 port);

/******************************************************************************
** Function Name    : mb_com_init
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_com_init(uint8 addr, uint8 port, uint16 baudrate, uint8 parity);

/******************************************************************************
** Function Name    : mb_com_event
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
void mb_com_event(uint8 port);

/******************************************************************************
** Function Name    : mb_net_recv
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_net_recv(uint8 SKTn);

/******************************************************************************
** Function Name    : mb_net_send
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_net_send(uint8 SKTn);

/******************************************************************************
** Function Name    : mb_net_init
** Descriptions     : protocol initialize
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_net_init(uint8 SKTn);

/******************************************************************************
** Function Name    : mb_net_event
** Descriptions     : 
** input parameters : s, socket index
** Returned value   : None
** Global variables : None
******************************************************************************/
void mb_net_event(uint8 SKTn);

#endif
/******************************************************************************
**                               end of file 
******************************************************************************/
                                                                               
