#include <iostream>
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include "textures.h"
#include "board.h"
#include "leaker.h"

using namespace std;

void debug(sf::RenderWindow &window, string s, sf::Font &font) {
    sf::Text text(s, font);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    window.draw(text);
}

int main() {
    //textures
    vector<string> filenamevect = {"files/images/debug.png", "files/images/digits.png", "files/images/face_happy.png", "files/images/face_lose.png",
                                   "files/images/face_win.png", "files/images/flag.png", "files/images/leaderboard.png", "files/images/mine.png",
                                   "files/images/number_1.png", "files/images/number_2.png", "files/images/number_3.png", "files/images/number_4.png",
                                   "files/images/number_5.png", "files/images/number_6.png", "files/images/number_7.png", "files/images/number_8.png",
                                   "files/images/pause.png", "files/images/play.png", "files/images/tile_hidden.png", "files/images/tile_revealed.png"};

    textures texturemanager(filenamevect);
    board gameboard(&texturemanager);
    string name = gameboard.welcomewindow();
    gameboard.setname(name);

    bool showleaderboard = false;
    bool gameover = false;
    bool paused = false;
    bool staypaused = false;
    bool firstclick = true;
    bool gamecontinue = true;
    bool pollevents = true;
    bool gameend = false;

    sf::RenderWindow window(sf::VideoMode(gameboard.getWIDTH(), gameboard.getHEIGHT()), "yay minesweeper", sf::Style::Close);
    sf::Clock clock;

    if (name.empty())
        return 0;

    while (window.isOpen())
    {
        if (showleaderboard) {
            gameboard.doleaderboard();
            if (!gameover and !firstclick) {
                clock.restart();
                paused = false;
                gameboard.pause();
            }
            if (staypaused) {
                paused = true;
                gameboard.pause();
                cout << "jaja" << endl;
            }
            showleaderboard = false;
            staypaused = false;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    gameboard.clearall();
                    window.close();
                    gameend = true;
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left and pollevents)
                    {
                        if (event.mouseButton.y < gameboard.getHEIGHT() - 100 and gamecontinue and !paused)
                        {
                            int x = event.mouseButton.x / 32;
                            int y = event.mouseButton.y / 32;

                            bool temp = gameboard.leftclick(y*gameboard.getboardwidth() +x);
                            if (firstclick) {
                                clock.restart();
                                firstclick = false;
                            }

                            if (!temp)
                                gameover = true;
                        }
                        else
                        {
                            showleaderboard = false;
                            string temp = gameboard.buttonclick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                            if (temp == "restart") {
                                clock.restart();
                                gameboard.updateclock(clock.getElapsedTime());
                                firstclick = true;
                                gameover = false;
                                paused = false;
                            }
                            if (temp == "paused" and !gameover) {
                                gameboard.savetime(clock.restart().asSeconds());
                                clock.restart();
                                paused = true;
                            }
                            if (temp == "unpaused") {
                                clock.restart();
                                paused = false;
                            }
                            if (temp == "leaderboard") {
                                if (paused)
                                    staypaused = true;
                                if (!gameover and !firstclick and !paused) {
                                    cout << "hi" << endl;
                                    gameboard.savetime(clock.restart().asSeconds());
                                    clock.restart();
                                    gameboard.pause();
                                    paused = true;
                                }
                                pollevents = false;
                                showleaderboard = true;
                                cout << "lol" << endl;
                            }
                            temp = "";
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right and pollevents)
                    {
                        if (event.mouseButton.y < gameboard.getHEIGHT() - 100 and gamecontinue) {
                            int x = event.mouseButton.x / 32;
                            int y = event.mouseButton.y / 32;
                            gameboard.rightclick(y*gameboard.getboardwidth() +x);
                        }
                    }
            }
        }
        if (gameend)
            break;
        if (!showleaderboard)
            pollevents = true;
        if (!gameover and !paused and !firstclick)
            gameboard.updateclock(clock.getElapsedTime());

        window.clear(sf::Color(215, 215, 215));
        gameboard.updatesprites(window);
        gameboard.updatebuttons(window);
        window.display();
    }
    return 0;
}
