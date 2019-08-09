#ifndef STD_TYPES_H
#define STD_TYPES_H
/******************************************************************************
 Project Code : LPC2148

 standard data types used by SPJ systems for all development

 Copyright (C) 2007 SPJ Systems Pune,India 
 All rights reserved.

 -------
 History
 ----------------
 Date       |Release|Author| Changes
 DD-MM-YYYY |       |      |
 ----------------------------------------------------------------------------
 30.03.2007 |  0.1  |  BM  | Initial Version
=============================================================================

******************************************************************************/
#ifndef NULL
#define NULL    (0)	
#endif
#ifndef FALSE
#define FALSE   (0)
#endif
#ifndef TRUE
#define TRUE    (1)
#endif

/* Typedefs for standard data types */

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef char            int8;
typedef short           int16;
typedef int             int32;

typedef unsigned char	BOOL;

#endif /* STD_TYPES_H*/

