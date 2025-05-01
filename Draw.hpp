#ifndef DRAW
#define DRAW

#include <ncurses.h>
#include <chrono>
#include <thread>
#include <vector>
#include <deque>

#include "Car.hpp"
#include "Lift.hpp"

class Draw{

    public:

    WINDOW *board_win;
    Lift *lift;
    std::vector<Car*> *auta;
    std::deque<Car*> *kolejka;
    bool *stop;

    int y1 = 1;
    int y2 = 4;
    int x1 = 51;
    int x2 = 58;

    int yKolejka;
    int xKolejka;

    Draw(WINDOW *board_win, Lift *lift, std::vector<Car*> *auta, bool *stop, std::deque<Car*> *kolejka){
        this->board_win = board_win;
        this->lift = lift;
        this->auta = auta;
        this->stop = stop;
        this->kolejka = kolejka;
    }

    void draw(){
        while(true){
            if(*stop){
                break;
            }
            werase(board_win);
            //werase(stdscr);
            box(board_win, 0, 0);
            //autka
            for(int i = 0; i < auta->size(); i++){
                auto autko = auta->at(i);
                mvwaddch(board_win, (auta->at(i))->car_y, (auta->at(i))->car_x, (auta->at(i))->car_symbol);
            }

            //kolejka
            mvwaddch(board_win, 6, 12, 'K');
            mvwaddch(board_win, 6, 13, 'O');
            mvwaddch(board_win, 6, 14, 'L');
            mvwaddch(board_win, 6, 15, 'E');
            mvwaddch(board_win, 6, 16, 'J');
            mvwaddch(board_win, 6, 17, 'K');
            mvwaddch(board_win, 6, 18, 'A');


            yKolejka = 7;
            xKolejka = 12;
            for(int i = 0; i < kolejka->size(); i++){
                mvwaddch(board_win, yKolejka, xKolejka, (kolejka->at(i))->car_symbol);
                if(yKolejka < 14){
                    yKolejka += 1;
                }
                else{
                    yKolejka = 7;
                    xKolejka += 5;
                }
            }

            //wewnetrzny prostokat
            mvwhline(board_win,5,10,0,40);
            mvwhline(board_win,15,10,0,40);
            mvwvline(board_win,5,10,0,10);
            mvwvline(board_win,5,50,0,10);
            mvwaddch(board_win,5,10,ACS_ULCORNER);
            mvwaddch(board_win,15,10,ACS_LLCORNER);
            mvwaddch(board_win,15,50,ACS_LRCORNER);
            mvwaddch(board_win,5,50,ACS_URCORNER);
            //dolny odcinek na 1 auto
            mvwvline(board_win,15,20,0,5);
            mvwvline(board_win,15,40,0,5);
            //winda
            mvwhline(board_win,y1,x1,0,7);
            mvwhline(board_win,y2,x1,0,7);
            mvwvline(board_win,y1,x1,0,4);
            mvwvline(board_win,y1,x2,0,4);
            mvwaddch(board_win,y1,x1,ACS_ULCORNER);
            mvwaddch(board_win,y2,x1,ACS_LLCORNER);
            mvwaddch(board_win,y2,x2,ACS_LRCORNER);
            mvwaddch(board_win,y1,x2,ACS_URCORNER);
            //ruch windy
            mvwhline(board_win,lift->y,x1,0,7);
            mvwhline(board_win,lift->y+3,x1,0,7);
            mvwvline(board_win,lift->y,x1,0,4);
            mvwvline(board_win,lift->y,x2,0,4);
            mvwaddch(board_win,lift->y,x1,ACS_ULCORNER);
            mvwaddch(board_win,lift->y+3,x1,ACS_LLCORNER);
            mvwaddch(board_win,lift->y+3,x2,ACS_LRCORNER);
            mvwaddch(board_win,lift->y,x2,ACS_URCORNER);

            wrefresh(board_win);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

};

#endif