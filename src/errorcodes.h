/*------------------------------------------------------------------------/
 * File:          errorcodes.h
 * Created:       13-05-11
 * Last modified: 2013-09-19 09:01:31 PM CEST
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

#ifndef ERRORCODES_H
#define ERRORCODES_H

#include "common.h"

enum ErrorCode : unsigned int
{
    OK                      = 0,
    ERROR_UNKNOWN           = 1,
    ERROR_OUT_OF_MEMORY     = 2,
    ERROR_FILE_NOT_FOUND    = 3,
    ERROR_OPENING_FILE      = 4,
    NB_ERROR_COUNTER
};

static const char *error_msgs[NB_ERROR_COUNTER] = 
{
    /* OK */
    "Ok. No error found",

    /* ERROR_UNKOWN */
    "Unknown error occoured",

    /* ERROR_OUT_OF_MEMORY */
    "Out of memory",

    /* ERROR_FILE_NOT_FOUND */
    "File not found",

    /* ERROR_OPENING_FILE */
    "Unable to open file"
};

#define ERRORMSG(type) error_msgs[type]

#endif
