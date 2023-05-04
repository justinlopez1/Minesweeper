//
// Created by funny on 4/17/2023.
//

#ifndef OPTIMIZEDMINESWEEPER_BUTTON_H
#define OPTIMIZEDMINESWEEPER_BUTTON_H
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include "textures.h"

struct button {
    sf::Sprite buttonsprite;
    std::string name;

    button(std::string n, textures* ptr, sf::Vector2f pos);
    bool click(sf::Vector2f pos);
    void clear();
};


#endif //OPTIMIZEDMINESWEEPER_BUTTON_H
