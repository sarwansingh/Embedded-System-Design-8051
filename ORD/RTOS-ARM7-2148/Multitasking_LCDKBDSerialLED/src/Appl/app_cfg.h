#ifndef APP_CFG_H
#define APP_CFG_H
/********************************************************************************************************
# Project Code : LPC2148
#
# Ruks LPC2148  Tasks Configuration
#
# Copyright (C) 2007 SPJ Systems Pune,India 
# All rights reserved.
#
# -------
# History
# ----------------
# Date       |Release|Author| Changes
# DD-MM-YYYY |       |      |
# ----------------------------------------------------------------------------
# 30.03.2007 |  0.1  |  BM  | Initial Version
#=============================================================================
********************************************************************************************************/

#include "std_types.h"      
#include "includes.h"
#include "os_cpu.h"

/* prototypes */
extern OS_STK  App_Stack[];
extern void  App_Task1(void *p_arg);
extern void  App_Task2(void *p_arg);
extern void  App_Task3(void *p_arg);

#endif /*APP_CFG_H*/

