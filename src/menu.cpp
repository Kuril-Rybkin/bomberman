//
// Created by kuril on 1.6.22.
//
#include "menu.h"

using namespace std;

int menu(vector<string> choices) {
    int choice = 0;
    int maxsize = choices.size();

    WINDOW * win = newwin(choices.size() + 2, 30, (LINES / 2) - maxsize, (COLS / 2) - 15); //draw 20x20 window in the centre

    keypad(win, true);
    touchwin(win);
    wrefresh(win);

    while (true) {
        //print all the options
        box(win, 120 | A_ALTCHARSET, 113 | A_ALTCHARSET);
        for (int i = 0; i < maxsize; i++) {
            //highlight the option selected
            if (i == choice) {
                wattron(win, A_REVERSE);
            }
            mvwprintw(win, i+1, 15 - choices[i].size() / 2, "%s", choices[i].c_str());
            wattroff(win, A_REVERSE);
        }

        wrefresh(win);
        auto a = wgetch(win);
        //increment choice (move down the list) if key down is pressed
        if (a == KEY_DOWN) {
            choice = (choice + 1) % maxsize;
        }
        //decrement choice (move up the list) if key up is pressed
        if (a == KEY_UP) {
            choice -= 1;
            if (choice < 0) {
                choice = maxsize - 1;
            }
        }
        //if enter is pressed record choice
        if (a == 10 || a == KEY_ENTER) {
            break;
        }
    }
    keypad(win, false);
    wclear(win);
    delwin(win);
    clear();
    refresh();
    return choice;
}

void changeString(string question, string & target) {
    clear();

    //print the question in the center of the screen:
    int ypos = LINES / 2;
    int xpos = COLS / 2;
    mvprintw(ypos, xpos - (question.size() / 2), question.c_str());
    mvprintw(ypos + 1, xpos - (target.size() / 2), target.c_str());
    curs_set(1);
    refresh();

    int a;
    a = getch();

    //edit the target string until the enter key is pressed
    while (a != 10 && a != KEY_ENTER) {
        //if a is a printable character that is not a space
        if (a > 31 && a < 127 && a != ' ') {
            target += a; //add character to the string
            if ((int)target.length() < COLS) { //if string fits into the screen, print it in the center
                clear();
                mvprintw(ypos, xpos - (question.size() / 2), question.c_str());
                mvprintw(ypos + 1, xpos - (target.size() / 2), target.c_str());
                refresh();
            }
        }
        //127 is delete key. If string is not empty and delete key is pressed, remove last character
        else if (a == 127 && !target.empty()){
            target.pop_back();
            if ((int)target.length() < COLS) { //if string fits into screen, print it in the center
                clear();
                mvprintw(ypos, xpos - (question.size() / 2), question.c_str());
                mvprintw(ypos + 1, xpos - (target.size() / 2), target.c_str());
                refresh();
            }
        }
        a = getch();
    }
    clear();
    refresh();
    curs_set(0);
}

void changeVec(vector<string> original, vector<string> & current) {
    //toggles options on/off by adding/removing them from the vector current
    keypad(stdscr, true);
    clear();

    string title = "USE ARROW KEYS AND ENTER TO MAKE SELECTION";
    mvprintw(0, (COLS / 2) - (title.length() / 2), title.c_str());
    title = "PRESS Q TO QUIT";
    mvprintw(1, (COLS / 2) - (title.length() / 2), title.c_str());
    refresh();

    size_t choice = 0;
    bool updated = false; //avoid constantly updating the vector and sclowing down the program

    while (true) {
        if (!updated) {
            updated = true;

            //print all available choices:
            for (size_t i = 0; i < original.size(); i++) {
                //highlight currently selected choice
                if (i == choice) {
                    attron(A_REVERSE);
                }

                //if option is toggled on, highlight it in green. If an option is off, it wont be present in current
                if (find(current.begin(), current.end(), original[i]) != current.end()) {
                    attron(COLOR_PAIR(3));
                    mvprintw((LINES / 2) - (original.size() / 2) + i, (COLS / 2) - (original[i].length() / 2), original[i].c_str());
                    attroff(COLOR_PAIR(3));
                } else {
                    //option is not present in current so highlight it in red
                    attron(COLOR_PAIR(2));
                    mvprintw((LINES / 2) - (original.size() / 2) + i, (COLS / 2) - (original[i].length() / 2), original[i].c_str());
                    attroff(COLOR_PAIR(2));
                }
                if (i == choice) {
                    attroff(A_REVERSE);
                }
            }
        }

        int a = getch();
        if (a == KEY_UP) { //move up the menu
            updated = false;
            if (choice == 0) {
                choice = original.size() - 1;
            }
            else {
                choice--;
            }
        }
        else if (a == KEY_DOWN) { //move down the menu
            updated = false;
            if (choice == original.size() - 1) {
                choice = 0;
            }
            else {
                choice++;
            }
        }
        else if (a == 10 || a == KEY_ENTER) {
            //toggle the option on/off
            updated = false; //list was changed, so it needs to be updated again
            //find selection in current list. If it is found, then it is turned on and needs to be turned off.
            //vice versa if it is not found.

            auto it = find(current.begin(), current.end(), original[choice]);
            if (it == current.end()) {
                //re-add option because it was turned back on
                current.push_back(original[choice]);
            }
            else {
                //remove option because it was turned off
                current.erase(it);
            }
        }
        else if (a == 'q' || a == 'Q') {
            clear();
            refresh();
            break;
        }
        refresh();
    }
    keypad(stdscr, false);
}