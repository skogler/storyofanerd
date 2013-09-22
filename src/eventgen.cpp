/*------------------------------------------------------------------------/
 * File:          eventgen.cpp
 * Created:       2013-09-21
 * Last modified: 2013-09-22 04:00:39 PM CEST
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
    //TODO: remove
    y = 10;

    LogDebug("Checking events for x/y: " << x << "/" << y);
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
            if(x < oit->x || y < oit->y)
            {
                LogDebug("Did not reach x/y points for " << oit->name << "(" << oit->x << "/" << oit->y << ")");
                continue;
            }

            //check if within checkbox (assuming we crossed x/y border)
            if(x < oit->x + oit->width && y < oit->y + oit->height)
            {
                LogDebug("Within checkbox of event " << oit->name);
                Event event;
                event.group = &(*git);
                event.object = &(*oit);

                /////////////////////////////////
                if(single)
                {
                    bool found = false;
                    for(vector<Event>::const_iterator sit =
                        m_triggered_single_events.cbegin();
                        sit != m_triggered_single_events.cend();
                        sit++)
                    {
                        if(sit->group->name == event.group->name &&
                           sit->object->name == event.object->name &&
                           sit->object->x == event.object->x &&
                           sit->object->y == event.object->y)
                        {
                            found = true;
                            break;
                        }
                    }

                    if(found)
                    {
                        LogDebug("Not triggering single event again");
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

    LogDebug("Triggering " << to_trigger.size() << " events");
    return to_trigger;
}

///////////////////////////////////////////////////////////////////////////

Eventhandler::Eventhandler(const string &eventhandlername, SDL_Surface *surface) :
    m_eventhandlername(eventhandlername), m_surface(surface)
{
}

///////////////////////////////////////////////////////////////////////////

Eventhandler::~Eventhandler()
{
}

///////////////////////////////////////////////////////////////////////////

ErrorCode Eventhandler::executeEvent(const Event &event) const
{
    LogDebug("Eventhandler::executeEvent... not doing anything here");
    return OK;
}

///////////////////////////////////////////////////////////////////////////

HandlerText::HandlerText(const string &eventhandlername, SDL_Surface *surface,
                         const string &text_to_draw) :
                        Eventhandler(eventhandlername, surface),
                        m_text_to_draw(text_to_draw)
{
}

///////////////////////////////////////////////////////////////////////////

HandlerText::~HandlerText()
{
}

///////////////////////////////////////////////////////////////////////////

ErrorCode HandlerText::executeEvent(const Event &event) const
{
    LogInfo("Executing Event! Success!");
    return OK;
};

///////////////////////////////////////////////////////////////////////////

EventhandlerMaster::EventhandlerMaster()
{
}

///////////////////////////////////////////////////////////////////////////

EventhandlerMaster::~EventhandlerMaster()
{
}

///////////////////////////////////////////////////////////////////////////

