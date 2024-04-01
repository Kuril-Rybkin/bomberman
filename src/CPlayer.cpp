//
// Created by kuril on 28.5.22.
//

#include "CPlayer.h"


CPlayer::CPlayer(int x, int y, string name, char icon, int colour, int lives) {
    coords = make_pair(x, y);
    this->icon = icon;
    this->colour = colour;
    this->lives = lives;
    this->name = name;
}

void CPlayer::print() {
    //turn on colours
    attron(COLOR_PAIR(colour));

    //print player icon
    mvprintw(coords.second, coords.first, "%c", icon); //prints the required character on coordinates

    //turn off colours
    attroff(COLOR_PAIR(colour));
    refresh();
}

bool CPlayer::checkValid(int x, int y, vector<vector<CEntity *>> & map) {
    //checks if passed in coordinates are a valid place to move to
    //is x, y, out of range of matrix and is it not a nullptr that is also an obstacle
    if (x < 0 || y < 0 || (map[x][y] != nullptr && map[x][y]->isObstacle())) {
        return false; //cannot move to x, y
    }
    else {
        return true; //possible to move to x, y
    }
}

bool CPlayer::move(int input, vector<vector<CEntity *>> & map) {
    //input is a mod-4 clockwise direction where:
    //0=north, 1=east, 2=south, 3=west
    switch(input) {
        case 0:
            //try to validate coordinates:
            if (!checkValid(coords.first, coords.second - 1, map)) {
                return false;
            }
            else {
                //print blank space on old position
                mvprintw(coords.second, coords.first, "%c", ' ');
                //change coordinates
                coords.second -= 1;
                //re-print self
                print();
                return true;
            }

        case 1:
            //try to validate coordinates:
            if (!checkValid(coords.first + 1, coords.second, map)) {
                return false;
            }
            else {
                //print blank space on old position
                mvprintw(coords.second, coords.first, "%c", ' ');
                coords.first += 1;
                //re-print self
                print();
                return true;
            }

        case 2:
            //try to validate coordinates:
            if (!checkValid(coords.first, coords.second + 1, map)) {
                return false;
            }
            else {
                //print blank space on old position
                mvprintw(coords.second, coords.first, "%c", ' ');
                coords.second += 1;
                //re-print self
                print();
                return true;
            }

        case 3:
            //try to validate coordinates:
            if (!checkValid(coords.first - 1, coords.second, map)) {
                return false;
            }
            else {
                //print blank space on old position
                mvprintw(coords.second, coords.first, "%c", ' ');
                coords.first -= 1;
                //re-print self
                print();
                return true;
            }

        default:
            return false;
    }
}

bool CPlayer::loseHealth(vector<vector<CEntity *>> &map) {
    lives -= 1;
    if (lives <= 0) {
        return false;
    }

    while (true) { //try to find a free place to respawn
        //generate random coordinates within the range of the matrix
        int x = random(0, map.size() - 1);
        int y = random(0, map[0].size() - 1);

        //check if the generated coordinates are valid:
        if (map[x][y] == nullptr) {
            //assign new coordinates:
            coords.first = x;
            coords.second = y;
            print();
            //stop generating coordinates
            break;
        }
    }
    return true;
}