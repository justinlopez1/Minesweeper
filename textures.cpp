//
// Created by funny on 4/16/2023.
//

#include "textures.h"
#include <iostream>
#include <string>
using namespace std;

textures::textures(std::vector<std::string> imagenames) {
    for (int i = 0; i < imagenames.size(); i++) {
        sf::Texture t;
        string temp = imagenames[i].substr(13, imagenames[i].size() - 17);
        texturemap.emplace(temp, t);
        texturemap[temp].loadFromFile(imagenames[i]);
    }
    for (int i = 0; i < 10; i++) {
        string temp = to_string(i);
        digitmap.emplace(temp, sf::IntRect(21 * i, 0, 21, 32));
    }
    digitmap.emplace("-", sf::IntRect(210, 0, 21, 32));

}

sf::Texture *textures::gettexture(std::string texturename) {
    sf::Texture* newptr = &texturemap[texturename];
    return newptr;
}

sf::IntRect textures::getdigit(std::string digitname) {
    return digitmap[digitname];
}
