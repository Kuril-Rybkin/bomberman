//
// Created by kuril on 7.5.22.
//

#include "CEntity.h"
#include <iostream>

bool CEntity::isObstacle() {
    return obstacle;
}

void CEntity::print() {
    //turn on colour for entity icon
    attron(COLOR_PAIR(colour));

    //print entity icon
    mvprintw(coords.second, coords.first, "%c", icon); //prints the required character on coordinates

    //turn off colours to avoid disrupting next prints
    attroff(COLOR_PAIR(colour));
    refresh();
}

CEntity::CEntity(int xcoord, int ycoord, char icn, int clr, bool obs) {
    colour = clr; //index of the colour pair used to colour the entity
    icon = icn; //what character to display in place of the entity
    obstacle = obs; //if entity is collidable, i.e. is an obstacle, or not
    coords = make_pair(xcoord, ycoord); //initialise variables
}