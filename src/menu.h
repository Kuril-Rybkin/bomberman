//
// Created by kuril on 1.6.22.
//

#ifndef BOMBERMAN_MENU_H
#define BOMBERMAN_MENU_H

#include <vector>
#include <algorithm>
#include <string>
#include <ncurses.h>

using namespace std;

int menu(vector<string> choices);

void changeString(string question, string & target);

void changeVec(vector<string> original, vector<string> & current);

#endif //BOMBERMAN_MENU_H
