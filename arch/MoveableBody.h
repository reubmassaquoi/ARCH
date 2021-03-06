#ifndef MOVEABLEBODY_H
#define MOVEABLEBODY_H



#include "PhysicsBody.h"
#include "GameObject.h"
#include "includes.h"


#include "Actor.h"




class MoveableBody : public PhysicsBody
{
    public:
        MoveableBody();
        virtual ~MoveableBody();
        void update(Actor* container);
        void applyImpact(Actor::ImpactState impactType, b2Vec2 direction);
        void update(GameObject* gObj);

    protected:
    private:
};

#endif //MOVEABLEBODY_H
