//
// Created by kuril on 8.5.22.
//

#include "CPlayer.h"


CBot::CBot(int x, int y, string name, char icon, int colour, int lives) : CPlayer(x, y, name, icon, colour, lives) {
    moveDelay = chrono::system_clock::now(); //save current time in member variable for movement delay
    bombDelay = chrono::system_clock::now(); //save current time in member variable for bomb placement delay
}

bool CBot::play(vector<vector<CEntity *>> & map, list<CBomb *> & bmbVec, char input) {
    auto timeDiff = chrono::system_clock::now(); //get current time

    //check if the difference in time between last move and now is greater or equal to one second
    //bot makes a decision every second
    if (chrono::duration_cast<chrono::seconds>(timeDiff - moveDelay) >= chrono::seconds(1)) {
        input = random(1, 100);
        if (input <= 20) { //first element in controls vector is to move up
            move(0, map); //move upwards in map
            moveDelay = chrono::system_clock::now();
        }
        else if (input <= 40) { //second element in controls vector is to move right
            move(1, map); //move right in map
            moveDelay = chrono::system_clock::now();
        }
        else if (input <= 60) { //third element in controls vector is to move down
            move(2, map); //move down in map;
            moveDelay = chrono::system_clock::now();
        }
        else if (input <= 80) { //fourth element in controls vector is to move left
            move(3, map); // move left in map
            moveDelay = chrono::system_clock::now();
        }
        else if (input <= 90) { //simply do nothing here
            moveDelay = chrono::system_clock::now();
        }
        else if (input <= 100) { //plant a bomb
            //check if last bomb was planted no longer than 4 seconds ago and no bombs on the current cell bot is in
            if (chrono::duration_cast<chrono::seconds>(timeDiff - bombDelay) >= chrono::seconds(4) && map[coords.first][coords.second] == nullptr) {
                //create new CBomb instance
                CBomb *playerBomb = new CBomb(coords.first, coords.second, bombCountdown, bombRange, nullptr);
                //place it on the map
                map[coords.first][coords.second] = playerBomb;
                //push back the bomb to bomb vector for easier memory management
                bmbVec.push_back(playerBomb);
                playerBomb->print();
                //reset bomb delay so robot doesnt lay bombs too fast
                bombDelay = chrono::system_clock::now();
            }
        }
    }
    return true;
}

bool CBot::update(vector<vector<CEntity *>> & map) {
    if (dynamic_cast<CExplosion *>(map[coords.first][coords.second]) != nullptr) {
        ((CExplosion *)map[coords.first][coords.second])->addScore(500);
        if (!loseHealth(map)) {
            return false;
        }
    }
    print();
    refresh();
    return true;
}

void CBot::addScore(int amount) {
    amount -= 1;
}