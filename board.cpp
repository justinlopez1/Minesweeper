//
// Created by funny on 4/16/2023.
//

#include "board.h"
#include "fstream"
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

std::string board::welcomewindow() {

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) cout << "font load error" << endl;

    sf::Text gametext("WELCOME TO MINESWEEPER!", font, 24);
    gametext.setStyle(sf::Text::Bold | sf::Text::Underlined);
    gametext.setPosition(float(WIDTH)/2 - gametext.getLocalBounds().width/2, (float(HEIGHT)/2) - 150);

    sf::Text nameinputtext("Enter your name:", font, 20);
    nameinputtext.setStyle(sf::Text::Bold);
    nameinputtext.setPosition(float(WIDTH)/2 - nameinputtext.getLocalBounds().width/2, float(HEIGHT)/2 - 75);

    sf::String playername;
    sf::Text nametext(playername, font, 18);
    nametext.setStyle(sf::Text::Bold);
    nametext.setFillColor(sf::Color::Yellow);
    nametext.setPosition(float(WIDTH)/2, float(HEIGHT)/2 - 45);

    sf::Text cursor("|", font, 18);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setPosition(float(WIDTH)/2, float(HEIGHT)/2 - 45);
    cursor.setStyle(sf::Text::Bold);
    sf::Clock time;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "yay minesweeper");
    bool closewindow = false;
    while (window.isOpen() and !closewindow)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    closewindow = true;
                    return "";

                case sf::Event::TextEntered:
                    if (event.text.unicode == '\b' and playername.getSize() > 0) {
                        string temp = playername.substring(0, playername.getSize()-1);
                        playername = temp;
                        nametext.setString(playername);
                    }
                    else if (event.text.unicode == 13 and playername.getSize() > 0) {
                        closewindow = true;
                    }
                    else if (((event.text.unicode > 64 and event.text.unicode < 91) or
                              (event.text.unicode > 96 and event.text.unicode < 123)) and
                             playername.getSize() < 10) {
                        playername += event.text.unicode;
                        nametext.setString(playername);
                    }

                    nametext.setPosition(WIDTH/2 - nametext.getLocalBounds().width/2, nametext.getPosition().y);
                    cursor.setPosition(WIDTH/2 + nametext.getLocalBounds().width/2, cursor.getPosition().y);
                    break;
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(gametext);
        window.draw(nameinputtext);
        window.draw(nametext);
        if (time.getElapsedTime().asSeconds() >= 0.5)
            window.draw(cursor);
        if (time.getElapsedTime().asSeconds() >= 1)
            time.restart();
        window.display();
    }

    if (playername.getSize() == 0)
        return "";

    string firstletter = playername.substring(0, 1);
    string lastletters = playername.substring (1);
    transform(firstletter.begin(), firstletter.end(), firstletter.begin(), ::toupper);
    transform(lastletters.begin(), lastletters.end(), lastletters.begin(), ::tolower);
    return firstletter + lastletters;

}

board::board(textures* ptr) {
    firstclick = true;
    flagcounter = 0;
    fstream file("files/board_config.cfg", ios_base::in);
    string read;
    getline(file, read);
    WIDTH = stoi(read) * 32;
    boardwidth = stoi(read);
    getline(file, read);
    HEIGHT = stoi(read) * 32 + 100;
    boardheight = stoi(read);
    getline(file, read);
    bombcount = stoi(read);
    file.close();
    for (int i = 0; i < boardwidth * boardheight; i++) {
        cellvect.push_back(new cell(ptr, i%boardwidth, i/boardwidth));
    }
    texturemanager = ptr;
    setcellvects();
    l = new leaderboard(WIDTH, HEIGHT);
}

