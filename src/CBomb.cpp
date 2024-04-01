//
// Created by kuril on 8.5.22.
//

#include "CEntity.h"
#include "CPlayer.h"
#define BOMB 6

CBomb::CBomb(int x, int y, int number, int range, CPlayer * src) : CEntity(x, y, (char)(number + 48), BOMB, true){
    //save parameters into member variables:
    coords.first = x;
    coords.second = y;
    this->number = number;
    time = chrono::system_clock::now(); //set time to current system time
    this->range = range;
    this->src = src;
}

pair<int, int> CBomb::getCoords() {
    return coords;
}

void CBomb::print() {
    attron(COLOR_PAIR(colour)); //turn on text colour

    mvprintw(coords.second, coords.first, "%d", number); //prints the required character on coordinates

    attroff(COLOR_PAIR(colour)); //turn off text colour so other colours can be displayed properly
    refresh(); //refresh screen to display character
}

bool CBomb::countdown(vector<vector<CEntity *>> &map, list<CExplosion *> &exps) {
    //get current system time
    auto newtime = chrono::system_clock::now();
    //calculate difference in time between last time bomb counted down and now
    auto timediff = chrono::duration_cast<chrono::milliseconds>(newtime - time);
    //re-print bomb in case it was overshadowed by another printed entity
    print();

    //if 1 or more seconds have passed
    if (timediff >= chrono::milliseconds(1000)) {
        //decrement bomb timer
        number -= 1;
        //print updated timer on the screen
        print();
        //reset last countdown time
        time = newtime;
    }

    //bomb has finished counting down
    if (number <= 0) {
        //print empty space on bomb's place
        mvprintw(coords.second, coords.first, " ");
        //place explosion on bomb's place
        CExplosion * temp = new CExplosion(coords.first, coords.second, src);
        //push back explosion onto explosions vector for easy memory management
        exps.push_back(temp);
        //place explosion onto map coordinates
        map[coords.first][coords.second] = temp;
        //print placed explosion
        map[coords.first][coords.second]->print();

        //variables to control in which direction explosion should move
        bool xP = false; //x positive - right
        bool xM = false; // x minus - left
        bool yP = false; // y positive - up
        bool yM = false; // y minus - down

        //add explosions in all 4 directions in a range
        for (int i = 1; i <= range; i++) {
            //if the slot is free on coordinates and control is not set
            if (!xP && map[coords.first + i][coords.second] == nullptr) { //i blocks away on x axis
                //create new explosion entity
                temp = new CExplosion(coords.first + i, coords.second, src);
                //place it on map
                map[coords.first + i][coords.second] = temp;
                //print it
                temp->print();
                //push for easier memory management
                exps.push_back(temp);
            }
            //if there is a destroyable object in the path of the explosion
            else if (!xP && dynamic_cast<CObject *>(map[coords.first + i][coords.second])) {
                //destroy the object in the path
                ((CObject *)map[coords.first + i][coords.second])->destroy(map, src);
                //mark this direction as unusable since explosions cant go through walls
                xP = true;
            }
            //else it is a completely solid wall
            else {
                //mark direction as unuseable since explosions cant go through walls
                xP = true;
            }

            //if the slot is free on coordinates and control is not set
            if (!xM && map[coords.first - i][coords.second] == nullptr) {
                //create new explosion entity
                temp = new CExplosion(coords.first - i, coords.second, src);
                //place it on map
                map[coords.first - i][coords.second] = temp;
                //print it
                temp->print();
                //push for easier memory management
                exps.push_back(temp);
            }
            else if (!xM && dynamic_cast<CObject *>(map[coords.first - i][coords.second])) {
                //destroy the object in the path
                ((CObject *)map[coords.first - i][coords.second])->destroy(map, src);
                //mark this direction as unusable since explosions cant go through walls
                xM = true;
            }
            else {
                //mark this direction as unusable since explosions cant go through walls
                xM = true;
            };

            //DO THE SAME HERE BUT FOR UPWARDS DIRECTION:
            if (!yP && map[coords.first][coords.second + i] == nullptr) {
                temp = new CExplosion(coords.first, coords.second + i, src);
                map[coords.first][coords.second + i] = temp;
                temp->print();
                exps.push_back(temp);
            }
            else if (!yP && dynamic_cast<CObject *>(map[coords.first][coords.second + i])) {
                ((CObject *)map[coords.first][coords.second + i])->destroy(map, src);
                yP = true;
            }
            else {
                yP = true;
            }

            //DO THE SAME HERE BUT FOR DOWNWARDS DIRECTION:
            if (!yM && map[coords.first][coords.second - i] == nullptr) {
                temp = new CExplosion(coords.first, coords.second - i, src);
                map[coords.first][coords.second - i] = temp;
                temp->print();
                exps.push_back(temp);
            }
            else if (!yM && dynamic_cast<CObject *>(map[coords.first][coords.second - i])) {
                ((CObject *)map[coords.first][coords.second - i])->destroy(map, src);
                yM = true;
            }
            else {
                yM = true;
            }
        }
        return false;
    }
    return true;
}