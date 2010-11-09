/**************************************************************************
tstamp.cpp

Created By: Randy Campbell

DEPENDENCIES: NONE

DESCRIPTION:

Provides a timestamp class.

The class represents a timestamp as a long long - which is at least a
64 bit integer in C.  Although long long is not, de jure, a type in C++
at present (JAN 2010) it is slated to be added to the next ISO C++ standard
and there are compilers (such as g++) that implement it.  If your compiler
implements a 64-bit int with another name, then you will need to modify the
class.  If your compiler does not implement 64-bit integers then this will
not work for you.

The timestamp type assumes that days begin at midnight and represents 
a time stamp by the number of seconds elapsed since midnight, Jan 1, of 
Julian Day 0 (altering the Julian Day scheme so that the days start at
midnight instead of noon).  Timestamps may be compared using relational 
operators.  They may also be subtracted from each other to yield
the number of seconds between them.  

Timestamps assume that a desired year, month, day, hour, minute, second
values can be represented as ints and that the largest number of desired Julian
Days can be represented as a long.

The routines used to convert from Julian Days to Gregorian Dates and
vice versa are based on the sample routines from the national naval 
observatory website which, in turn, created them from algorithms that
came from:

Fliegel, H.F. and van Flandern, T.C., Communications of the ACM,
Vol. 11, No. 10 (October 1968).

ASSUMPTIONS:
* The Julian Day scheme used here assumes that Days begin at midnight
  (rather than noon)
* Assumes that the largest desired year can fit in an int
* Assumes that the largest desired number of Julian Days can fit into
  a long.

**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tstamp.hpp"

/*************************************************************************
**************************************************************************
Julian Day functions (static, so local to module)
**************************************************************************
**************************************************************************/

static void GetGregorianDate(long jdays, int *year, int *month, int *day);
static long GetJulianDays (int year, int month, int day);

/*************************************************************************
**************************************************************************/

static void GetGregorianDate(long jdays, int &year, int &month, int &day)  
/*----------------------------------------------------------
Parameters:

jdays:  Number of Julian Days
*year:  Integer giving the year
*month: Integer giving the month (starts at 1)
*day:   Integer giving the day (starts at 1)

~~~~~~~~~~~
Description:

Converts a number of Julian Days to a Gregorian Date

This routine based on the sample routine from the national naval 
observatory website which, in turn, created them from algorithms that
came from:

Fliegel, H.F. and van Flandern, T.C., Communications of the ACM,
Vol. 11, No. 10 (October 1968).
------------------------------------------------------------*/
{ long t1, t2, ty, tm, td;

t1= jdays+68569L;
t2= 4*t1/146097L;
t1= t1-(146097L*t2+3L)/4L;
ty= 4000L*(t1+1L)/1461001L;
t1= t1-1461L*ty/4L+31L;
tm= 80L*t1/2447L;
td= t1-2447L*(tm)/80L;
t1= tm/11L;
tm= tm+2L-12L*t1;
ty= 100L*(t2-49L)+ty+t1;
year = (int) ty;
month = (int) tm;
day = (int) td;
}

/*************************************************************************
**************************************************************************/

