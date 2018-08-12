
#ifndef _TIME_H_
#define _TIME_H_


typedef struct 
{
  unsigned int secondsCount;
  unsigned int minuteCount;
  unsigned int hourCount;
  unsigned int dayCount;
  unsigned int yearCount;  
}_savedTime;

void initTime(void);

#endif
