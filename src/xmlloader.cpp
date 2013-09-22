/*------------------------------------------------------------------------/
 * File:          xmlloader.cpp
 * Created:       2013-09-21
 * Last modified: 2013-09-22 07:36:30 AM CEST
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

//monkey monkey
const string LoadedMap::XML_MAP             = "map";
const string LoadedMap::XML_MAP_WIDTH       = "width";
const string LoadedMap::XML_MAP_HEIGHT      = "height";
const string LoadedMap::XML_MAP_TILEWIDTH   = "tilewidth";
const string LoadedMap::XML_MAP_TILEHEIGHT  = "tileheight";

const string LoadedMap::XML_TILESET         = "tileset";
const string LoadedMap::XML_TILESET_NAME    = "name";
const string LoadedMap::XML_TILESET_WIDTH   = "tilewidth";
const string LoadedMap::XML_TILESET_HEIGHT  = "tileheight";
const string LoadedMap::XML_TILESET_SPACING = "spacing";
const string LoadedMap::XML_TILESET_MARGIN  = "margin";

const string LoadedMap::XML_IMAGE           = "image";
const string LoadedMap::XML_IMAGE_SOURCE    = "source";
const string LoadedMap::XML_IMAGE_WIDTH     = "width";
const string LoadedMap::XML_IMAGE_HEIGHT    = "height";

const string LoadedMap::XML_TERRAINTYPE     = "terraintypes";
const string LoadedMap::XML_TERRAIN         = "terrain";
const string LoadedMap::XML_TERRAIN_NAME    = "name";
const string LoadedMap::XML_TERRAIN_TILE    = "tile";
const string LoadedMap::XML_TERRAIN_PROPS   = "properties";
const string LoadedMap::XML_TERRAIN_PROP    = "property";
const string LoadedMap::XML_TERRAIN_PROP_NAME   = "name";
const string LoadedMap::XML_TERRAIN_PROP_VALUE  = "value";

const string LoadedMap::XML_TILE            = "tile";
const string LoadedMap::XML_TILE_ID         = "id";
const string LoadedMap::XML_TILE_TERRAIN    = "terrain";

const string LoadedMap::XML_LAYER           = "layer";
const string LoadedMap::XML_LAYER_NAME      = "name";
const string LoadedMap::XML_LAYER_WIDTH     = "width";
const string LoadedMap::XML_LAYER_HEIGHT    = "height";
const string LoadedMap::XML_LAYER_DATA      = "data";
const string LoadedMap::XML_LAYER_DATA_ENCODING     = "encoding";
const string LoadedMap::XML_LAYER_DATA_COMPRESSION  = "compression";

const string LoadedMap::XML_OBJECTGROUP             = "objectgroup";
const string LoadedMap::XML_OBJECTGROUP_DRAWORDER   = "draworder";
const string LoadedMap::XML_OBJECTGROUP_NAME        = "name";
const string LoadedMap::XML_OBJECTGROUP_WIDTH       = "width";
const string LoadedMap::XML_OBJECTGROUP_HEIGHT      = "height";

const string LoadedMap::XML_OBJECTGROUP_PROPS       = "properties";
const string LoadedMap::XML_OBJECTGROUP_PROP        = "property";
const string LoadedMap::XML_OBJECTGROUP_PROP_NAME   = "name";
const string LoadedMap::XML_OBJECTGROUP_PROP_VALUE  = "value";

const string LoadedMap::XML_OBJECT                  = "object";
const string LoadedMap::XML_OBJECT_NAME             = "name";
const string LoadedMap::XML_OBJECT_X                = "x";
const string LoadedMap::XML_OBJECT_Y                = "y";
const string LoadedMap::XML_OBJECT_WIDTH            = "width";
const string LoadedMap::XML_OBJECT_HEIGHT           = "height";

///////////////////////////////////////////////////////////////////////////

LoadedMap::LoadedMap(const string &filename) :
    m_filename(filename)
{
}

///////////////////////////////////////////////////////////////////////////

LoadedMap::~LoadedMap()
{
}

///////////////////////////////////////////////////////////////////////////

ErrorCode LoadedMap::loadFile()
{
    LogDebug("LoadedMap::loadFile start");
    int ret = m_doc.LoadFile(m_filename.c_str());

    if(ret != 0)
    {
        LogError("Unable to open file for parsing");
        return ERROR_OPENING_FILE;
    }

    XMLElement *root_map = m_doc.FirstChildElement(XML_MAP.c_str());
    ASSERT(root_map);
    loadMap(root_map);

    XMLElement *child = root_map->FirstChildElement();
    ASSERT(child);
    while(child != NULL)
    {
        if(child->Name() == XML_TILESET)
        {
            loadTileset(child);
        }
        else if(child->Name() == XML_LAYER)
        {
            loadLayer(child);
        }
        else if(child->Name() == XML_OBJECTGROUP)
        {
            loadObjectGroup(child);
        }
        child = child->NextSiblingElement();
    }
    LogDebug("LoadedMap::loadFile end");

    return OK;
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadMap(XMLElement *element)
{
    LogDebug("LoadedMap::loadMap");
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
    LogDebug2("LoadedMap::loadTileset");

    ASSERT(element);

    TileSet tileset;
    tileset.name = element->Attribute(XML_TILESET_NAME.c_str());

    stringstream tilewidth (element->Attribute(XML_TILESET_WIDTH.c_str()));
    stringstream tileheight (element->Attribute(XML_TILESET_HEIGHT.c_str()));
    stringstream spacing (element->Attribute(XML_TILESET_SPACING.c_str()));
    stringstream margin (element->Attribute(XML_TILESET_MARGIN.c_str()));

    LogDebug2("LoadedMap::loadTileset: Finished parsing attributes");

    tilewidth >> tileset.tilewidth;
    tileheight >> tileset.tileheight;
    spacing >> tileset.spacing;
    margin >> tileset.margin;

    XMLElement *image = element->FirstChildElement(XML_IMAGE.c_str());
    ASSERT(image);
    loadImageSource(image, &tileset);

    XMLElement *terrains = element->FirstChildElement(XML_TERRAINTYPE.c_str());
    ASSERT(terrains);
    loadTerrains(terrains, &tileset);

    XMLElement *first_tile = element->FirstChildElement(XML_TILE.c_str());
    ASSERT(first_tile);
    loadTiles(first_tile, &tileset);

    m_tilesets.push_back(tileset);
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadImageSource(XMLElement *element, TileSet *target)
{
    LogDebug2("LoadedMap::loadImageSource");

    ASSERT(element);
    ASSERT(target);

    target->image.source_image = element->Attribute(XML_IMAGE_SOURCE.c_str());

    stringstream width (element->Attribute(XML_IMAGE_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_IMAGE_HEIGHT.c_str()));

    width >> target->image.width;
    height >> target->image.height;
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTerrains(XMLElement *element, TileSet *target)
{
    LogDebug2("LoadedMap::loadTerrains");

    ASSERT(element);
    ASSERT(target);

    XMLElement *terrain = element->FirstChildElement(XML_TERRAIN.c_str());

    LogDebug2("LoadedMap::loadTerrains: Found first terrain");
    while(terrain != NULL)
    {
        TerrainType parsed_terrain;
        parsed_terrain.name = terrain->Attribute(XML_TERRAIN_NAME.c_str());

        stringstream tile (terrain->Attribute(XML_TERRAIN_TILE.c_str()));
        tile >> parsed_terrain.tile;

        XMLElement *properties = terrain->FirstChildElement(XML_TERRAIN_PROPS.c_str());
        if(properties != NULL)
        {
            loadTerrainProperties(properties, &parsed_terrain);
        }

        target->terraintypes.push_back(parsed_terrain);

        terrain = terrain->NextSiblingElement();
    }
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTerrainProperties(XMLElement *element, TerrainType *target)
{
    LogDebug2("LoadedMap::loadTerrainProperties");

    ASSERT(element);
    ASSERT(target);

    XMLElement *property = element->FirstChildElement(XML_TERRAIN_PROP.c_str());
    while(property != NULL)
    {
        std::pair<string, string> parsed_property;
        parsed_property.first = property->Attribute(XML_TERRAIN_PROP_NAME.c_str());
        parsed_property.second= property->Attribute(XML_TERRAIN_PROP_VALUE.c_str());

        target->properties.insert(parsed_property);
        property = property->NextSiblingElement();
    }
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTiles(XMLElement *element, TileSet *target)
{
    LogDebug2("LoadedMap::loadTiles");

    ASSERT(target);

    while(element != NULL)
    {
        Tile parsed_tile;

        stringstream tile_id (element->Attribute(XML_TILE_ID.c_str()));
        tile_id >> parsed_tile.id; 

        string tile_terrains = element->Attribute(XML_TILE_TERRAIN.c_str());
        mapTilesToTerrainPointers(tile_terrains, target, &parsed_tile);

        element = element->NextSiblingElement();
    }
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::mapTilesToTerrainPointers(string parsed, TileSet *tset, Tile *target)
{
    LogDebug2("LoadedMap::mapTilesToTerrainPointers");

    ASSERT(tset);
    ASSERT(target);

    std::stringstream ss(parsed);
    
    int parsed_val;
    int cnt = 1;
    while (ss >> parsed_val && cnt <= 4)
    {
        switch(cnt)
        {
            case 1:
                target->terrain_1 = &tset->terraintypes[parsed_val];
                break;
            case 2:
                target->terrain_2 = &tset->terraintypes[parsed_val];
                break;
            case 3:
                target->terrain_3 = &tset->terraintypes[parsed_val];
                break;
            case 4:
                target->terrain_4 = &tset->terraintypes[parsed_val];
                break;
        }
            
        if (ss.peek() == ',')
        {
            ss.ignore();
        }

        cnt = cnt + 1;
    }
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadLayer(XMLElement *element)
{
    LogDebug2("LoadedMap::loadLayer");

    ASSERT(element);

    Layer parsed_layer;

    parsed_layer.name = element->Attribute(XML_LAYER_NAME.c_str());

    stringstream width (element->Attribute(XML_LAYER_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_LAYER_HEIGHT.c_str()));

    width >> parsed_layer.width;
    height >> parsed_layer.height;

    XMLElement *data = element->FirstChildElement(XML_LAYER_DATA.c_str());
    if(data != NULL)
    {
        parsed_layer.encoding = data->Attribute(XML_LAYER_DATA_ENCODING.c_str());
        if(parsed_layer.encoding != "csv")
        {
            parsed_layer.compression = data->Attribute(XML_LAYER_DATA_COMPRESSION.c_str());
        }
        parsed_layer.data = data->GetText();
    }

    LogDebug2("LoadedMap::loadLayer: Loaded layer: " << parsed_layer.name);
    m_layers.push_back(parsed_layer);
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadObjectGroup(XMLElement *element)
{
    LogDebug2("LoadedMap::loadObjectGroup");

    ASSERT(element);

    ObjectGroup parsed_group;

    parsed_group.draworder = element->Attribute(XML_OBJECTGROUP_DRAWORDER.c_str());
    parsed_group.name = element->Attribute(XML_OBJECTGROUP_NAME.c_str());

    stringstream width (element->Attribute(XML_OBJECTGROUP_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_OBJECTGROUP_HEIGHT.c_str()));

    width >> parsed_group.width;
    height >> parsed_group.height;

    XMLElement *first_object = element->FirstChildElement(XML_OBJECT.c_str());
    loadObjects(first_object, &parsed_group);

    XMLElement *properties = element->FirstChildElement(XML_OBJECTGROUP_PROPS.c_str());
    if(properties != NULL)
    {
        loadObjectGroupProperties(properties, &parsed_group);
    }
    m_objectgroups.push_back(parsed_group);
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadObjects(XMLElement *element, ObjectGroup *target)
{
    LogDebug2("LoadedMap::loadObjects");

    ASSERT(target);

    while(element != NULL)
    {
        Object parsed_object;

        parsed_object.name = element->Attribute(XML_OBJECT_NAME.c_str());

        stringstream x (element->Attribute(XML_OBJECT_X.c_str()));
        stringstream y (element->Attribute(XML_OBJECT_Y.c_str()));

        stringstream width (element->Attribute(XML_OBJECT_WIDTH.c_str()));
        stringstream height (element->Attribute(XML_OBJECT_HEIGHT.c_str()));

        x >> parsed_object.x;
        y >> parsed_object.y;

        width >> parsed_object.width;
        width >> parsed_object.height;

        target->objects.push_back(parsed_object);
        element = element ->NextSiblingElement();
    }
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadObjectGroupProperties(XMLElement *element, ObjectGroup *target)
{
    LogDebug2("LoadedMap::loadObjectGroupProperties");

    ASSERT(element);
    ASSERT(target);

    XMLElement *property = element->FirstChildElement(XML_OBJECTGROUP_PROP.c_str());
    while(property != NULL)
    {
        std::pair<string, string> parsed_property;
        parsed_property.first = property->Attribute(XML_OBJECTGROUP_PROP_NAME.c_str());
        parsed_property.second= property->Attribute(XML_OBJECTGROUP_PROP_VALUE.c_str());

        target->properties.insert(parsed_property);
        property = property->NextSiblingElement();
    }
}

///////////////////////////////////////////////////////////////////////////
