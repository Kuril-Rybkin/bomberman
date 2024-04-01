//
// Created by kuril on 28.5.22.
//

#ifndef BOMBERMAN_CPLAYER_H
#define BOMBERMAN_CPLAYER_H
#include "CEntity.h"
#include "helpers.h"
#include <ncurses.h>


class CPlayer {
public:
    CPlayer(int x, int y, string name, char icon, int colour, int lives);

    virtual ~CPlayer() = default;

    bool move(int input, vector<vector<CEntity *>> & map); //change the coordinates of the entity

    void print(); //print the current entity onto the map

    bool checkValid(int x, int y, vector<vector<CEntity *>> & map);

    virtual bool play(vector<vector<CEntity *>> & map, list<CBomb *> & bmbVec, char input = 'a') = 0;

    virtual bool update(vector<vector<CEntity *>> & map) = 0;

    bool loseHealth(vector<vector<CEntity *>> & map);

    virtual void addScore(int amount) = 0;

protected:
    int lives;
    int bombCountdown = 5;
    int bombRange = 3;
    int colour; //index of the colour pair used to colour the entity
    char icon;
    string name;
    pair<int, int> coords;
};

class CHuman : public CPlayer {
public:
    CHuman(int x, int y, string name, char icon, int colour, int lives, int bombs, vector<char> & controls, vector<string> & bonuses);

    ~CHuman() override = default;

    bool play(vector<vector<CEntity *>> & map, list<CBomb *> & bmbVec, char input = 'a') override;

    bool update(vector<vector<CEntity *>> & map) override;

    void getStats(int & lives, int & bombs, string & name, string & bonus, int & score);

    void addScore(int amount) override;

    pair<string, int> returnScore();

private:
    int bombs;
    int score = 0;
    vector<char> controls;
    string bonus;
    vector<string> bonuses;
    chrono::time_point<chrono::system_clock> time;
    chrono::time_point<chrono::system_clock> bombRegen;
};

class CBot : public CPlayer {
public:
    CBot(int x, int y, string name = "BOT", char icon = '*', int colour = 4, int lives = 1);

    ~CBot() override = default;

    bool play(vector<vector<CEntity *>> & map, list<CBomb *> & bmbVec, char input = 'a') override;

    bool update(vector<vector<CEntity *>> & map) override;

    void addScore(int amount) override;

private:
    chrono::time_point<chrono::system_clock> moveDelay;
    chrono::time_point<chrono::system_clock> bombDelay;
};


#endif //BOMBERMAN_CPLAYER_H
