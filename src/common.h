/*------------------------------------------------------------------------/
 * File:	      common.h
 * Created:       13-04-13
 * Last modified: 2013-09-20 06:37:23 PM CEST
 * Author:        David Robin 'starbuck' Cvetko
 *-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------/
 *
 * Copyright (c) 2013 David Robin Cvetko
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the 
 * "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to 
 * permit persons to whom the Software is furnished to do so, subject to 
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *-----------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

#include "errorcodes.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::endl;
using std::string;

typedef unsigned int uint;
typedef unsigned long ulong;

#define __STDC_LIMIT_MACROS

#define DATETIME

string getCurrentTime()
{
    time_t rawtime;
    struct tm *timeinfo;

    char buffer[10];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 10, "%T", timeinfo);
    return string(buffer);
};

#define LogHere             "(" << __FILE__ << "@" << __LINE__ << ") "

#if defined(DATETIME)
    #define TIMESTAMP       getCurrentTime() + " "
#else
    #define TIMESTAMP       ""
#endif

#if defined(DEBUG)
    #define LogDebug(msg)   cout << TIMESTAMP << "DEBUG: " << msg << endl
#else
    #define LogDebug(msg)
#endif

#if defined(DEBUG2)
    #define LogMem(msg)     cout << TIMESTAMP << "MEMORY: " << LogHere << msg << endl
    #define LogDebug2(msg)  cout << TIMESTAMP << "DEBUG2: " << LogHere << msg << endl
#else
    #define LogMem(msg)
    #define LogDebug2(msg)
#endif

#define LogInfo(msg)        cout << TIMESTAMP << "INFO: " << msg << endl
#define LogWarning(msg)     cout << TIMESTAMP << "WARNING: " << msg << endl
#define LogError(msg)       cout << TIMESTAMP << "ERROR: " << msg << endl
#define LogFatal(msg)       cout << TIMESTAMP << "FATAL: " << LogHere << msg << endl; \
                                    exit(EXIT_FAILURE)

#define ASSERT(object)      if(object == NULL){LogFatal("Assertion failed");}

#define DISABLECOPY(classname)  private: \
                                 classname(const classname &rhs); \
                                 classname operator=(const classname &rhs)

#endif
