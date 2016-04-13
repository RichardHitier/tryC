#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>


void showWins(int i){

    WINDOW *winRight, *winLeft, *winLow;

    const int max_channels = 15;
    const int tot_width=80;
    const int higher=2*max_channels/3;

    initscr();

    winLeft  = subwin( stdscr, higher, tot_width/2, 2, 0);
    winRight = subwin( stdscr, higher, tot_width/2, 2, tot_width/2);
    winLow   = subwin( stdscr, max_channels/3,   tot_width, higher, 0);

    while(i-->0){
        mvwprintw( winLeft, 0, 0, "Hello winLeft\n");
        mvwprintw( winRight, 0, 0, "Hello winRight\n");
        mvwprintw( winLow, 0, 0, "Hello winLow\n");
        refresh();
        sleep(1);

    }

    endwin();

}

int main(int argc, char **argv){
    int i = atoi(argv[1]);
    showWins(i);
}
