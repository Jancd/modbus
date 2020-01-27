/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                 Copyright (c) 2012-2017, All rights reserved               *
 ******************************************************************************
 * File name  : mb.c                                                          *
 *                                                                            *
 * Description: Modbus RTU protocol stack, Only support UART device           *
 *                                                                            *
 * Author     : Jancd                                                         *
 *                                                                            *
 * Version    : v1.2.0                                                        *
 *                                                                            *
 * modification history                                                       *
 * --------------------                                                       *
 * Version    : v1.0.0, Jancd, C, 2016-07-20                                  *
 * 				Create Modbus RTU mode					                      *
 * Version    : v1.1.0, Jancd, A, 2016-11-07 							      *
 *				Add Modbus TCP mode     			                          *
 * Version    : v1.2.0, Jancd, A, 2017-08-06 							      *
 *				Support multiport and CRC check sum                           *
 *                                                                            *
******************************************************************************/
#include "driver.h"

#include "mb_crc.h"
#include "mb_func.h"
#include "mb.h"
#include "com.h"
#include "mb_crc.h"

st_mb_blk g_mb_blk[UART_MAX_NUM];
//st_mb_blk g_mbt_blk[SKT_MAX_NUM];
/******************************************************************************
** Function Name    : mb_com_recv
** Descriptions     : modbus receive in link layer 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_com_recv(uint8 port)
{
	st_mb_blk_ptr	mb_blk	= &g_mb_blk[port];
	
    st_com_blk_ptr 	com_blk	= &g_com_blk[mb_blk->port];   

	puint8	pbuf	= com_blk->rxd_buff;
	uint16	size	= com_blk->rxd_size;
		
    if((size < MB_ADU_SIZE_MIN)
    || (size > MB_ADU_SIZE_MAX)   
    || (mb_chk_crc16(pbuf, size)))
    {
        return FALSE;
    }

	/* Save the address field. All frames are passed to the upper layed
     * and the decision if a frame is used is done there. */
    mb_blk->req_addr = pbuf[MB_ADU_ADDR_OFF];        

    /* Return the start of the Modbus PDU to the caller. */
    mb_blk->pdu_buf = (puint8)(pbuf + MB_ADU_PDU_OFF);
    /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
     * size of address field and CRC checksum. */
    mb_blk->pdu_len = (uint8)(size - MB_ADU_PDU_OFF - MB_ADU_SIZE_CRC);
    
    /* Save function code which need to process */
    mb_blk->code = mb_blk->pdu_buf[MB_PDU_FUNC_OFF];
    
    return TRUE;
}

/******************************************************************************
** Function Name    : mb_com_send
** Descriptions     : modbus send in link layer
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_com_send(uint8 port)
{    
    st_mb_blk_ptr   mb_blk  = &g_mb_blk[port];
	
	st_com_blk_ptr 	com_blk	= &g_com_blk[port];

    puint8  txd_buf = NULL;
    uint8   txd_len = 0x00; 
    un_u16	crc16, *pcrc16 = &crc16;

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if(com_blk->recv_state != COM_RX_IDLE)
    {
        return FALSE;
    }        
  
    /* First byte before the Modbus-PDU is the slave address. */
    txd_buf = (puint8)(mb_blk->pdu_buf - MB_ADU_PDU_OFF);         
    txd_buf[txd_len++] = mb_blk->addr;   
    
    /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
    txd_len += mb_blk->pdu_len;

    /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */	
	pcrc16->word	= mb_chk_crc16(txd_buf, txd_len);
    txd_buf[txd_len++]	= pcrc16->byte.byte0;        
    txd_buf[txd_len++] 	= pcrc16->byte.byte1;    
    
    com_iSend_buf(port, txd_buf, txd_len);

     
    return TRUE;
}

/******************************************************************************
** Function Name    : mb_com_init
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
bool mb_com_init(uint8 addr, uint8 port, uint16 baudrate, uint8 parity)
{	
    st_mb_blk_ptr   mb_blk  = &g_mb_blk[port];

    memset(mb_blk, 0x00, sizeof(st_mb_blk));
    
    /* Assert slave_address */
    if((addr == MB_ADDR_BROADCAST) 
    || (addr < MB_ADDR_MIN) 
    || (addr > MB_ADDR_MAX))
    {
        return FALSE;
    }
    else
    {      
		mb_blk->port	= port;

        mb_blk->addr	= addr;
		mb_blk->baudrate= baudrate;
		mb_blk->parity	= parity;

		mb_blk->function= g_mb_func;
		
        /* initialize port layor: usart and timer */     
    	return UART_Configuration(mb_blk->port, mb_blk->baudrate, mb_blk->parity);    
    }
}

/******************************************************************************
** Function Name    : mb_com_event
** Descriptions     : 
** input parameters : None
** Returned value   : None
** Global variables : None
******************************************************************************/
void mb_com_event(uint8 port)
{    
	uint8	ex_code	= MB_EX_ILLEGAL_FUNCTION;
	
    st_mb_blk_ptr   mb_blk  = &g_mb_blk[port]; 
        
    uint8 i;   
                
    for(i = 0x00; i < MB_FUNC_HANDLERS_MAX; i++)
    {
        /* No more function handlers registered. Abort. */
        if(g_mb_func[i].func_code == MB_FUNC_NONE)
        {                        
            break;
        }
        
        if(g_mb_func[i].func_code == mb_blk->code)
        {
            ex_code = mb_blk->function[i].func_handle(mb_blk->pdu_buf, 
													 &mb_blk->pdu_len);              
            break;
        }              
    }
#if 0
    /* If the request was not sent to the broadcast address we
     * return a reply. */
    if(mb_blk->req_addr != MB_ADDR_BROADCAST)
    {
        if(ex_code != MB_EX_NONE)
        {
            /* An exception occured. Build an error frame. */
            mb_blk->pdu_len = 0x00;
            
            mb_blk->code |= MB_FUNC_ERROR;
            mb_blk->pdu_buf[mb_blk->pdu_len++] = mb_blk->code;            
            mb_blk->pdu_buf[mb_blk->pdu_len++] = ex_code;
        }
                 
        mb_com_send(mb_blk->port);
        
    }
#endif
}

/******************************************************************************
**                               end of file 
******************************************************************************/
                                                                               
