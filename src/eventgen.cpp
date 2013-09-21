/*------------------------------------------------------------------------/
 * File:          eventgen.cpp
 * Created:       2013-09-21
 * Last modified: 2013-09-21 11:08:49 PM CEST
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

#include "eventgen.h"

const string Eventgen::PROPERTY_GROUP_SINGLE_IDENTIFIER = "single";

///////////////////////////////////////////////////////////////////////////

Eventgen::Eventgen(const vector<ObjectGroup> grouped_event_boxes) :
    m_grouped_event_boxes(grouped_event_boxes)
{
    LogDebug2("Constructed eventgen");
}

///////////////////////////////////////////////////////////////////////////

Eventgen::~Eventgen()
{
    LogDebug2("Destoryed eventgen");
}

///////////////////////////////////////////////////////////////////////////

vector<Event> Eventgen::checkForEvents(uint x, uint y)
{
    //TODO: probably better to return pointers to events?
    vector<Event> to_trigger;
    for(vector<ObjectGroup>::const_iterator git = m_grouped_event_boxes.cbegin();
        git != m_grouped_event_boxes.cend(); git++)
    {
        /////////////////////////////////
        //check if the group has the "single" property
        bool single = false;
        map<string, string>::const_iterator pip = 
            git->properties.find("PROPERTY_GROUP_SINGLE_IDENTIFIER");

        if(pip != git->properties.end() && pip->second == "true")
        {
            single = true;
        }
        /////////////////////////////////

        for(vector<Object>::const_iterator oit = git->objects.cbegin();
            oit != git->objects.cend(); oit++)
        {
            //did not reach point yet
            if(oit->x < x || oit->y < y)
            {
                continue;
            }

            //check if within checkbox (assuming we crossed x/y border)
            if(oit->x + oit->width < x && oit->y + oit->height < y)
            {
                Event event;
                event.group = git->name;
                event.name = oit->name;

                /////////////////////////////////
                if(single)
                {
                    bool found = false;
                    for(vector<Event>::const_iterator sit =
                        m_triggered_single_events.cbegin();
                        sit != m_triggered_single_events.cend();
                        sit++)
                    {
                        if(sit->group == event.group &&
                           sit->name == event.name)
                        {
                            found = true;
                            break;
                        }
                    }

                    if(found)
                    {
                        continue;
                    }
                    else
                    {
                        m_triggered_single_events.push_back(event);
                    }
                }
                /////////////////////////////////
                
                to_trigger.push_back(event);
            }
        }
    }

    LogDebug2("Triggering " << to_trigger.size() << " events");
    return to_trigger;
}

///////////////////////////////////////////////////////////////////////////


