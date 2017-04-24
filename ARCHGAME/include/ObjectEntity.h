#ifndef OBJECTENTITY_H
#define OBJECTENTITY_H

#include <entity.h>
#include "PhysicsComponent.h"


class ObjectEntity : public entity
{
    public:
        ObjectEntity(std::string e_ID, PhysicsComponent* p);
        virtual ~ObjectEntity();

        void setPosition(b2Vec2 p);
        void setRotation(float a);
        b2Vec2 getPosition();
        float getRotation();

        void update();
        std::string getID();
        std::string getType();

        States::positionState getCurrentState();
        States::positionState getPreviousState();

        PhysicsComponent* getPhysics();
    protected:
    private:
        PhysicsComponent* physics_component;
};

#endif // OBJECTENTITY_H
