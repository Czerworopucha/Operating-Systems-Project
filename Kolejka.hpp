#ifndef KOLEJKA
#define KOLEJKA

#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <condition_variable>
#include <deque>

#include "Car.hpp"

class Kolejka{
    public:

    bool *stop;
    bool *roadFree;
    std::condition_variable *cv;
    std::mutex *mut;
    std::deque<Car*> *kolejka;

    Kolejka(bool *stop, bool *roadFree, std::condition_variable *cv, std::mutex *mut, std::deque<Car*> *kolejka){
        this->stop = stop;
        this->roadFree = roadFree;
        this->cv = cv;
        this->mut = mut;
        this->kolejka = kolejka;
    }

    void kolejkowanie(){
        while(true){
            if(*stop){
                break;
            }

            if(*roadFree){
                std::unique_lock<std::mutex> lock(*mut);
                if(!(kolejka->empty())){
                    (kolejka->front())->inQueue = 0;
                    kolejka->pop_front();
                }
                cv->notify_all();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};
#endif