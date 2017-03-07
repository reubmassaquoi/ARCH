#include "level.h"
#include "EntitySpawner.h"
#include "SpriteBatcher.h"

class EntitySpawner;
class SpriteBatcher;


struct spawnPoint
{
    std::string spawnID;
    b2Vec2 location;
    float rotation;


};
level::level()
{
    //ctor
    ///Creates A spawner object for creating entites and a batcher object for assembling sprites in a vertex array
    spawner = new EntitySpawner;
    batcher = new SpriteBatcher;
    entityList= {};
}

level::~level()
{
    //dtor
}

/**
This function loads a specified level and retrieves and stores data

**/
void level::load()
{
    spawnPoint pSpawn;
    pSpawn.spawnID = "entity_player";
    pSpawn.location =b2Vec2(300/30,300/30);
    pSpawn.rotation = 1.5;
    batcher->loadEntityTextures();
    entityList.push_back(spawner->spawnEntity(pSpawn));
    batcher->setFrameTexture(((player*)entityList[0])->getGraphic(),"archii_texture.png");
}


///Updates game entities by a dt variable
void level::update(float dt)
{
    if(!entityList.empty())
    {
        //std::cout<<"GOT HERE"<<std::endl;
        for(int i = 0; i<entityList.size();++i)
        {

            entityList[i]->update(dt);
            //std::cout<<entityList[i]->getPosition().x/30<<","<<entityList[i]->getPosition().y<<std::endl;
        }
    }

}

///Draws Entites along with the map
void level::render(sf::RenderWindow* window)
{
     if(!entityList.empty())
    {
        for(int i = 0; i<entityList.size();++i)
        {
            batcher->addToBatch(entityList[i]);
        }
    }

    batcher->batchSprites();

    window->draw(batcher->getBatch());


}
