#include <iostream>
#include "Board.h"

int main()
{
    Board Minesweeper("boards/config.cfg");
    Minesweeper.WindowFromBoard();

    //Minesweeper.Display();
   // ABOVE LINE DISPLAYS INITIAL BOARD TO CONSOLE

    /* ~~~~~~~~~~~~~~~ A NOTE TO THE PLAYER ~~~~~~~~~~~~~~~ /
    * 
    * 
    * Feeling adventurous? Does the regular, slow pace of Minesweeper bore you?
    *
    * 
    * Well, there's a hidden mechanic you can implement to vastly ramp up the pace!
    * It's called "chording", and the concept is simple.
    * 
    * If you have an open tile and want to quickly reveal all the adjacent safe tiles,
    * simply set flags around the tile until the number of adjacent flags and the
    * number on the tile are equal, and...
    * 
    * LEFT AND RIGHT CLICK IT AT THE SAME TIME!
    * 
    * Instantly, all adjacent, unmarked tiles will be popped open!
    * 
    * Give it a shot, and revolutionize your minesweeping experience!
    * 
    * 
    * -Nicholas DeSanctis
    * 
    * 
    * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

   

    return 0;
}