/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *              	   Copyright (c) 2016, All rights reserved                *
 ******************************************************************************
 * File name  : config.h                                                      *
 *                                                                            *
 * Description: 														      *
 *                                                                            *
 * Author     : Jancd                                                         *
 *                                                                            *
 * Version    : v1.0.0                                                        *
 *                                                                            *
 * modification history                                                       *
 * --------------------                                                       *
 * Version    : v1.0.0, Jancd, C, 2016-10-05                                  *
 *                                                                            *
 *                                                                            *
******************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define BIG_ENDIAN      (0x00u)

#ifndef NULL
#define NULL            ((void*)0)
#endif

typedef unsigned char   uint8;
typedef signed char     sint8;
typedef unsigned short  uint16;
typedef signed short    sint16;
typedef unsigned int    uint32;
typedef signed int      sint32;

typedef volatile uint8  vuint8;
typedef volatile uint16 vuint16;
typedef volatile uint32 vuint32;

typedef uint8*          puint8;
typedef sint8*          psint8;
typedef uint16*         puint16;
typedef sint16*         psint16;
typedef uint32*         puint32;
typedef sint32*         psint32;
typedef void*           pvoid;

typedef uint8 (*handle)(puint8 , puint16);

typedef struct
{
    uint8	func_code;
    handle	func_handle;
}st_func, *st_func_ptr;

typedef union
{
    uint16  word;

    struct
    {    
#if BIG_ENDIAN

        uint8 byte0;
        uint8 byte1;
        
#else

        uint8 byte1;
        uint8 byte0;
        
#endif
    }byte;
}un_u16;

typedef union
{
    uint32  dword;

    struct
    {
#if BIG_ENDIAN

        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        
#else

        uint8 byte3;
        uint8 byte2;
        uint8 byte1;
        uint8 byte0;
        
#endif
    }byte;
    
}un_u32;

#endif

/******************************************************************************
**                               end of file 
******************************************************************************/
                                                                               
