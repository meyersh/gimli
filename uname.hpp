#ifndef __UNAME__HPP__
#define __UNAME__HPP__

#include <string>
#include <sys/utsname.h>

std::string uname()
{
   struct utsname name;
   uname(&name);
   return name.sysname;
}

#endif
