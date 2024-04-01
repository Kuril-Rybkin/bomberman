//
// Created by kuril on 8.5.22.
//
#define EXPLOSION 7
#include "CEntity.h"
#include "CPlayer.h"

CExplosion::CExplosion(int x, int y, CPlayer * src) : CEntity(x, y, '#', EXPLOSION, true) {
    //save parameters as member variables and initialise other member variables:
    coords.first = x;
    coords.second = y;
    time = chrono::system_clock::now();
    blink = true;
    duration = 3;
    //src is the pointer to the source of the explosion, i.e. who planted the bomb.
    //this is so that score can be correctly assigned
    this->src = src;
}

void CExplosion::addScore(int amount) {
    //since bots dont have a score, they pass in a nullptr
    //this check is necessary so score isnt added to bots.
    if (src) {
        src->addScore(amount);
    }
}

bool CExplosion::countdown() {
    //compare the differences between current time and last time explosion counted down
    //this acts as a timer for the duration of the explosion
    auto newtime = chrono::system_clock::now();
    auto timediff = chrono::duration_cast<chrono::milliseconds>(newtime - time);

    if (timediff >= chrono::milliseconds(500)) {
        //every half second print either space or # to simulate blinking effect
        if (blink) {
            //print '#' here
            print();
            //set blink to false so that a ' ' is printed next time
            blink = false;
        }
        else {
            //print ' ' here
            mvprintw(coords.second, coords.first, " ");
            refresh();
            //space is printed every other time, with a delay of 0.5 seconds between each time
            //every time a space is printed a second has passed, so count down timer
            duration -= 1;
            blink = true;
        }
       time = newtime; //reset time
    }
    if (duration <= 0) { //timer has run out
        mvprintw(coords.second, coords.first, " "); //explosion has expired
        return false; //so that explosion can be properly deleted
    }
    return true;
}

pair<int, int> CExplosion::getCoords() {
    return coords;
}