void board::setcellvects() {
    for (int i = 0; i < cellvect.size(); i ++) {
        cell *thiscell = cellvect[i];
        vector<cell *> tempcellvect;
        if (thiscell->getyindex() != 0 and thiscell->getxindex() != 0)
            tempcellvect.push_back(cellvect[i - boardwidth - 1]);

        if (thiscell->getyindex() != 0)
            tempcellvect.push_back(cellvect[i - boardwidth]);

        if (thiscell->getyindex() != 0 and thiscell->getxindex() != boardwidth - 1)
            tempcellvect.push_back(cellvect[i - boardwidth + 1]);

        if (thiscell->getxindex() != boardwidth - 1)
            tempcellvect.push_back(cellvect[i + 1]);

        if (thiscell->getyindex() != boardheight - 1 and thiscell->getxindex() != boardwidth - 1)
            tempcellvect.push_back(cellvect[i + boardwidth + 1]);

        if (thiscell->getyindex() != boardheight - 1)
            tempcellvect.push_back(cellvect[i + boardwidth]);

        if (thiscell->getyindex() != boardheight - 1 and thiscell->getxindex() != 0)
            tempcellvect.push_back(cellvect[i + boardwidth - 1]);

        if (thiscell->getxindex() != 0)
            tempcellvect.push_back(cellvect[i - 1]);

        cellvect[i]->setnearbycellsvect(tempcellvect);
    }
}

void board::printcellvects() {
    for (int i = 0; i < cellvect.size(); i++) {
        cellvect[i]->printcellvect();
        cout << endl;
    }
}

void board::setbombs(int index) {
    vector<string> tempvect;
    if (bombcount >= boardwidth * boardheight) {
        bombcount = boardwidth * boardheight;
        cout << "Error: More bombs than area\n";
        for (int i = 0; i < bombcount; i++) {
            tempvect.push_back("X");
        }
    }
    else {
        for (int i = 0; i < cellvect.size(); i++)
            tempvect.emplace_back("0");

        for (int i = 0; i < bombcount; i++)
            tempvect[i] = "X";

        while (true) {
            mt19937 randomizer(chrono::steady_clock::now().time_since_epoch().count());
            shuffle(tempvect.begin(), tempvect.end(), randomizer);
            if (tempvect[index] != "X")
                break;
        }
    }

    for (int i = 0; i < cellvect.size(); i++) {
        if (tempvect[i] == "X") {
            cellvect[i]->settype("X");

        }

    }
}

void board::printboard() {
    int count = 0;
    for (int i = 0; i < cellvect.size(); i++) {
        cout << cellvect[i]->gettype() << " ";
        count++;
        if (count == boardwidth) {
            count = 0;
            cout << endl;
        }
    }
}

void board::setnumbers() {
    for (int i = 0; i < boardwidth * boardheight; i++) {
        if (cellvect[i]->gettype() != "X") {
            cellvect[i]->setnearbybombcount();
        }
        cellvect[i]->setcellsprite();

    }
}

void board::updatesprites(sf::RenderWindow &window) {
    for (int i = 0; i < cellvect.size(); i++) {
        window.draw(*cellvect[i]->getbackspriteptr());
        if (cellvect[i]->isflagged() and !paused) {
            window.draw(*cellvect[i]->getflagspriteptr());
        }
        if (cellvect[i]->isrevealed() and !paused) {
            window.draw(*cellvect[i]->getfrontspriteptr());
        }
        if (debug and cellvect[i]->gettype() == "X" and !paused) {
            window.draw(*cellvect[i]->getfrontspriteptr());
        }
    }
    for (int i = 0; i < bombdigits.size(); i++) {
        window.draw(bombdigits[i]);
    }
    for (int i = 0; i < clockdigits.size(); i++) {
        window.draw(clockdigits[i]);
    }
    if (displaynegative)
        window.draw(negative);
}

int board::getWIDTH() {
    return WIDTH;
}

int board::getHEIGHT() {
    return HEIGHT;
}

bool board::leftclick(int i) {

    cout << "width: " << cellvect[i]->getbackspriteptr()->getGlobalBounds().width << endl;
    cout << "height: " << cellvect[i]->getbackspriteptr()->getGlobalBounds().height << endl;

    if (firstclick) {

        setbombs(i);
        setnumbers();
        firstclick = false;
    }
    if (!gameover) {
        if (!cellvect[i]->leftclick()) {
            cout << "lose" << endl;
            gameover = true;
            buttonvect[0]->buttonsprite.setTexture(*texturemanager->gettexture("face_lose"));
            cellvect[i]->setred();

            for (auto & j : cellvect) {
                if (j->gettype() == "X") {
                    j->setrevealed(true);
                }
            }
            return false;
        }

    }

    if (gamewon()) {
        gameover = true;
        cout << time << endl;
        buttonvect[0]->buttonsprite.setTexture(*texturemanager->gettexture("face_win"));
        l->addscore(time.substr(0, 2) + ":" + time.substr(2, 2), playername);
        for (auto & j : cellvect) {
            if (j->gettype() == "X") {
                j->setflagged(true);
            }
        }
        if (debug)
            debug = false;

        vector<string> tempvect;
        for (int j = 0; j < 3; j++) {
            tempvect.push_back("0");
        }
        for (int j = 0; j < tempvect.size(); j++) {
            bombdigits[j].setTextureRect(texturemanager->getdigit(tempvect[j]));
        }

        return false;
    }
    
    return true;
    
}

