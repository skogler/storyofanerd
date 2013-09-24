/*------------------------------------------------------------------------/
 * File:          eventgen.cpp
 * Created:       2013-09-21
 * Last modified: 2013-09-23 11:39:28 AM CEST
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
#include <SDL2/SDL.h>
#include <algorithm>

const string Eventgen::PROPERTY_GROUP_SINGLE_IDENTIFIER = "single";

///////////////////////////////////////////////////////////////////////////

Eventgen::Eventgen(const vector<ObjectGroup> grouped_event_boxes, const SDL_Rect& viewport, const SDL_Rect& player, std::shared_ptr<LoadedMap>& map)
    : m_grouped_event_boxes(grouped_event_boxes)
    , mViewport(viewport)
    , mPlayerBoundingBox(player)
    , mMap(map)
{
    LogDebug2("Constructed eventgen");
}

///////////////////////////////////////////////////////////////////////////

Eventgen::~Eventgen()
{
    LogDebug2("Destroyed eventgen");
}

///////////////////////////////////////////////////////////////////////////

vector<Event> Eventgen::checkForEvents()
{
    vector<Event> to_trigger;

    for(const auto& group : m_grouped_event_boxes)
    {
        /////////////////////////////////
        //check if the group has the "single" property
        bool single = false;
        map<string, string>::const_iterator pip = group.properties.find(PROPERTY_GROUP_SINGLE_IDENTIFIER);

        if(pip != group.properties.end() && pip->second == "true")
        {
            single = true;
        }

        for (const auto& object : group.objects)
        {
            //TODO: optimize map translation
            SDL_Rect objectBoundingBox(object.bbox);
            objectBoundingBox.y += mViewport.h - (mMap->getTileMap().height * mMap->getTileMap().tileheight);
            if (SDL_HasIntersection(&objectBoundingBox, &mPlayerBoundingBox))
            {
                Event event;
                event.groupname = group.name;
                event.objectname = object.name;

                if(single)
                {
                    auto singleIterator = std::find(m_triggered_single_events.begin(), m_triggered_single_events.end(), event);
                    if(singleIterator != m_triggered_single_events.end())
                    {
                        LogDebug("Not triggering single event again: " << object.name);
                        continue;
                    }
                    else
                    {
                        m_triggered_single_events.push_back(event);
                    }
                }
                LogDebug("Triggering event for object: " << object.name);
                to_trigger.push_back(event);
            }
        }
    }

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

SDL_Surface* Eventhandler::executeEvent(const Event &event)
{
    LogDebug("Eventhandler::executeEvent... not doing anything here");
    return NULL;
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

SDL_Surface* HandlerText::executeEvent(const Event &event)
{
    LogInfo("Executing Event! Success!");
    SDL_Surface *bmp = SDL_LoadBMP("../res/images/event.bmp");
    return bmp;
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

