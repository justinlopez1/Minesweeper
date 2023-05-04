//
// Created by funny on 4/16/2023.
//

#include "cell.h"
#include <iostream>
using namespace std;

cell::cell(textures* ptr, int x, int y) {
    texturemanager = ptr;
    revealed = false;
    flagged = false;
    type = "0";
    backsprite.setTexture(*ptr->gettexture("tile_hidden"));
    backsprite.setPosition(x * 32, y * 32);
    flagsprite.setTexture(*ptr->gettexture("flag"));
    flagsprite.setPosition(x * 32, y * 32);
    frontsprite.setPosition(x * 32, y * 32);
    frontspriteptr = &frontsprite;
    backspriteptr = & backsprite;
    flagspriteptr = &flagsprite;
    xindex = x;
    yindex = y;
}

void cell::setnearbycellsvect(std::vector<cell*> vect) {
    for (int i = 0; i < vect.size(); i++) {
        nearbycellsvect.push_back(vect[i]);
    }
}

int cell::getxindex() {
    return xindex;
}

int cell::getyindex() {
    return yindex;
}

void cell::printcellvect() {
    for (int i = 0; i < nearbycellsvect.size(); i++) {
        nearbycellsvect[i]->printcords();
        cout << " ";
    }
}

void cell::printcords() {
    cout << "(" << xindex << ", " << yindex << ")";
}

void cell::settype(std::string s) {
    type = s;
}

std::string cell::gettype() {
    return type;
}

void cell::setnearbybombcount() {
    int count = 0;
    for (int i = 0; i < nearbycellsvect.size(); i++) {
        if (nearbycellsvect[i]->gettype() == "X")
            count++;
    }
    type = to_string(count);
}

sf::Sprite *cell::getfrontspriteptr() {
    return frontspriteptr;
}

sf::Sprite *cell::getbackspriteptr() {
    return backspriteptr;
}

bool cell::leftclick() {
    if (!flagged and !revealed) {
        revealed = true;
        backsprite.setTexture(*texturemanager->gettexture("tile_revealed"));
        if (type == "0")
            clearsurroundingzeroes();
        if (type == "X") {
            return false;
        }
    }
    return true;
}

void cell::clearsurroundingzeroes() {
    revealed = true;
    flagged = false;
    backsprite.setTexture(*texturemanager->gettexture("tile_revealed"));
    for (int i = 0; i < nearbycellsvect.size(); i++) {
        if (!nearbycellsvect[i]->revealed and !nearbycellsvect[i]->flagged)
            if (nearbycellsvect[i]->type == "0") {
                nearbycellsvect[i]->clearsurroundingzeroes();
            }
            else if (nearbycellsvect[i]->type != "X") {
                nearbycellsvect[i]->revealed = true;
                nearbycellsvect[i]->backsprite.setTexture(*texturemanager->gettexture("tile_revealed"));
                nearbycellsvect[i]->flagged = false;
            }
    }
}

bool cell::isrevealed() {
    return revealed;
}

bool cell::rightclick() {
    flagged = !flagged;
    return flagged;
}

bool cell::isflagged() {
    return flagged;
}

sf::Sprite *cell::getflagspriteptr() {
    return flagspriteptr;
}

void cell::setcellsprite() {
    if (type == "X") {
        frontsprite.setTexture(*texturemanager->gettexture("mine"));
    }
    else
        frontsprite.setTexture(*texturemanager->gettexture("number_" + type));
}

void cell::clearcell() {
    delete this;
}

void cell::pause() {
    backsprite.setTexture(*texturemanager->gettexture("tile_revealed"));
}

void cell::unpause() {
    if (!revealed)
        backsprite.setTexture(*texturemanager->gettexture("tile_hidden"));
}

void cell::setflagged(bool f) {
    flagged = f;
}

void cell::setrevealed(bool f) {
    revealed = f;
    if (f) {
        backsprite.setTexture(*texturemanager->gettexture("tile_revealed"));
    }
}

void cell::setred() {
    backsprite.setColor(sf::Color(250, 0, 0));
}
