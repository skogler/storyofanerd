/*------------------------------------------------------------------------/
 * File:          eventgen.h
 * Created:       2013-09-21
 * Last modified: 2013-09-21 10:56:16 PM CEST
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

#ifndef EVENTGEN_H
#define EVENTGEN_H

#include <vector>

#include "common.h"
#include "errorcodes.h"

#include "xmlloader.h"

using std::vector;

typedef struct Event Event;

struct Event
{
    string      group;
    string      name;
};

class Eventgen
{
    public:
        explicit Eventgen(const vector<ObjectGroup> grouped_event_boxes);
        ~Eventgen();

        vector<Event> checkForEvents(uint x, uint y);

    private:
        vector<ObjectGroup> m_grouped_event_boxes;

        //! Keep a list of already triggered events if the single property
        //  is true to avoid multiple triggering of certain events
        //  TODO -- add property parsing in xmlloader and checking here
        vector<Event>       m_triggered_single_events;

        static const string PROPERTY_GROUP_SINGLE_IDENTIFIER;
};

#endif