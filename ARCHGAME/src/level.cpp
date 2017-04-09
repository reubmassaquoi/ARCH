#include "level.h"
#include "EntitySpawner.h"
#include "SpriteBatcher.h"
#include "GameMap.h"


class EntitySpawner;
class SpriteBatcher;
class GameMap;


camera gameCamera;

struct spawnPoint
{
    std::string spawnID;
    b2Vec2 location;
    float rotation;


};
level::level()
{
    ///Creates A spawner object for creating entites and a batcher object for assembling sprites in a vertex array
    spawner = new EntitySpawner;
    batcher = new SpriteBatcher;
    _map = new GameMap;
    entityList= {};
}

level::~level()
{
    //dtor
}

/**
This function loads a specified level and retrieves and stores data

**/
void level::load(std::string mapFile)
{

    _map->loadFile(mapFile);

    _map->create();

    spawnPoint pSpawn;

    pSpawn.spawnID = "entity_player";
    pSpawn.location =b2Vec2(3,20);
    pSpawn.rotation =0;
    batcher->loadEntityTextures();
    ///TEST CODE PLEASE REMOVE LATER
    for(int i = 0; i<1;++i)
    {
        entityList.push_back(spawner->spawnEntity(pSpawn));
        if(entityList[i]->getID().compare("entity_player")==0)
        {
             gameCamera.setTarget(entityList[i]);
        }
        /**
        THIS IS A PROBLEM IN THE MAKING
        having to cast the entity in order to set it's frame texture
        **/
        batcher->setFrameTexture(((player*)entityList[i])->getGraphic(),"archii_texture.png");
        pSpawn.location.y+=5;
    }
    //entityList.push_back(spawner->spawnEntity(pSpawn));


    //batcher->setFrameTexture(((player*)entityList[0])->getGraphic(),"archii_texture.png");
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
        gameCamera.update();
    }

}

///Draws Entites along with the map
void level::render(sf::RenderWindow* window,double alpha)
{
    if(!entityList.empty())
    {
        for(int i = 0; i<entityList.size();++i)
        {
            batcher->addToBatch(entityList[i],alpha);
        }
    }

    batcher->batchSprites();

    //window->draw(_map->getFirstLayer());
    //window->draw(_map->getSecondLayer());
    window->draw(_map->layer1);
    window->draw(_map->layer2);

    window->draw(batcher->b);


}



void level::physicsUpdate(float dt, float a)
{
    const int maxSteps = 5;
    float32 timeStep = 1.f/60.f;
    //a = 0.0f;
    a+=dt;
    int numSteps = static_cast<int>(std::floor(a/timeStep));
    if(numSteps>0)
    {
        a -=(numSteps*timeStep);
    }
    assert (
		"Accumulator must have a value lesser than the fixed time step" &&
		a < timeStep + FLT_EPSILON
	);
    int clampedNumSteps = std::min(numSteps,maxSteps);
    //std::cout<<clampedNumSteps<<std::endl;


    for(int i = 0;i<clampedNumSteps;++i)
    {
        world->Step(timeStep,6,2);
    }
    world->ClearForces();
    /*
    PhysicsComponent* ph;
    player* p;
    GraphicsComponent* g;
    float alpha = a/timeStep;
    //update(.01f);
    if(!entityList.empty())
    {
        //std::cout<<"GOT HERE"<<std::endl;
       // for(int i = 0; i<entityList.size();++i)
        //{
            if(entityList[0]->getID().compare("entity_player")==0)
            {

                p = static_cast<player*>(entityList[0]);
                ph = p->getPhysics();
                //ph->update(.01f);
                States::positionState state = States::lerpPositionState(ph->getPreviousState(),ph->getCurrentState(),alpha);
                ph->setPosition(state.position);
                ph->setRotation(state.rotation);

            }
            //entityList[i]->update(dt);
            //std::cout<<entityList[i]->getPosition().x/30<<","<<entityList[i]->getPosition().y<<std::endl;

    }

*/
}

void level::physicsSmooth(float alpha)
{
    PhysicsComponent* ph;
    player* p;
    if(!entityList.empty())
    {


        //std::cout<<"GOT HERE"<<std::endl;
       for(int i = 0; i<entityList.size();++i)
        {
            if(entityList[i]->getID().compare("entity_player")==0)
            {

                p = static_cast<player*>(entityList[i]);

                ph = p->getPhysics();
                b2Vec2 cur = p->getPosition();
                b2Vec2 pre = ph->getBody()->GetPosition();
                //std::cout<<cur.x<<","<<pre.x<<std::endl;
                float curAngle = p->getRotation();
                float preAngle = ph->getBody()->GetAngle();
                cur.x = (pre.x*alpha+cur.x*(1.0f-alpha));
                cur.y = (pre.y*alpha+cur.y*(1.0f-alpha));
                curAngle = (curAngle*alpha+preAngle*(1.0f-alpha));
                p->setPosition(cur);
                p->setRotation(curAngle);

                //States::positionState state = //States::lerpPositionState(ph->getPreviousState(),ph->getCurrentState(),a);

                /////>update(.01);
            }
            //entityList[i]->update(dt);
            //std::cout<<entityList[i]->getPosition().x/30<<","<<entityList[i]->getPosition().y<<std::endl;
        }
    }
}


