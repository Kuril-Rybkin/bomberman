//
// Created by kuril on 7.5.22.
//
#include <utility>
#include <string>
#include <stack>
#include <vector>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <list>
#include "helpers.h"
using namespace std;

#ifndef BOMBERMAN_CENTITY_H
#define BOMBERMAN_CENTITY_H


class CPlayer;

class CEntity {
public:
    CEntity(int xcoord, int ycoord, char icn, int clr, bool obs); //initialise entity

    virtual ~CEntity() = default;

    virtual void print(); //print the current entity onto the map

    bool isObstacle(); //return if the entity is an obstacle or not

protected:
    int colour; //index of the colour pair used to colour the entity
    char icon; //contains the character that is supposed to be printed
    bool obstacle; //easy check if collisions for this entity should be followed
    pair<int, int> coords; //pair of x, y
};

class CExplosion : public CEntity {
public:
    CExplosion(int x, int y, CPlayer * src); //initialize explosion

    ~CExplosion() override = default;

    void addScore(int amount); //will add score to the source of the explosion

    bool countdown(); //expires the explosion

    pair<int, int> getCoords();
private:
    int duration; //stores how long the bomb should stay alive in seconds
    bool blink;
    CPlayer * src; //stores the pointer to the player who planted the explosion, to assign points to him
    chrono::time_point<chrono::system_clock> time; //stores the time of the last update
};

class CBomb : public CEntity {
public:
    CBomb(int x, int y, int number, int range, CPlayer * src); //instantiates bomb on coordinates

    ~CBomb() override = default;

    bool countdown(vector<vector<CEntity *>> & map, list<CExplosion *> & exps); //checks time and counts down to explosion

    void print() override;

    pair<int, int> getCoords(); //returns bomb coordinates <x, y>

private:
    int number; //
    int range;
    CPlayer * src;
    chrono::time_point<chrono::system_clock> time;
};

class CObject : public CEntity {
public:
    CObject(int x, int y, char icn, int clr, bool destroyable, int lvs);

    ~CObject() override = default;

    bool isDestroyable();

    void destroy(vector <vector <CEntity *>> & map, CPlayer * src); //called from within the explosion entity -- subtracts lives as necessary and cleans up after itself

private:
    bool destroyable; //if object can be destroyed or is indestructible

    int lives; //determines how many times object needs to be hit to be destroyed
};

class CBonus : public CEntity {
public:
    CBonus(int x, int y, char icn);

    void print() override;

    ~CBonus() override = default;

private:
    pair<int, int> coords;
};

#endif //BOMBERMAN_CENTITY_H
