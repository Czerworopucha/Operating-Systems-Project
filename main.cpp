#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <deque>

#include "Car.hpp"
#include "Lift.hpp"
#include "Factory.hpp"
#include "Draw.hpp"
#include "Kolejka.hpp"

#define board_y 20
#define board_x 60


int main(int argc, char **argv){

    initscr();
    noecho();
    nodelay(stdscr,TRUE);
    curs_set(0);
    refresh();

    int key;
    bool liftDone = 0;
    bool liftGoingDown = 0;
    bool stop = 0;
    bool roadFree = 1;

    std::condition_variable *cv = new std::condition_variable;
    std::mutex *mut = new std::mutex;
    std::condition_variable *cv2 = new std::condition_variable;
    std::mutex *mut2 = new std::mutex;

    std::vector<Car*> auta;
    std::vector<std::thread*> watki;
    std::deque<Car*> kolejka;

    WINDOW *board_win = newwin(board_y, board_x, 0, 0);
    nodelay(board_win,TRUE);
    box(board_win, 0, 0);
    wrefresh(board_win);

    //---------------------------------------------------------
    Lift *lift = new Lift(&liftDone, &liftGoingDown, &stop, cv2, mut2);
    Factory *factory = new Factory(&auta, &watki, &liftDone, &liftGoingDown, &stop, &roadFree, cv, mut, cv2, mut2, &kolejka);
    Draw *draw = new Draw(board_win, lift, &auta, &stop, &kolejka);
    Kolejka *kolejkaHandler = new Kolejka(&stop, &roadFree, cv, mut, &kolejka);

    std::thread tFactory(&Factory::factory, std::ref(factory));
    std::thread tDraw(&Draw::draw, std::ref(draw));
    std::thread tLift(&Lift::lift, std::ref(lift));
    std::thread tKolejka(&Kolejka::kolejkowanie, std::ref(kolejkaHandler));

    /*
    Car *car = new Car(&liftDone, &liftGoingDown, &stop, &roadFree);
    auta.push_back(car);
    std::thread *t1 = new std::thread(&Car::drive, std::ref(car));
    watki.push_back(t1);
    */

    while(true){

        key = getch();
        if (key == ' '){
            stop = 1;
            cv->notify_all();
            cv2->notify_all();
            break;
        }

        for(int i = 0; i < auta.size(); i++){
            if((auta.at(i))->carDone){
                delete (auta.at(i));
                auta.erase(auta.begin() + i);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for(int i = 0; i < watki.size(); i++){
        if (watki[i]->joinable()){
            watki[i]->join();
        }
    }
    if (tLift.joinable()){
        tLift.join();
        delete lift;
    }
    if (tFactory.joinable()){
        tFactory.join();
        delete factory;
    }
    if (tDraw.joinable()){
        tDraw.join();
        delete draw;
    }
    if (tKolejka.joinable()){
        tKolejka.join();
        delete kolejkaHandler;
    }
    watki.clear();
    endwin();

    return 0;
}