#ifndef __aodv_rtable_h__
#define __aodv_rtable_h__

#include <assert.h>
#include <config.h>
#include <sys/types.h>
#include <lib/bsd-list.h>
#include <scheduler.h>

#define CURRENT_TIME    Scheduler::instance().clock()
#define INFINITY2        0xff

