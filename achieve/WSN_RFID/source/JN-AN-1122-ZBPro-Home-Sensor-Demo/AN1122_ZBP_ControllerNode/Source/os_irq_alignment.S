#==============================================================================
#
#                 THIS IS A GENERATED FILE. DO NOT EDIT!
#
# MODULE:         OS
#
# COMPONENT:      os_irq_alignment.s
#
# DATE:           Thu Jul 26 02:07:20 2012
#
# AUTHOR:         Jennic RTOS Configuration Tool
#
# DESCRIPTION:    Wrapper for uncontrolled 'Alignment' exception. Calls a C Function
#                 to handle the exception.
#
#==============================================================================
#
# This software is owned by Jennic and/or its supplier and is protected
# under applicable copyright laws. All rights are reserved. We grant You,
# and any third parties, a license to use this software solely and
# exclusively on Jennic products. You, and any third parties must reproduce
# the copyright and warranty notice and any other legend of ownership on each
# copy or partial copy of the software.
#
# THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
# ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
# BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
# INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
#
# Copyright Jennic Ltd. 2009 All rights reserved
#
#==============================================================================

    .globl _IRQ_Alignment
    .extern _os_CFunc_W
_IRQ_Alignment:
    .extern _os_vAPP_isrAlignmentException
    b.addi r1,r1,-8
    b.sw   0(r1),r9
    b.sw   4(r1),r5
    b.ori  r5,r0,_os_vAPP_isrAlignmentException
    b.jal  _os_CFunc_W
    b.lwz  r9,0(r1)
    b.lwz  r5,4(r1)
    b.addi r1,r1,8
    b.rfe
