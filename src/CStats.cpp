//
// Created by kuril on 25.5.22.
//

#include "CStats.h"
#include "CPlayer.h"


CStats::CStats(int dimX, int dimY, int x, int y, int clr, CHuman * plyr) {
    win = newwin(dimY, dimX, y, x);
    player = plyr;
    colour = clr;
    //update player statistics:
    player->getStats(lives, bombs, name, bonus, score);

    //create new window with border:
    touchwin(win);
    wattron(win, COLOR_PAIR(colour));
    box(win, 120 | A_ALTCHARSET, 113 | A_ALTCHARSET);

    //print player statistics and refresh window:
    if (name.length() > 22) { //if name is too big to display
        name = name.substr(0, 19); //shorten displayed name (not the true name)
        name += "..."; //add elipses to the end
    }
    mvwprintw(win, 1, 1, "%s", name.c_str());
    mvwprintw(win, 2, 1, "SCORE: %d", score);
    mvwprintw(win, 3, 1, "LIVES: %d", lives);
    mvwprintw(win, 4, 1, "BOMBS: %d", bombs);
    wrefresh(win);
    wattroff(win, COLOR_PAIR(clr));
}

CStats::~CStats() {
    //clear the window
    wclear(win);
    delwin(win);
}

void CStats::update() {
    //initialise new variables to compare with old variables
    //newLives is set to -1 so the scoreboard will update when the player has 0 lives left
    int newLives = -1, newBombs = 0, newScore = 0;
    string newName, newBonus;
    //update statistics:
    player->getStats(newLives, newBombs, newName, newBonus, newScore);

    //see if there are any changes to avoid constantly re-printing window
    if (newLives != lives || newBombs != bombs || newBonus != bonus || newScore != score) {
        //update member variables
        lives = newLives;
        bombs = newBombs;
        name = newName;
        bonus = newBonus;
        score = newScore;

        //reprint window
        wclear(win);
        wattron(win, COLOR_PAIR(colour));
        box(win, 120 | A_ALTCHARSET, 113 | A_ALTCHARSET);

        if (name.length() > 22) { //if name is too big to display
            name = name.substr(0, 19); //shorten displayed name (not the true name)
            name += "..."; //add elipses to the end
        }
        mvwprintw(win, 1, 1, "%s", name.c_str());
        mvwprintw(win, 2, 1, "SCORE: %d", score);
        mvwprintw(win, 3, 1, "LIVES: %d", lives);
        mvwprintw(win, 4, 1, "BOMBS: %d", bombs);

        //check if there is a bonus that needs to be printed
        if (!bonus.empty()) {
            wattron(win, A_BLINK);
            mvwprintw(win, 5, 1, "BONUS: %s", bonus.c_str());
            wattroff(win, A_BLINK);
        }
        wrefresh(win);
        wattroff(win, COLOR_PAIR(colour));
    }
}

void CStats::print() {
    //re-print CStats without updating them. Used when screen was cleared and stats need to be printed
    wattron(win, COLOR_PAIR(colour));
    box(win, 120 | A_ALTCHARSET, 113 | A_ALTCHARSET);
    if (name.length() > 22) { //if name is too big to display
        name = name.substr(0, 19); //shorten displayed name (not the true name)
        name += "..."; //add elipses to the end
    }
    mvwprintw(win, 1, 1, "%s", name.c_str());
    mvwprintw(win, 2, 1, "SCORE: %d", score);
    mvwprintw(win, 3, 1, "LIVES: %d", lives);
    mvwprintw(win, 4, 1, "BOMBS: %d", bombs);

    //check if there is a bonus that needs to be printed
    if (!bonus.empty()) {
        wattron(win, A_BLINK);
        mvwprintw(win, 5, 1, "BONUS: %s", bonus.c_str());
        wattroff(win, A_BLINK);
    }
    wrefresh(win);
    wattroff(win, COLOR_PAIR(colour));
}

int CStats::getScore() {
    return score;
}