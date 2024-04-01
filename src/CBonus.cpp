//
// Created by kuril on 25.5.22.
//

#include "CEntity.h"


CBonus::CBonus(int x, int y, char icn) : CEntity(x, y, icn, 1, false) {
    coords = make_pair(x, y); //save coordinates in member variable
}

void CBonus::print() {
    //overload print function since bonus doesnt need colour attributes
    mvprintw(coords.second, coords.first, "B");
    refresh();
}