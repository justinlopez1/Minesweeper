//
// Created by funny on 4/17/2023.
//

#include "leaderboard.h"
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
using namespace std;

void leaderboard::leaderboardwindow() {

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) cout << "font load error" << endl;

    sf::Text leaderboardtitle("LEADERBOARD", font, 20);
    leaderboardtitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    leaderboardtitle.setPosition(WIDTH / 4 - 70, HEIGHT / 4 - 120);

    string temp = "";
    for (int i = 0; i < 5; i++) {
        string place = to_string(i + 1);
        temp += place + ".     ";
        string entry = scorevect[i].time + "    " + scorevect[i].name;
        temp += entry;
        if (scorevect[i].recentscore)
            temp += "*";
        temp += "\n\n";
    }

    sf::Text leaderboardscores(temp, font, 18);
    leaderboardscores.setStyle(sf::Text::Bold);
    leaderboardscores.setPosition(WIDTH / 4 - 119, HEIGHT / 4 + 120 - 195);

    sf::RenderWindow window(sf::VideoMode(WIDTH / 2, HEIGHT / 2), "Leaderboard yaayy");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear(sf::Color::Blue);

        window.draw(leaderboardtitle);
        window.draw(leaderboardscores);

        window.display();
    }

}

leaderboard::leaderboard(int w, int h) {
    WIDTH = w;
    HEIGHT = h;
    fstream file("files/leaderboard.txt", ios_base::in);
    if (not file.is_open()) {
        cout << "file failed to open" << endl;
    }
    for (int i = 0; i < 5; i++) {
        string time;
        string name;
        string temp;
        getline(file, temp);
        time = temp.substr(0, 5);
        name = temp.substr(6);
        scorevect.emplace_back(score(time, name, false));
    }
}

void leaderboard::addscore(std::string t, std::string n) {
    int minues = stoi(t.substr(0, 2));
    int seconds = stoi(t.substr(3, 2));
    int total = (minues * 60) + seconds;
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (total > scorevect[i].total)
            count++;
    }
    for (int i = 0; i < scorevect.size(); i++) {
        scorevect[i].recentscore = false;
    }
    scorevect.insert(scorevect.begin() + count, score(t, n, true));
    ofstream file("files/leaderboard.txt", ios_base::out);
    for (int i = 0; i < 5; i++) {
        string temp = scorevect[i].time + "," + scorevect[i].name;
        file << temp << "\n";
    }
}

void leaderboard::clear() {
    delete this;
}

score::score(std::string Time, std::string Name, bool recent) {
    time = Time;
    name = Name;
    recentscore = recent;
    int minues = stoi(time.substr(0, 2));
    int seconds = stoi(time.substr(3, 2));
    total = (minues * 60) + seconds;
}
