/**************************************************************************
tstamp.hpp

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

#ifndef TSTAMP_HPP
#define TSTAMP_HPP

#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;	

//*** Define values for doing times
#define DAYSEC       86400
#define HALFDAYSEC   43200
#define HOURSEC      3600
#define MINUTESEC    60

#define DAYSECL      86400L
#define HALFDAYSECL  43200L
#define HOURSECL     3600L
#define MINUTESECL   60L

#define DAYSECLL     86400LL
#define HALFDAYSECLL 43200LL
#define HOURSECLL    3600LL
#define MINUTESECLL  60LL

//*******************************************************************
// The Class
class tstamp
{
long long stmp;

public:
tstamp() {stmp = 0LL;}

// ------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------

bool operator<(const tstamp &op2) {return stmp < op2.stmp;}
bool operator>(const tstamp &op2) {return stmp > op2.stmp;}
bool operator<=(const tstamp &op2) {return stmp <= op2.stmp;}
bool operator>=(const tstamp &op2) {return stmp >= op2.stmp;}
bool operator==(const tstamp &op2) {return stmp == op2.stmp;}
bool operator!=(const tstamp &op2) {return stmp != op2.stmp;}

tstamp operator-=(long seconds) {stmp -= seconds; return *this;}
tstamp operator+=(long seconds) {stmp += seconds; return *this;}
tstamp operator-(long seconds)
   {tstamp rval; rval.stmp = stmp - seconds; return rval;}
tstamp operator+(long seconds) 
   {tstamp rval; rval.stmp = stmp + seconds; return rval;}
friend tstamp operator+(long seconds, const tstamp & stamp);
// Returns the number of seconds difference between the two timestamps
long long operator-(const tstamp &op2) 
   {return stmp - op2.stmp;}

// ------------------------------------------------------------------
// Inline non-operator functions
// ------------------------------------------------------------------

// Return the index in the week (Sunday: 0, Monday: 1, Tuesday: 2, etc.) of
//    the day of the date given by this timestamp 
//    Note: Julian Day 0 is a Monday 
int getDayIndex() {return (int) (((stmp/DAYSECLL) + 1LL) % 7LL);}

// ------------------------------------------------------------------

// Return the date (at the same time) of the Sunday of the week
//         in which this appears
//            Note: Julian Day 0 is a Monday
tstamp getWeekSunday ()
{ tstamp thisday;
thisday.stmp = stmp/DAYSECLL;
thisday.stmp = ((thisday.stmp - ((thisday.stmp + 1LL) % 7LL)) * DAYSECLL) + 
                                                           (stmp % DAYSECLL);
return thisday;
}

// ------------------------------------------------------------------

// Return all constituent time parts of the time stamp through
//     pointer params.  

void getTime (int &hour, int &min, int &sec)
{ long tsec;     //*** Time of day (in seconds)
tsec = (long) (stmp % DAYSECLL);
hour = (int) (tsec / HOURSECL);
min = (int) ((tsec % HOURSECL) / 60L);
sec = (int) (tsec % 60L);
}

// ------------------------------------------------------------------

// Convert a numeric timestamp to a string.  The string should have at
// least 13 bytes that can receive data.

void stampToString (string &str)
{ ostringstream oss;

oss << stmp;
str = oss.str();
}

//----------------------------------------------------------

// Convert a timestamp string back to a timestamp
void stringToStamp (const string &str)
{ 
stmp = strtoll(str.c_str(), NULL, 10);
}

// ------------------------------------------------------------------
// Member functions defined in the .cpp file
// ------------------------------------------------------------------

// Return the number in the week (Sunday: 1, Monday: 2, Tuesday: 3, etc.)
//   of the day in the date given by the timestamp
int getDayNum();

// ------------------------------------------------------------------
 
// Return all constituent date parts of the time stamp through
 void getDate (int &year, int &month, int &day);

// ------------------------------------------------------------------

// Return all constituent time and date parts of the time stamp through
void getStampParts (int &year, int &month, int &day,
                    int &hour, int &min,   int &sec);

// ------------------------------------------------------------------

// Set the timestamp to the current date and time
void setCurrentDateTimeStamp (int uselocal);

// ------------------------------------------------------------------

//Set the timestamp to the time and date indicated
//     Note... Violating range expectations will likely 
//     result in incorrect return value.
void setTimeStamp (int year, int mon, int day,
                     int hour, int min, int sec);
};
#endif
