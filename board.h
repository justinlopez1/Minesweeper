//
// Created by funny on 4/16/2023.
//

#ifndef OPTIMIZEDMINESWEEPER_BOARD_H
#define OPTIMIZEDMINESWEEPER_BOARD_H
#include "textures.h"
#include "cell.h"
#include "button.h"
#include "leaderboard.h"

class board {
    int WIDTH;
    int HEIGHT;
    int boardwidth;
    int boardheight;
    int bombcount;
    int flagcounter;
    std::string playername;
    leaderboard* l;
    textures* texturemanager;
    std::vector<cell*> cellvect;
    std::vector<button*> buttonvect;
    std::vector<sf::Sprite> bombdigits;
    std::vector<sf::Sprite> clockdigits;
    sf::Sprite negative;
    bool firstclick = true;
    bool paused = false;
    bool gameover = false;
    bool debug = false;
    bool displaynegative = false;
    float savedtime = 0;
    std::string time = "0000";
public:
    bool isgameover();
    board(textures* ptr);
    std::string welcomewindow();
    void setcellvects();
    void printcellvects();
    void setbombs(int index);
    void setnumbers();
    void printboard();
    void updatesprites(sf::RenderWindow &window);
    int getWIDTH();
    int getHEIGHT();
    bool leftclick(int i);
    int getboardwidth();
    int rightclick(int i);
    void createbuttons();
    void updatebuttons(sf::RenderWindow &window);
    std::string buttonclick(sf::Vector2f pos);
    void restart();
    bool gamewon();
    void pause();
    void updateclock(sf::Time t);
    void savetime(float t);
    void doleaderboard();
    void setname(std::string n);
    void clearall();
};


#endif //OPTIMIZEDMINESWEEPER_BOARD_H
