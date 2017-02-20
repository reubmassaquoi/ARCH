#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <string>
#include <math.h>
#include <iostream>

extern b2World* world;
namespace States
{
   struct positionState
   {
       b2Vec2 postion;
       b2Vec2 velocity;
       b2Vec2 acceleration;
       float rotation;
   };
}


class Game
{
    public:
        Game();
        virtual ~Game();
        void start();
    protected:
    private:
        /**Game Variables**/

        bool gameRunning;

        /**SFML Variables**/
        sf::Vector2f v2f_windowSize;
        sf::RenderWindow* window;
        sf::Keyboard keyBoard;
        sf::Mouse mouse;
        sf::Vector2f v2f_mousePos;
        sf::Event event;
        sf::Clock clock;

        sf::Time currentTime;
        float flPreviousTime;
        float dt;

        /**Box2d Variables**/

        float32 timeStep;
        int32 velocityIterations;
        int32 positionIterations;




        void processInput();
        void update(float dt);
        void render();


};

#endif // GAME_H