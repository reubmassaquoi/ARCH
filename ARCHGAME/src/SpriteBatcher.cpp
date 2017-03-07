#include "SpriteBatcher.h"
class level;



SpriteBatcher::SpriteBatcher()
{
    //ctor

}
void SpriteBatcher::setBatchTexture(sf::Image i)
{
    if(!batch_texture.loadFromImage(i))
    {
        std::cout<<"Unable to load batch texture"<<std::endl;
    }
    else
    {
        std::cout<<"Entity_texture has successfully been stored"<<std::endl;

    }

}

///Load Entity Spritesheet into game
void SpriteBatcher::loadEntityTextures()
{
    textureFile.open("assets/characterTextures.json");
    bool parsedSuccess = myReader.parse(textureFile,textureRoot,false);
    if(!parsedSuccess)
    {
         std::cout<<"failed to parse JSON"<< std::endl
            <<myReader.getFormattedErrorMessages()<<std::endl;

    }
    else
    {
        std::cout<<"Entity Texture File has successfully been parsed"<<std::endl;
        sf::Image i;

        if(!i.loadFromFile("assets/"+textureRoot["meta"]["image"].asString()))
        {
            std::cout<<"Entity Texture File has failed to load; Confirm the correct Name or Directory"<<std::endl;
        }
        else
        {
            setBatchTexture(i);

        }
    }
}
void SpriteBatcher::setFrameTexture(GraphicsComponent* g,std::string textureName)
{
    for(int i = 0;i<textureRoot["frames"].size();++i)
    {
        if(textureName.compare(textureRoot["frames"][i]["filename"].asString())==0)
        {
            sf::IntRect frame = sf::IntRect(textureRoot["frames"][i]["frame"]["x"].asInt(),
                            textureRoot["frames"][i]["frame"]["y"].asInt(),
                            textureRoot["frames"][i]["frame"]["w"].asInt(),
                            textureRoot["frames"][i]["frame"]["h"].asInt());

            g->setFrame(frame);
            break;
        }
    }

}

///Add a quad representation of an entity objects graphics component to a list of quads to be drawn to screen
void SpriteBatcher::addToBatch(entity* e)
{
    GraphicsComponent* g;
    States::renderState state;
    sf::IntRect i;
    if(e->getID().compare("entity_player")==0)
    {
        player* p;
        p = static_cast<player*>(e);
        g = p->getGraphic();
        g->update(e);
        state = g->getCurrentState();
        i = g->getTextureCoord();

        //std::cout<<i.left<<","<<i.top<<","<<i.width<<","<<i.height<<std::endl;
    }
    //sf::Vertex* quad;

    ///Create a quad struct object and set the 4 corners of it's physical location
    /// and the 4 corners of it's texture position on spritesheet
    batchQuad quad;
    quad.point1 = sf::Vector2f(state.position.x,state.position.y);
    quad.point2 = sf::Vector2f(state.position.x+i.width,state.position.y);
    quad.point3 = sf::Vector2f(state.position.x+i.width,state.position.y+i.height);
    quad.point4 = sf::Vector2f(state.position.x,state.position.y+i.height);

    quad.texPoint1 = sf::Vector2f(i.left,i.top);
    quad.texPoint2 = sf::Vector2f(i.left+i.width,i.top);
    quad.texPoint3 = sf::Vector2f(i.left+i.width,i.top+i.height);
    quad.texPoint4 = sf::Vector2f(i.left,i.top+i.height);

    ///Set the rotation of the quad based on the graphicComponents container entity
    quad.center = sf::Vector2f((quad.point1.x+quad.point2.x+quad.point3.x+quad.point4.x)/4,
                               (quad.point1.y+quad.point2.y+quad.point3.y+quad.point4.y)/4);
    quad.rotation =state.rotation;
    //quad.rotation =270;


    batch_list.push_back(quad);
}

void SpriteBatcher::batchSprites()
{
    sf::Transform rotation;
    b.v.clear();

    if(!batch_list.empty())
    {
        b.v.resize(batch_list.size()*4);
        int j = 0;
        for(int i = 0; i <batch_list.size();++i)
        {
            rotation.rotate(batch_list[i].rotation,batch_list[i].center);
            sf::Vertex* quad = &b.v[j];
            quad[j].position = rotation.transformPoint(batch_list[i].point1);
            quad[j+1].position =rotation.transformPoint(batch_list[i].point2);
            quad[j+2].position =rotation.transformPoint(batch_list[i].point3);
            quad[j+3].position =rotation.transformPoint(batch_list[i].point4);

            quad[j].texCoords = batch_list[i].texPoint1;
            quad[j+1].texCoords =batch_list[i].texPoint2;
            quad[j+2].texCoords =batch_list[i].texPoint3;
            quad[j+3].texCoords =batch_list[i].texPoint4;




            j+=2;
            //batch_vertArray[i]=batch_list[i];
        }
    }

    b.t = batch_texture;
    //b.t.setSmooth(true);
    batch_list.clear();




}

batch SpriteBatcher::getBatch()
{
    return b;
}

SpriteBatcher::~SpriteBatcher()
{

}