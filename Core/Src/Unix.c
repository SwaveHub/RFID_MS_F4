#include "Unix.h"

system_time_t system_time = {
	.year = 1970,
	.month = 1,
	.day = 1,
	.hour = 0,
	.minute = 0,
	.second = 0,
};

const unsigned char g_day_per_mon[MONTH_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/*
 * 功能：
 *     判断是否是闰年
 * 参数：
 *     year：需要判断的年份数
 *
 * 返回值：
 *     闰年返回1，否则返回0
 */
static unsigned char applib_dt_is_leap_year(unsigned short year)
{

    if ((year % 400) == 0) {
        return 1;
    } else if ((year % 100) == 0) {
        return 0;
    } else if ((year % 4) == 0) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * 功能：
 *     得到每个月有多少天
 * 参数：
 *     month：需要得到天数的月份数
 *     year：该月所对应的年份数
 *
 * 返回值：
 *     该月有多少天
 *
 */
static unsigned char applib_dt_last_day_of_mon(unsigned char month, unsigned short year)
{
    if ((month == 0) || (month > 12)) {
        return g_day_per_mon[1] + applib_dt_is_leap_year(year);
    }

    if (month != 2) {
        return g_day_per_mon[month - 1];
    } else {
        return g_day_per_mon[1] + applib_dt_is_leap_year(year);
    }
}


void Unix_To_YMD_Time(system_time_t *time, uint32_t unix_ts)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;


    //将时间戳值转化成天数。通过天数可以比较方便地算出年、月、日。
    int days = unix_ts / SEC_PER_DAY;
    //这个时间戳值的年。
    int yearTmp = 0;
    int dayTmp = 0;
    //使用夹逼法计算 days 天中包含的年数。
    for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++) {
        dayTmp = (DAY_PER_YEAR + applib_dt_is_leap_year(yearTmp)); //这一年有多少天？
        if (days >= dayTmp) //条件成立，则 yearTmp 即是这个时间戳值所代表的年数。
        {
           days -= dayTmp;
        }
        else
        {
           break;
        }
    }
    year = yearTmp;

    //这个时间戳值的月
    int monthTmp = 0;
    for (monthTmp = 1; monthTmp < MONTH_PER_YEAR; monthTmp++) {
       dayTmp = applib_dt_last_day_of_mon(monthTmp, year);
       if (days >= dayTmp) {
           days -= dayTmp;
       }
       else
       {
           break;
       }
    }
    month = monthTmp;

    day = days + 1;

    //转化成秒。
    int secs = unix_ts % SEC_PER_DAY;
    //这个时间戳值的小时数。
    hour = secs / SEC_PER_HOUR + UTC_TIMEZONE;
    //这个时间戳值的分钟数。
    secs %= SEC_PER_HOUR;
    minute = secs / SEC_PER_MIN;
    //这个时间戳的秒钟数。
    second = secs % SEC_PER_MIN;
    
    time->year = year;
    time->month = month;
    time->day = day;
    time->hour = hour;
    time->minute = minute;
    time->second = second;
    
}

uint32_t YMD_To_Unix_Time(const system_time_t *time)
{
	/*----------------------------------------------------------------*/
	/* Local Variables      */
	/*----------------------------------------------------------------*/
	unsigned short i;
	unsigned int no_of_days = 0;
	int utc_time;
	unsigned char dst;

	/*----------------------------------------------------------------*/
	/* Code Body       */
	/*----------------------------------------------------------------*/
	if (time->year < UTC_BASE_YEAR)
		return 0;

	/* year */
	for (i = UTC_BASE_YEAR; i < time->year; i++)
		no_of_days += (DAY_PER_YEAR + applib_dt_is_leap_year(i));

	/* month */
	for (i = 1; i < time->month; i++)
		no_of_days += applib_dt_last_day_of_mon((unsigned char) i, time->year);

	/* day */
	no_of_days += (time->day - 1);

	/* sec */
	utc_time = (unsigned int) no_of_days * SEC_PER_DAY + (unsigned int) ((time->hour - 8)* SEC_PER_HOUR +
	time->minute * SEC_PER_MIN + time->second);

	return utc_time;
}
