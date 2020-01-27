/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                   Copyright (c) 2016, All rights reserved                  *
 ******************************************************************************
 * File name  : Mb_func.c                                                     *
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

#include "mb_func.h"
#include "mb.h"

#include "hmi.h"
#include "com.h"


typedef  eMBException( *MBFunctionHandler ) ( uint8* pucFrame, uint8* pusLength );

st_func g_mb_func[] = 
{
    {MB_FUNC_READ_COILS, mb_func_RC},
    {MB_FUNC_READ_DISCRETE_INPUTS, mb_func_RDI},
    {MB_FUNC_READ_HOLDING_REGISTER, mb_func_RHR},
    {MB_FUNC_READ_INPUT_REGISTER, mb_func_RIR},
    {MB_FUNC_WRITE_SINGLE_COIL, mb_func_WSC},
    {MB_FUNC_WRITE_SINGLE_REGISTER, mb_func_WSR},    
    {MB_FUNC_WRITE_MULTIPLE_COILS, mb_func_WMC},
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, mb_func_WMR},
};




eMBErrorCode eMBRegHoldingCB( uint8 * pucRegBuffer, uint8 usAddress, uint8 usNRegs,
                 eMBRegisterMode eMode )
{
    return MB_ENOREG;
}


eMBErrorCode eMBRegCoilsCB( uint8 * pucRegBuffer, uint8 usAddress, uint8 usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB( uint8 * pucRegBuffer, uint8 usAddress, uint8 usNDiscrete )
{
    return MB_ENOREG;
}



static uint8 Error2Exception(uint8 ErrorCode);

/******************************************************************************
** Function Name    : Error2Exception
** Descriptions     : Error code to Exception code
** input parameters : ErrorCode
** Returned value   : Exception
** Global variables : None
******************************************************************************/
static uint8 Error2Exception(uint8 ErrorCode)
{
    uint8 ex_code;

    switch(ErrorCode)
    {
        case MB_ENOERR:
            
            ex_code = MB_EX_NONE;
            
            break;

        case MB_ENOREG:
            
            ex_code = MB_EX_ILLEGAL_DATA_ADDRESS;
            
            break;

        case MB_ETIMEDOUT:
            
            ex_code = MB_EX_SLAVE_BUSY;
            
            break;

        default:
            
            ex_code = MB_EX_SLAVE_DEVICE_FAILURE;
            
            break;
    }

    return ex_code;
}


/******************************************************************************
** Function Name    : mb_func_RC
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RC(puint8 pdu_buf, puint16 pdu_len)
{		
	uint8          usRegAddress;
    uint8          usCoilCount;
    uint8           ucNBytes;
    uint8          *pucFrameCur;

    eMBException    eStatus = MB_EX_NONE;
    eMBErrorCode    eRegStatus;

    if( *pdu_len == ( MB_PDU_FUNC_RHR_SIZE + MB_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( uint8)( pdu_buf[MB_PDU_FUNC_RHR_ADDR_OFF] << 8 );
        usRegAddress |= ( uint8 )( pdu_buf[MB_PDU_FUNC_RHR_ADDR_OFF + 1] );
        usRegAddress++;

        usCoilCount = ( uint8 )( pdu_buf[MB_PDU_FUNC_RHR_NREG_OFF] << 8 );
        usCoilCount |= ( uint8 )( pdu_buf[MB_PDU_FUNC_RHR_NREG_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( ( usCoilCount >= MB_PDU_FUNC_RHR_REGCNT_MIN ) &&
            ( usCoilCount <  MB_PDU_FUNC_RHR_REGCNT_MAX ) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pdu_buf[MB_PDU_FUNC_OFF];
            *pdu_len = MB_PDU_FUNC_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_COILS;
            *pdu_len += 1;

            /* Test if the quantity of coils is a multiple of 8. If not last
             * byte is only partially field with unused coils set to zero. */
            if( ( usCoilCount & 0x0007 ) != 0 )
            {
                ucNBytes = ( uint8 )( usCoilCount / 8 + 1 );
            }
            else
            {
                ucNBytes = ( uint8 )( usCoilCount / 8 );
            }
            *pucFrameCur++ = ucNBytes;
            *pdu_len += 1;

            eRegStatus =
                eMBRegCoilsCB( pucFrameCur, usRegAddress, usCoilCount,
                               MB_REG_READ );
			
            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR )
            {
                eStatus = Error2Exception( eRegStatus );
            }
            else
            {
                /* The response contains the function code, the starting address
                 * and the quantity of registers. We reuse the old values in the 
                 * buffer because they are still valid. */
                *pdu_len += ucNBytes;;
            }
        }
        else
        {
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid read coil register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}

