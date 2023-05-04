//
// Created by funny on 4/17/2023.
//

#include "button.h"
#include <iostream>
using namespace std;

button::button(std::string n, textures* ptr, sf::Vector2f pos) {
    name = n;
    buttonsprite.setTexture(*ptr->gettexture(n));
    buttonsprite.setPosition(pos);

}

bool button::click(sf::Vector2f pos) {
    if (buttonsprite.getGlobalBounds().contains(pos))
        cout << "clicked " << name << endl;
    return buttonsprite.getGlobalBounds().contains(pos);
}

void button::clear() {
    delete this;
}