int board::getboardwidth() {
    return boardwidth;
}

int board::rightclick(int i) {
    if (!cellvect[i]->isrevealed() and !gameover) {
        bool temp = cellvect[i]->rightclick();
        if (temp){
            flagcounter++;
        }
        else if (!temp) {
            flagcounter--;
        }

        int tempint = bombcount - flagcounter;
        string tempstr;
        cout << tempint << endl;

        if (tempint < 0) {
            displaynegative = true;
            tempint = -tempint;
            tempstr = to_string(tempint);
        }
        else {
            displaynegative = false;
            tempstr = to_string(tempint);
        }

        if (tempstr.size() == 1)
            tempstr = "00" + tempstr;
        else if (tempstr.size() == 2)
            tempstr = "0" + tempstr;
        vector<string> tempvect;
        for (int j = 0; j < tempstr.size(); j++) {
            tempvect.push_back(tempstr.substr(j, 1));
        }
        for (int j = 0; j < tempvect.size(); j++) {
            bombdigits[j].setTextureRect(texturemanager->getdigit(tempvect[j]));
        }

    }

    return 0;
}

void board::createbuttons() {
    buttonvect.push_back(new button("face_happy", texturemanager, sf::Vector2f(WIDTH/2-32, HEIGHT-50-32)));
    buttonvect.push_back(new button("leaderboard", texturemanager, sf::Vector2f(WIDTH-176, HEIGHT-50-32)));
    buttonvect.push_back(new button("pause", texturemanager, sf::Vector2f(WIDTH-240, HEIGHT-50-32)));
    buttonvect.push_back(new button("debug", texturemanager, sf::Vector2f(WIDTH-304, HEIGHT-50-32)));

    for (int i = 0; i < 2; i++) {
        sf::Sprite s;
        s.setPosition(WIDTH-97 + (21 * i), HEIGHT-50-16);
        s.setTexture(*texturemanager->gettexture("digits"));
        s.setTextureRect(texturemanager->getdigit("0"));
        clockdigits.push_back(s);
    }
    for (int i = 2; i < 4; i++) {
        sf::Sprite s;
        s.setPosition(WIDTH-96 + (21 * i), HEIGHT-50-16);
        s.setTexture(*texturemanager->gettexture("digits"));
        s.setTextureRect(texturemanager->getdigit("0"));
        clockdigits.push_back(s);
    }



    string tempstr = to_string(bombcount);
    if (tempstr.size() == 1)
        tempstr = "00" + tempstr;
    else if (tempstr.size() == 2)
        tempstr = "0" + tempstr;
    vector<string> tempvect;
    for (int i = 0; i < tempstr.size(); i++) {
        tempvect.push_back(tempstr.substr(i, 1));
    }

    for (int i = 0; i < tempvect.size(); i++) {
        sf::Sprite s;
        s.setPosition(33 + (21 * i), HEIGHT-50-16);
        s.setTexture(*texturemanager->gettexture("digits"));
        s.setTextureRect(texturemanager->getdigit(tempvect[i]));
        bombdigits.push_back(s);
    }
    negative.setTexture(*texturemanager->gettexture("digits"));
    negative.setTextureRect(texturemanager->getdigit("-"));
    negative.setPosition(12, HEIGHT-50-16);
}

void board::updatebuttons(sf::RenderWindow &window) {
    for (int i = 0; i < buttonvect.size(); i++) {
        window.draw(buttonvect[i]->buttonsprite);
    }
}

