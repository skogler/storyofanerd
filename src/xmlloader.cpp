/*------------------------------------------------------------------------/
 * File:          xmlloader.cpp
 * Created:       2013-09-21
 * Last modified: 2013-09-21 03:26:52 PM CEST
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

#include "xmlloader.h"

#include <sstream>

const string LoadedMap::XML_MAP             = "map";
const string LoadedMap::XML_MAP_WIDTH       = "width";
const string LoadedMap::XML_MAP_HEIGHT      = "height";
const string LoadedMap::XML_MAP_TILEWIDTH   = "tilewidth";
const string LoadedMap::XML_MAP_TILEHEIGHT  = "tileheight";

const string LoadedMap::XML_TILESET         = "tileset";
const string LoadedMap::XML_TILESET_NAME    = "name";
const string LoadedMap::XML_TILESET_WIDTH   = "width";
const string LoadedMap::XML_TILESET_HEIGHT  = "height";
const string LoadedMap::XML_TILESET_SPACING = "spacing";
const string LoadedMap::XML_TILESET_MARGIN  = "margin";

///////////////////////////////////////////////////////////////////////////

LoadedMap::LoadedMap(const string &filename) :
    m_filename(filename)
{
    loadFile();
}

///////////////////////////////////////////////////////////////////////////

LoadedMap::~LoadedMap()
{
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadFile()
{
    m_doc.LoadFile(m_filename.c_str());

    XMLElement *root_map = m_doc.FirstChildElement(XML_MAP.c_str());
    ASSERT(root_map);
    loadMap(root_map);

    //TODO loop with sibling || sibling = layer or tileset
    XMLElement *tileset = m_doc.FirstChildElement(XML_TILESET.c_str());
    ASSERT(tileset);
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadMap(XMLElement *element)
{
    stringstream width (element->Attribute(XML_MAP_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_MAP_HEIGHT.c_str()));
    stringstream tilewidth (element->Attribute(XML_MAP_TILEWIDTH.c_str()));
    stringstream tileheight (element->Attribute(XML_MAP_TILEHEIGHT.c_str()));
    width >> m_map.width;
    height >> m_map.height;
    tilewidth >> m_map.tilewidth;
    tileheight >> m_map.tileheight;
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTileset(XMLElement *element)
{
}

///////////////////////////////////////////////////////////////////////////
