//
// Created by kuril on 25.5.22.
//

#ifndef BOMBERMAN_CSTATS_H
#define BOMBERMAN_CSTATS_H

#include "CPlayer.h"
#include <ncurses.h>

class CStats {
public:
    CStats(int dimX, int dimY, int x, int y, int clr, CHuman * plyr);

    void update();

    int getScore();

    void print();

    ~CStats();


private:
    WINDOW * win;
    CHuman * player;
    int colour;
    int lives;
    int bombs;
    int score;
    string name;
    string bonus;
};


#endif //BOMBERMAN_CSTATS_H
