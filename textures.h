//
// Created by funny on 4/16/2023.
//

#ifndef OPTIMIZEDMINESWEEPER_TEXTURES_H
#define OPTIMIZEDMINESWEEPER_TEXTURES_H
#include <map>
#include <vector>
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>

class textures {
    std::map<std::string, sf::Texture> texturemap;
    std::map<std::string, sf::IntRect>  digitmap;

public:
    textures(std::vector<std::string> imagenames);
    sf::Texture* gettexture(std::string texturename);
    sf::IntRect getdigit(std::string digitname);
};


#endif //OPTIMIZEDMINESWEEPER_TEXTURES_H
