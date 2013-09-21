/*------------------------------------------------------------------------/
 * File:          xmlloader.h
 * Created:       2013-09-21
 * Last modified: 2013-09-21 05:44:04 PM CEST
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

#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <string>
#include <vector>
#include <map>

#include <tinyxml2.h>

#include "common.h"
#include "errorcodes.h"

using namespace std;
using namespace tinyxml2;

typedef struct TileMap TileMap;
typedef struct ImageSource ImageSource;
typedef struct TileSet TileSet;
typedef struct TerrainType TerrainType;
typedef struct Tile Tile;
typedef struct Layer Layer;

struct TileMap
{
    uint        width;
    uint        height;
    uint        tilewidth;
    uint        tileheight;
};

struct ImageSource
{
    string      source_image;
    uint        width;
    uint        height;
};

struct TileSet
{
    string      name;
    uint        tilewidth;
    uint        tileheight;
    uint        spacing;
    uint        margin;

    vector<TerrainType> terraintypes;
    vector<Tile> tiles;

    ImageSource image;
};

struct TerrainType
{
    string      name;
    uint        tile;
    map<string, string>     properties;
};

struct Tile
{
    uint         id;
    TerrainType* terrain_1;
    TerrainType* terrain_2;
    TerrainType* terrain_3;
    TerrainType* terrain_4;
};

struct Layer
{
    string      name;
    uint        width;
    uint        height;
    string      encoding;
    string      compression;
};

class LoadedMap
{
    public:
        explicit LoadedMap(const string &filename);
        ~LoadedMap();

    private:
        //TODO: error handling (maybe...)
        void loadFile();
        void loadMap(XMLElement *element);
        void loadTileset(XMLElement *element);
        void loadImageSource(XMLElement *element, TileSet *target);
        void loadTerrains(XMLElement *element, TileSet *target);
        void loadProperties(XMLElement *element, TerrainType *target);
        void loadTiles(XMLElement *element, TileSet *target);
        void mapTilesToTerrainPointers(string parsed, TileSet *tset, Tile *target);

        string          m_filename;

        TileMap         m_map;
        vector<TileSet> m_tilesets;
        vector<Layer>   m_layers;

        XMLDocument     m_doc;

        static const string XML_MAP;
        static const string XML_MAP_WIDTH;
        static const string XML_MAP_HEIGHT;
        static const string XML_MAP_TILEWIDTH;
        static const string XML_MAP_TILEHEIGHT;

        static const string XML_TILESET;
        static const string XML_TILESET_NAME;
        static const string XML_TILESET_WIDTH;
        static const string XML_TILESET_HEIGHT;
        static const string XML_TILESET_SPACING;
        static const string XML_TILESET_MARGIN;

        static const string XML_IMAGE;
        static const string XML_IMAGE_SOURCE;
        static const string XML_IMAGE_WIDTH;
        static const string XML_IMAGE_HEIGHT;

        static const string XML_TERRAINTYPE;
        static const string XML_TERRAIN;
        static const string XML_TERRAIN_NAME;
        static const string XML_TERRAIN_TILE;
        static const string XML_TERRAIN_PROPS;
        static const string XML_TERRAIN_PROP;
        static const string XML_TERRAIN_PROP_NAME;
        static const string XML_TERRAIN_PROP_VALUE;

        static const string XML_TILE;
        static const string XML_TILE_ID;
        static const string XML_TILE_TERRAIN;
};

#endif
