//
// Created by funny on 4/16/2023.
//

#ifndef OPTIMIZEDMINESWEEPER_CELL_H
#define OPTIMIZEDMINESWEEPER_CELL_H
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include "textures.h"

class cell {
    std::string type;
    int xindex;
    int yindex;
    textures* texturemanager;
    std::vector<cell*> nearbycellsvect;
    sf::Sprite frontsprite;
    sf::Sprite backsprite;
    sf::Sprite flagsprite;
    sf::Sprite* frontspriteptr;
    sf::Sprite* backspriteptr;
    sf::Sprite* flagspriteptr;
    bool revealed;
    bool flagged;

public:
    cell(textures* ptr, int x, int y);
    void setnearbycellsvect(std::vector<cell*> vect);
    int getxindex();
    int getyindex();
    void printcellvect();
    void printcords();
    void settype(std::string s);
    std::string gettype();
    void setnearbybombcount();
    sf::Sprite* getfrontspriteptr();
    sf::Sprite* getbackspriteptr();
    sf::Sprite* getflagspriteptr();
    bool leftclick();
    void clearsurroundingzeroes();
    bool isrevealed();
    bool rightclick();
    bool isflagged();
    void setcellsprite();
    void clearcell();
    void pause();
    void unpause();
    void setflagged(bool f);
    void setrevealed(bool f);
    void setred();

};


#endif //OPTIMIZEDMINESWEEPER_CELL_H
