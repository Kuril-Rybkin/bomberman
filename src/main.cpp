#include <iostream>
#include <ncurses.h>
#include "menu.h"
#include "game.h"
#include <vector>

//Define used colour pairs here:

using namespace std;

int main()
{
    initscr();

    start_color(); //enable use of colour
    use_default_colors(); //stop ncurses from colouring backgrounds automatically
    noecho(); //stop letter inputs from appearing on screen
    curs_set(0); //set cursor to invisible
    notimeout(stdscr, true); //dont wait for key combinations

    //initialize foreground and background pairs to create solid blocks of colour:
    init_pair(1, COLOR_BLUE , COLOR_BLUE); //WALL
    init_pair(2, COLOR_RED, -1); //PLAYER1
    init_pair(3, COLOR_GREEN, -1); //PLAYER2
    init_pair(4, COLOR_MAGENTA, -1); //BOT
    init_pair(5, COLOR_CYAN, -1); //OBSTACLE
    init_pair(6, COLOR_RED, COLOR_YELLOW); //BOMB
    init_pair(7, COLOR_YELLOW, -1); //EXPLOSION

    string fileName = "./examples/mapfile";

    string p1Name = "Player1";
    vector<char> controls0 = {'w', 'd', 's', 'a', 'e'}; //controls preset for player1

    string p2Name = "Player2";
    vector<char> controls1 = {'i', 'l', 'k', 'j', 'o'}; //controls preset for player2

    vector<string> bonuses = {"+ 3 bombs", "+ 1 life", "Faster countdown", "Larger range"}; //list of obtainable bonuses

    //display menu with following choices:
    int choice = menu({"Singleplayer", "Co-op", "Settings", "Exit"}); //menu will draw menu and wait user input, then return it here as choice
    clear(); //clear menu

    while (choice != 3) {
        if (choice == 0 || choice == 1) { //choice is singleplayer or co-op
            startGame(choice, fileName, controls0, controls1, bonuses, p1Name, p2Name);
        }
        else if (choice == 2) { //choice is settings
            choice = menu({"Change map", "Configure bonuses", "Change Player 1 name", "Change Player 2 name", "Back"});
            if (choice == 0) {
                //change path to mapfile which is to be loaded
                changeString(string("Enter file name and path (No spaces allowed):"), fileName);
            }
            else if (choice == 1) {
                //configure bonuses on/off
                changeVec({"+ 3 bombs", "+ 1 life", "Faster countdown", "Larger range"}, bonuses);
            }
            else if (choice == 2) {
                //change player 1 name
                changeString(string("Enter Player 1 name (No spaces allowed):"), p1Name);
            }
            else if (choice == 3) {
                //change player 2 name
                changeString(string("Enter Player 2 name (No spaces allowed):"), p2Name);
            }
        }

        //display menu with following choices:
        choice = menu({"Singleplayer", "Co-op", "Settings", "Exit"});
    }

    endwin(); //end window
    return 0;
}
