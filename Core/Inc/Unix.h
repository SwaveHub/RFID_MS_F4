#ifndef __UNIX_H
#define __UNIX_H

#include "stdint.h"

#define UTC_BASE_YEAR   1970
#define MONTH_PER_YEAR  12
#define DAY_PER_YEAR    365
#define SEC_PER_DAY     86400
#define SEC_PER_HOUR    3600
#define SEC_PER_MIN     60

//北京时区
#define UTC_TIMEZONE	(8)

#define BASE_YEAR   2000


typedef volatile struct {
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
}system_time_t;

extern system_time_t system_time;
void Unix_To_YMD_Time(system_time_t *time, uint32_t unix_ts);
uint32_t YMD_To_Unix_Time(const system_time_t *time);

#endif
