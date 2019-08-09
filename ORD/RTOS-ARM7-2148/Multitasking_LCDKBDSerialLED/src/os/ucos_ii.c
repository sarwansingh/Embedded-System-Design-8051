/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2006, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File    : uCOS_II.C
* By      : Jean J. Labrosse
* Version : V2.83
*********************************************************************************************************
*/

#define  OS_GLOBALS                           /* Declare GLOBAL variables                              */
#include <ucos_ii.h>


#define  OS_MASTER_FILE                       /* Prevent the following files from including includes.h */
#include <os_core.cxx>
#include <os_flag.cxx>
#include <os_mbox.cxx>
#include <os_mem.cxx>
#include <os_mutex.cxx>
#include <os_q.cxx>
#include <os_sem.cxx>
#include <os_task.cxx>
#include <os_time.cxx>
#include <os_tmr.cxx>
