#include "rtc_time.h"

void timer_to_cal (unsigned long timer, rtc_cal * rtc_time)
{
	unsigned long a;
	char b;
	char c;
	char d;
	unsigned long time;

	time = timer%SEC_A_DAY;
	a = ((timer+43200)/(86400>>1)) + (2440587<<1) + 1;
	a>>=1;
	rtc_time->wday = a%7;
	a+=32044;
	b=(4*a+3)/146097;
	a=a-(146097*b)/4;
	c=(4*a+3)/1461;
	a=a-(1461*c)/4;
	d=(5*a+2)/153;
	rtc_time->mday=a-(153*d+2)/5+1;
	rtc_time->mon=d+3-12*(d/10);
	rtc_time->year=100*b+c-4800+(d/10);
	rtc_time->hour=time/3600;
	rtc_time->min=(time%3600)/60;
	rtc_time->sec=(time%3600)%60;
}

unsigned long cal_to_timer (rtc_cal * rtc_time)
{
	char a;
	int y;
	char m;
	unsigned long Uday;
	unsigned long time;

	a=((14-rtc_time->mon)/12);
	y=rtc_time->year+4800-a;
	m=rtc_time->mon+(12*a)-3;
	Uday=(rtc_time->mday+((153*m+2)/5)+365*y+(y/4)-(y/100)+(y/400)-32045)-2440588;
	time=Uday*86400;
	time+=rtc_time->sec+rtc_time->min*60+rtc_time->hour*3600;
	return time;
}

uint8_t convert_time_from_android(rtc_cal *rtc_time, char *date){

	//*xxx*yyyy-MM-dd HH:mm:ss

	uint8_t res_date = DATE_OK;

	int s;
	char buf[64];

	char *p_rtc = date;

	*p_rtc++;
	*p_rtc++;
	*p_rtc++;
	*p_rtc++;
	*p_rtc++;

	for(s = 0; s < 4; s++){
		buf[s] = *p_rtc++;
	}
	rtc_time->year=atoi(buf);
			if(rtc_time->year == 0 || rtc_time->year < 1980)
				return res_date = INCORRECT_YEAR;
	*p_rtc++;
	memset(buf,0,strlen(buf));
	for(s = 0; s < 2; s++){
		buf[s] = *p_rtc++;
	}
	rtc_time->mon=(char)atoi(buf);
			if(rtc_time->mon == 0 || rtc_time->mon > 12)
					return res_date = INCORRECT_MONTH;
	*p_rtc++;
	memset(buf,0,strlen(buf));
	for(s = 0; s < 2; s++){
		buf[s] = *p_rtc++;
	}
	rtc_time->mday=(char)atoi(buf);
			if(rtc_time->mday == 0 || rtc_time->mday > 31)
					return res_date = INCORRECT_DAY;
	*p_rtc++;
	memset(buf,0,strlen(buf));
	for(s = 0; s < 2; s++){
		buf[s] = *p_rtc++;
	}
	rtc_time->hour=(char)atoi(buf);
	*p_rtc++;
	memset(buf,0,strlen(buf));
	for(s = 0; s < 2; s++){
		buf[s] = *p_rtc++;
	}
	rtc_time->min=(char)atoi(buf);
	*p_rtc++;
	memset(buf,0,strlen(buf));
	for(s = 0; s < 2; s++){
		buf[s] = *p_rtc++;
	}
	rtc_time->sec=(char)atoi(buf);

	return res_date;
//	if(date[])

}

