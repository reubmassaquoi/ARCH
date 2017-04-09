#include "GameMap.h"

GameMap::GameMap()
{
    v2f_mapDimensions = sf::Vector2f(0,0);
    v2f_textureSheetDimensions = sf::Vector2f(0,0);
    v2f_tileDimensions = sf::Vector2f(0,0);
    levelData= {};
    layerData = {};
    textureSheetCoords = {};
}

void GameMap::loadFile(std::string mapFile)
{


///THE MAP FILE MUST FIRST BE OPENED AND PARSED FROM JSON TO VARIABLES*******/
/**********************************************************************/
/**********************************************************************/
    currentMapFile.open(mapFile);
    bool parsedSuccess = mapReader.parse(currentMapFile,baseMapRoot,false);
    if(!parsedSuccess)
    {
        std::cout<<"failed to parse JSON"<< std::endl
        <<mapReader.getFormattedErrorMessages()
        <<std::endl;
    }
    else
    {
        std::cout<<"Map file has been parsed"<<std::endl;

     ///store the tileCount Dimensions
    v2f_mapDimensions = sf::Vector2f(baseMapRoot["width"].asFloat(),baseMapRoot["height"].asFloat());//*v2f_tileDimension.x,baseMapRoot["height"].asFloat()*v2f_tileDimension.y);
    ///store the tileWidth Dimensions
    v2f_tileDimensions = sf::Vector2f(baseMapRoot["tilewidth"].asFloat(),baseMapRoot["tileheight"].asFloat());
    ///store texture sheet Dimensions
    v2f_textureSheetDimensions = sf::Vector2f(baseMapRoot["tilesets"][0]["imagewidth"].asFloat(),baseMapRoot["tilesets"][0]["imageheight"].asFloat());
    ///store map Image
    mapImage.loadFromFile("assets/"+baseMapRoot["tilesets"][0]["image"].asString());
    ///store map texture
    mapTexture.loadFromImage(mapImage);
    layer1.t = mapTexture;
    layer2.t = mapTexture;

         /**This segement involved going through the various layers,
        checking whether their type is tilelayer, and storing the layers data in a level data list
        and storing that list in a level layers list**/
        for(int i = 0; i<baseMapRoot["layers"][i].size();++i)
        {

            if(baseMapRoot["layers"][i]["type"].asString().compare("tilelayer")==0)
            {

                for(int ii = 0; ii<baseMapRoot["layers"][i]["data"].size();++ii)
                {

                    levelData.push_back(baseMapRoot["layers"][i]["data"][ii].asInt());

                }
                std::vector<int> l;
                l = levelData;
                layerData.push_back(l);
                levelData.clear();
            }
        }

        /** store tile coordinates from the map sheet texture**/

        int x,y;
        x = 0;
        y = 0;
        //std::cout<<baseMapRoot["tilsets"]<<std::endl;
        for (int i = 0; i<baseMapRoot["tilesets"][0]["tilecount"].asInt(); ++i)
        {
            textureSheetCoords.push_back(sf::Vector2f(x,y));
            x+=v2f_tileDimensions.x;
            if(x == v2f_textureSheetDimensions.x)
            {
                x = 0;
                y+=v2f_tileDimensions.y;
            }

        }

        std::cout<<"Map Sheet coordinates stored"<<std::endl;
        }
}

void GameMap::create()
{
    /** to create and store the map,
    we will have to set numerous quads
    in the vertex array to represent each tile**/
    sf::VertexArray tileMap;
    tileMap.setPrimitiveType(sf::Quads);

    /**
    First, Loop through the level data to determine which tile is being placed.
    Second, check to see if the tile is not an empty tile.
    third, create a pointer to a quad in the vertex array and set the position of the tile based
    on an incremental pattern and it's position in the map.
    repeat for each column by row until map is stored with tiles.
    **/
    for(int ii = 0; ii<layerData.size();++ii)
    {

        if(ii==0)
        {
            layer1.v.resize(layerData[ii].size()*4);
        }
        if(ii==1)
        {
            layer2.v.resize(layerData[ii].size()*4);
        }

        int xx,yy;
        xx = 0;
        yy = 0;
        int rowCount = 0;
        int colCount = 0;
        int j = 0;


        for(int i = 0; i<layerData[ii].size(); ++i)
        {

                if(layerData[ii][i]>0)
                {
                    if(ii==0)
                    {
                        sf::Vertex* quad = &layer1.v[j];
                        quad[j].position=sf::Vector2f(xx,yy);
                        quad[j+1].position=sf::Vector2f(xx+v2f_tileDimensions.x,yy);
                        quad[j+2].position=sf::Vector2f(xx+v2f_tileDimensions.x,yy+v2f_tileDimensions.y);
                        quad[j+3].position=sf::Vector2f(xx,yy+v2f_tileDimensions.y);

                        quad[j].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x,textureSheetCoords[layerData[ii][i]-1].y);
                        quad[j+1].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x+v2f_tileDimensions.x,textureSheetCoords[layerData[ii][i]-1].y);
                        quad[j+2].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x+v2f_tileDimensions.x,textureSheetCoords[layerData[ii][i]-1].y+v2f_tileDimensions.y);
                        quad[j+3].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x,textureSheetCoords[layerData[ii][i]-1].y+v2f_tileDimensions.y);

                    }
                    if(ii==1)
                    {
                        sf::Vertex* quad = &layer2.v[j];
                        quad[j].position=sf::Vector2f(xx,yy);
                        quad[j+1].position=sf::Vector2f(xx+v2f_tileDimensions.x,yy);
                        quad[j+2].position=sf::Vector2f(xx+v2f_tileDimensions.x,yy+v2f_tileDimensions.y);
                        quad[j+3].position=sf::Vector2f(xx,yy+v2f_tileDimensions.y);

                        quad[j].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x,textureSheetCoords[layerData[ii][i]-1].y);
                        quad[j+1].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x+v2f_tileDimensions.x,textureSheetCoords[layerData[ii][i]-1].y);
                        quad[j+2].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x+v2f_tileDimensions.x,textureSheetCoords[layerData[ii][i]-1].y+v2f_tileDimensions.y);
                        quad[j+3].texCoords= sf::Vector2f(textureSheetCoords[layerData[ii][i]-1].x,textureSheetCoords[layerData[ii][i]-1].y+v2f_tileDimensions.y);

                    }
                }
                //tileMapeLayers.push_back(tileMap);


                j+=2;
                xx+=v2f_tileDimensions.x;
                ++rowCount;
                if(rowCount==v2f_mapDimensions.x)
                {

                    rowCount = 0;
                    xx = 0;
                    yy+=v2f_tileDimensions.y;
                }

        }


        /*if(ii==0)
        {
            std::cout<<"got here"<<std::endl;
            layer1.v = tileMap;
            //
            //
        }
        if(ii==1)
        {
            std::cout<<"Got Here"<<std::endl;
            layer2.v = tileMap;
            tileMap.clear();

        }*/
    }

}

batch GameMap::getFirstLayer()
{
    return layer1;
}
batch GameMap::getSecondLayer()
{
    return layer2;
}

GameMap::~GameMap()
{
    //dtor
}