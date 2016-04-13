#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>


void showWins(){

    WINDOW *winRight, *winLeft, *winLow;

    const int max_channels = 15;
    const int tot_width=80;
    const int higher=2*max_channels/3;

    int n, p;

    initscr();

    winLeft  = subwin( stdscr, higher, tot_width/2, 2, 0);
    winRight = subwin( stdscr, higher, tot_width/2, 2, tot_width/2);
    winLow   = subwin( stdscr, max_channels/3,   tot_width, higher+2, 0);



    while(1){
        mvwprintw( winLeft, 1, 1, "Hello winLeft\n");
        mvwprintw( winRight, 1, 1, "Hello winRight\n");
        mvwprintw( winLow, 1, 1, "Hello winLow\n");
        box(winLeft, ACS_VLINE, ACS_HLINE);
        box(winRight, ACS_VLINE, ACS_HLINE);
        box(winLow, ACS_VLINE, ACS_HLINE);

        int yoff=2, xoff=1;

        for( n=0; n<4; n++){
            for( p=0; p<4; p++){
                mvwprintw( winLeft, yoff+n, xoff+p, ".");
            }
        }


        refresh();
        sleep(1);
    }

    endwin();

}

int main(int argc, char **argv){
    showWins();
}
