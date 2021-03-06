#ifndef CAM_H
#define CAM_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "includes.h"


class cam
{
    public:
        cam(sf::Vector2f pos, sf::Vector2f cameraDimension);
        void follow(sf::Vector2i point);
        void frame();
        void update();
        sf::View getView();
        sf::View camView;
        sf::Vector2f v2f_Position;
        sf::Vector2i v2i_FocalPoint;
        sf::Vector2f v2f_CamDim;
        virtual ~cam();
    protected:
    private:
};

#endif // CAM_H