static int daylims[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//======================================================================

static long GetJulianDays (int year, int month, int day)
/*----------------------------------------------------------
Parameters:

year:  Integer giving the year
month: Integer giving the month (starts at 1)
day:   Integer giving the day (starts at 1)

~~~~~~~~~~~
Description:

Returns the number of Julian Days matching the given Gregorian
Date  Year must be >= 1.
Returns >= 0 on success and negative number on failure

Failure Values:
-1: Year < 1
-2: Month not in range 1-12
-3: Day out of range for the month
-4: Number of Julian Days is not in range JULIAN_MIN - JULIAN_MAX 


This routine based on the sample routine from the national naval 
observatory website which, in turn, created them from algorithms that
came from:

Fliegel, H.F. and van Flandern, T.C., Communications of the ACM,
Vol. 11, No. 10 (October 1968).
------------------------------------------------------------*/
{ long jdays,         //*** Julian days
       ty,            //*** Temp Years
       tm,            //*** Temp Months
       td,            //*** Temp Days
       extraday = 0L;  //*** Extra day for leap years

if (year < 1)
   return -1;
ty = year;
   
if ((month < 1) || (month > 12))
   return -2;
tm = month;
   
if ((month == 2) && (((year % 100) && !(year % 4)) || !(year % 400)))
   extraday = 1L;

if ((day < 1) || (day > (daylims[month-1] + 
             ((month == 2) ? extraday : 0))))
   return -3;
td = day;

jdays= td-32075L+1461L*(ty+4800L+(tm-14L)/12L)/4L
              +367L*(tm-2L-(tm-14L)/12L*12L)/12L
              -3L*((ty+4900L+(tm-14L)/12L)/100L)/4L;
              
return jdays;
}

/*************************************************************************
**************************************************************************
friend functions
**************************************************************************
**************************************************************************/

tstamp operator+(long seconds, const tstamp &stamp)
// Add seconds to a time stamp (seconds is first operator)
{ tstamp rval; 

rval.stmp = stamp.stmp + seconds; 
return rval;
}


/*************************************************************************
**************************************************************************
member functions
**************************************************************************
**************************************************************************/

/************************************************************
tstamp::getDayNum

Return the number in the week (Sunday: 1, Monday: 2, Tuesday: 3, etc.)
of the day in the date given by the timestamp
------------------------------------------------------------*/
int tstamp::getDayNum()
{ int temp;

temp = (int) ((stmp/DAYSECLL) % 7LL);
if (temp != 6)
   return temp + 2; //*** Any day but Sunday

return 1; //*** Sunday
}

/************************************************************
tstamp::getDate

Parameters:
year:     Year of the timestamp
mon:      Month of the timestamp   (range: 1-12)
day:      Day of the timestamp     (range: 1-31, or less depending on month)

Return all constituent date parts of the time stamp through
pointer params.  
------------------------------------------------------------*/
void tstamp::getDate (int &year, int &month, int &day)
{ long jdays;    //*** Julian days (giving the date)
      
GetGregorianDate((long) (stmp/DAYSECLL), year, month, day); 
}

/************************************************************
tstamp::getStampParts

Parameters:
year:     Year of the timestamp
mon:      Month of the timestamp   (range: 1-12)
day:      Day of the timestamp     (range: 1-31, or less depending on month)
hour:     Hours of the timestamp   (range: 0-23)
min:      Minutes of the timestamp (range: 0-59)
sec:      Seconds of the timestamp (range: 0-59)

Return all constituent time and date parts of the time stamp through
pointer params.  
------------------------------------------------------------*/
void tstamp::getStampParts (int &year, int &month, int &day,
                    int &hour, int &min,   int &sec)
{ long tsec;     //*** Time of day (in seconds)
      
tsec = (long) (stmp % DAYSECLL);
GetGregorianDate((long) (stmp/DAYSECLL), year, month, day); 
hour = (int) (tsec / HOURSECL);
min = (int) ((tsec % HOURSECL) / 60L);
sec = (int) (tsec % 60L);
}


/************************************************************
tstamp::setCurrentDateTimeStamp

Parameters:
uselocal: If NZ, use local time, otherwise Universal Time

Set the timestamp to the current date and time
------------------------------------------------------------*/
void tstamp::setCurrentDateTimeStamp (int uselocal)
{ time_t curdate;     //*** Current date in time.h format
  struct tm *tdptr; //*** Defined by time.h
  
curdate = time(NULL);

if (uselocal)
   tdptr = localtime(&curdate);
else
   tdptr = gmtime(&curdate);
   
stmp =  GetJulianDays(tdptr->tm_year + 1900, tdptr->tm_mon + 1, 
                       tdptr->tm_mday) * DAYSECLL;
                       
stmp +=  tdptr->tm_hour * HOURSECL + tdptr->tm_min * MINUTESECL + 
                tdptr->tm_sec;
}

/************************************************************
tstamp::setTimeStamp

Parameters:
year:     Year of the timestamp
mon:      Month of the timestamp   (range: 1-12)
day:      Day of the timestamp     (range: 1-31, or less depending on month)
hour:     Hours of the timestamp   (range: 0-23)
min:      Minutes of the timestamp (range: 0-59)
sec:      Seconds of the timestamp (range: 0-59)

Set the timestamp to the time and date indicated
Note... Violating range expectations will likely result in incorrect return
value.
------------------------------------------------------------*/
void tstamp::setTimeStamp (int year, int mon, int day,
                     int hour, int min, int sec)
{       
stmp = GetJulianDays (year, mon, day);
stmp = (stmp * DAYSECLL) + (hour * HOURSECL)
                                  + (min * MINUTESECL) + sec;
}