std::string board::buttonclick(sf::Vector2f pos) {
    for (int i = 0; i < buttonvect.size(); i++) {
        if (buttonvect[i]->click(pos)) {
            if (buttonvect[i]->name == "face_happy") {
                restart();
                return "restart";
            }
            else if (buttonvect[i]->name == "debug" and !firstclick and !gameover and !paused) {
                debug = !debug;
            }
            else if (buttonvect[i]->name == "pause" and !gameover and !firstclick) {
                pause();
                if (paused) return "paused";
                if (!paused) return "unpaused";
            }
            else if (buttonvect[i]->name == "leaderboard") {
                return "leaderboard";
            }
        }
    }
    return "";

}

void board::restart() {
    buttonvect[0]->buttonsprite.setTexture(*texturemanager->gettexture("face_happy"));
    savedtime = 0;
    flagcounter = 0;
    gameover = false;
    paused = false;
    gameover = false;
    debug = false;
    displaynegative = false;
    buttonvect[2]->buttonsprite.setTexture(*texturemanager->gettexture("pause"));
    for (int i = 0; i < cellvect.size(); i++) {
        cellvect[i]->clearcell();
        cellvect[i] = nullptr;
    }
    firstclick = true;
    for (int i = 0; i < boardwidth * boardheight; i++) {
        cellvect[i] = (new cell(texturemanager, i%boardwidth, i/boardwidth));
    }
    setcellvects();

    string tempstr = to_string(bombcount);
    if (tempstr.size() == 1)
        tempstr = "00" + tempstr;
    else if (tempstr.size() == 2)
        tempstr = "0" + tempstr;
    vector<string> tempvect;

    for (int i = 0; i < tempstr.size(); i++) {
        tempvect.push_back(tempstr.substr(i, 1));
    }
    for (int i = 0; i < bombdigits.size(); i++) {
        bombdigits[i].setTextureRect(texturemanager->getdigit(tempvect[i]));
        cout << tempvect[i];
    }

    for (int i = 0; i < tempvect.size(); i++) {
        bombdigits[i].setTextureRect(texturemanager->getdigit(tempvect[i]));
    }
    time = "0000";
}

bool board::gamewon() {
    for (int i = 0; i < cellvect.size(); i++) {
        if (!cellvect[i]->isrevealed() and cellvect[i]->gettype() != "X")
            return false;
    }
    return true;
}

void board::pause() {
    paused = !paused;
    if (paused) {
        for (int i = 0; i < cellvect.size();i ++) {
            cellvect[i]->pause();
        }
        buttonvect[2]->buttonsprite.setTexture(*texturemanager->gettexture("play"));
    }
    else {
        for (int i = 0; i < cellvect.size();i ++) {
            cellvect[i]->unpause();
        }
        buttonvect[2]->buttonsprite.setTexture(*texturemanager->gettexture("pause"));
    }
}

void board::updateclock(sf::Time t) {
    int total = t.asSeconds() + savedtime;
    string secondsstr = to_string(total % 60);
    string minutesstr = to_string(total / 60);
    if (secondsstr.size() == 1)
        secondsstr = "0" + secondsstr;
    if (minutesstr.size() == 1)
        minutesstr = "0" + minutesstr;
    time = minutesstr + secondsstr;
    clockdigits[0].setTextureRect(texturemanager->getdigit(minutesstr.substr(0, 1)));
    clockdigits[1].setTextureRect(texturemanager->getdigit(minutesstr.substr(1, 1)));
    clockdigits[2].setTextureRect(texturemanager->getdigit(secondsstr.substr(0, 1)));
    clockdigits[3].setTextureRect(texturemanager->getdigit(secondsstr.substr(1, 1)));
}

bool board::isgameover() {
    return gameover;
}

void board::savetime(float t) {
    savedtime += t;
}

void board::doleaderboard() {
    l->leaderboardwindow();
}

void board::setname(std::string n) {
    playername = n;
    createbuttons();
}

void board::clearall() {
    l->clear();
    l = nullptr;
    for (int i = 0; i < buttonvect.size(); i++) {
        buttonvect[i]->clear();
        buttonvect[i] = nullptr;
    }
    for (int i = 0; i < cellvect.size(); i++) {
        delete cellvect[i];
        cellvect[i] = nullptr;
    }
}




