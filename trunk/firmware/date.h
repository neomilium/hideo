#ifndef _DATE_H_
#define _DATE_H_

struct datetime {
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char day;
	unsigned char month;
	unsigned char year;
};

typedef struct datetime datetime_t;

#endif

