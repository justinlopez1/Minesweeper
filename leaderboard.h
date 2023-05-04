//
// Created by funny on 4/17/2023.
//

#ifndef OPTIMIZEDMINESWEEPER_LEADERBOARD_H
#define OPTIMIZEDMINESWEEPER_LEADERBOARD_H
#include <vector>
#include <string>

struct score{
    bool recentscore;
    std::string time;
    std::string name;
    int total;

    score(std::string Time, std::string Name, bool recent);

};
class leaderboard {
    std::vector<score> scorevect;
    int WIDTH;
    int HEIGHT;
public:
    leaderboard(int w, int h);
    void leaderboardwindow();
    void addscore(std::string t, std::string n);
    void clear();
};


#endif //OPTIMIZEDMINESWEEPER_LEADERBOARD_H
