/*------------------------------------------------------------------------/
 * File:          eventgen.h
 * Created:       2013-09-21
 * Last modified: 2013-09-23 11:27:38 AM CEST
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
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common.h"
#include "errorcodes.h"

#include "xmlloader.h"

using std::vector;

typedef struct Event Event;
typedef struct EventName EventName;
typedef struct EventAssociation EventAssociation;

class Eventhandler;

///////////////////////////////////////////////////////////////////////////

struct Event
{
    string groupname;
    string objectname;
    bool operator== (const Event& other) {
        if (groupname != other.groupname || objectname != other.objectname) {
            return false;
        }
        return true;
    }
};

struct EventName
{
    string groupname;
    string objectname;
};

struct EventAssociation
{
    EventName event;
    Eventhandler *eventhandler;
};

///////////////////////////////////////////////////////////////////////////

class Eventgen
{
    public:
        explicit Eventgen(const vector<ObjectGroup> grouped_event_boxes, const SDL_Rect& viewport, const SDL_Rect& player, std::shared_ptr<LoadedMap>& map);
        ~Eventgen();

        vector<Event> checkForEvents();

    protected:
        vector<ObjectGroup> m_grouped_event_boxes;
        const SDL_Rect& mViewport;
        const SDL_Rect& mPlayerBoundingBox;
        std::shared_ptr<LoadedMap> mMap;

        //! Keep a list of already triggered events if the single property
        //  is true to avoid multiple triggering of certain events
        vector<Event>       m_triggered_single_events;

        static const string PROPERTY_GROUP_SINGLE_IDENTIFIER;
};

///////////////////////////////////////////////////////////////////////////

class Eventhandler
{
    public:
        explicit Eventhandler(const string &eventhandlername = "", SDL_Surface *surface = NULL);
        virtual ~Eventhandler();

        //! execute the given event
        //  \param event info (x, y drawable etc)
        virtual SDL_Surface* executeEvent (const Event &event);

        inline const string& getName() const
        {
            return m_eventhandlername;
        }

        inline bool operator< (const Eventhandler &rhs) const
        {
            return m_eventhandlername < rhs.getName();
        }

    protected:
        string m_eventhandlername;
        SDL_Surface *m_surface;
};

///////////////////////////////////////////////////////////////////////////

class HandlerText : public Eventhandler
{
    public:
        explicit HandlerText(const string &eventhandlername, SDL_Surface *surface, 
                             const string &text_to_draw);
        ~HandlerText();

        SDL_Surface* executeEvent(const Event &event);

    private:
        string m_text_to_draw;
};

///////////////////////////////////////////////////////////////////////////

class EventhandlerMaster
{
    public:
        explicit EventhandlerMaster();
        ~EventhandlerMaster();

        //todo: take ownership, delete in destructor
        inline void registerEventhandler(const string &groupname, const string &objectname,
                                         Eventhandler *eventhandler)
        {
            LogDebug("Registering event handler: " << eventhandler->getName());
            EventName eventname;
            eventname.groupname = groupname;
            eventname.objectname = objectname;

            EventAssociation eventassociation;
            eventassociation.event = eventname;
            eventassociation.eventhandler = eventhandler;

            m_eventhandlers.push_back(eventassociation);
        };

        inline void triggerHandlers(const vector<Event> events)
        {
            m_to_draw.clear();
            for(vector<Event>::const_iterator passed_it = events.cbegin();
                passed_it != events.cend(); passed_it++)
            {
                for(vector<EventAssociation>::iterator it = m_eventhandlers.begin();
                    it != m_eventhandlers.end(); it++)
                {
                    if(it->event.groupname == passed_it->groupname &&
                       it->event.objectname == passed_it->objectname)
                    {
                        LogDebug("Triggering eventhandler: " << it->eventhandler->getName());
                        SDL_Surface* to_draw = it->eventhandler->executeEvent(*passed_it);
                        if(to_draw != NULL)
                        {
                            m_to_draw.push_back(to_draw);
                        }
                    }
                }
            };
        };

        inline vector<SDL_Surface*> getSurfaces()
        {
            return m_to_draw;
        };

    private:
        //TODO: make use of wildcards in the future (eventname vector)
        vector<EventAssociation> m_eventhandlers;
        vector<SDL_Surface*> m_to_draw;
};

///////////////////////////////////////////////////////////////////////////

#endif