/******************************************************************************
** Function Name    : mb_func_RDI
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RDI(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}

/******************************************************************************
** Function Name    : mb_func_RHR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RHRn(uint8 port, uint16 reg_addr,uint16 reg_num)
{
	st_com_blk_ptr com_blk  = &g_com_blk[port];
	st_mb_blk_ptr  mb_blk	= &g_mb_blk[port];
	
    uint8   ex_code = MB_EX_NONE;    
    uint8   status  = MB_ENOERR;

    puint8  reply_buf   = NULL;
    uint8   reply_len   = 0x00;
    
    un_u16  u16temp, *ptemp = &u16temp;
    
    uint8   i;

	mb_blk->pdu_buf = com_blk->txd_buff + MB_ADU_PDU_OFF;
	reply_buf = mb_blk->pdu_buf;
	

	reply_buf[reply_len++] = MB_FUNC_READ_HOLDING_REGISTER;

	ptemp->word	= reg_addr;
	reply_buf[reply_len++] = ptemp->byte.byte0;
	reply_buf[reply_len++] = ptemp->byte.byte1;
	
    ptemp->word	= reg_num;
	reply_buf[reply_len++] = ptemp->byte.byte0;
	reply_buf[reply_len++] = ptemp->byte.byte1;

	mb_blk->pdu_len = reply_len;

	mb_com_send(port);
    
    return ex_code;
}

/******************************************************************************
** Function Name    : mb_func_RHR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RHR(puint8 pdu_buf, puint16 pdu_len)
{
    uint8   ex_code = MB_EX_NONE;    
    uint8   status  = MB_ENOERR;

    puint8  reply_buf   = pdu_buf;
    uint8   reply_len   = 0x00;
    
    uint16  reg_value    = 0x00;
    uint16  reg_num     = 0x00;    
    uint8  	nbytes	= 0x00;

    un_u16  u16temp, *ptemp = &u16temp;
    
    uint8   i;

	pdu_buf++;
    nbytes	= *pdu_buf++;

	//if(nbytes != 0x01 * 2)
	//{
	//	return;
	//}

	reg_num = nbytes >> 1;

	for(i = 0x00; i < reg_num; i++)
	{
		/* get register addr */
    	ptemp->byte.byte0 = *pdu_buf++;
    	ptemp->byte.byte1 = *pdu_buf++; 

		if(ptemp->word)
		{
			g_hmi_blk.led_value |= (0x01 << i);
		}
    	else
    	{	
    		g_hmi_blk.led_value &= ~(0x01 << i);
    	}
	}   
    
    return ex_code;
}



/******************************************************************************
** Function Name    : mb_func_RIR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_RIR(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}

/******************************************************************************
** Function Name    : mb_func_RIR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WSC(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}

/******************************************************************************
** Function Name    : mb_func_WSR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WSR(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}

/******************************************************************************
** Function Name    : mb_func_WSR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WMC(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}

/******************************************************************************
** Function Name    : mb_func_WMR
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_WMR(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}

/******************************************************************************
** Function Name    : mb_func_SOE
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
uint8 mb_func_SOE(puint8 pdu_buf, puint16 pdu_len)
{
		return TRUE;
}


