/*------------------------------------------------------------------------/
 * File:          xmlloader.cpp
 * Created:       2013-09-21
 * Last modified: 2013-09-21 08:09:17 PM CEST
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
        child = child->NextSiblingElement();
    }
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
    XMLElement *terrain = element->FirstChildElement(XML_TERRAIN.c_str());

    LogDebug2("LoadedMap::loadTerrains: Found first terrain");
    while(terrain != NULL)
    {
        TerrainType parsed_terrain;
        parsed_terrain.name = terrain->Attribute(XML_TERRAIN_NAME.c_str());

        stringstream tile (terrain->Attribute(XML_TERRAIN_TILE.c_str()));
        tile >> parsed_terrain.tile;

        XMLElement *properties = element->FirstChildElement(XML_TERRAIN_PROPS.c_str());
        if(properties != NULL)
        {
            loadProperties(properties, &parsed_terrain);
        }

        target->terraintypes.push_back(parsed_terrain);

        terrain = terrain->NextSiblingElement();
    }
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadProperties(XMLElement *element, TerrainType *target)
{
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

void  LoadedMap::loadLayer(XMLElement *element)
{
    LogDebug2("LoadedMap::loadLayer");
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
        parsed_layer.compression = data->Attribute(XML_LAYER_DATA_COMPRESSION.c_str());
        parsed_layer.data = data->GetText();
    }

    LogDebug2("LoadedMap::loadLayer: Loaded layer: " << parsed_layer.name);
    m_layers.push_back(parsed_layer);
}

