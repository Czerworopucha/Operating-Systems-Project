#ifndef FACTORY
#define FACTORY

#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <condition_variable>
#include <deque>

#include "Car.hpp"
#include "Lift.hpp"

class Factory{

    public:

    int wait = 0;

    std::vector<Car*> *auta;
    std::vector<std::thread*> *watki;
    std::deque<Car*> *kolejka;

    bool *liftDone;
    bool *liftGoingDown;
    bool *stop;
    bool *roadFree;

    std::condition_variable *cv;
    std::mutex *mut;
    std::condition_variable *cv2;
    std::mutex *mut2;

    Factory(std::vector<Car*> *auta, std::vector<std::thread*> *watki, bool *liftDone, bool *liftGoingDown, bool *stop, bool *roadFree, std::condition_variable *cv, std::mutex *mut, std::condition_variable *cv2, std::mutex *mut2, std::deque<Car*> *kolejka){
        this->auta = auta;
        this->watki = watki;
        this->liftDone = liftDone;
        this->liftGoingDown = liftGoingDown;
        this->stop = stop;
        this->roadFree = roadFree;
        this->cv = cv;
        this->mut = mut;
        this->cv2 = cv2;
        this->mut2 = mut2;
        this->kolejka = kolejka;
    }

    void factory(){
        while(true){
            if(*stop){
                break;
            }
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<> dist(100, 5000);
            wait = dist(rng);

            std::this_thread::sleep_for(std::chrono::milliseconds(wait));

            Car *car = new Car(liftDone, liftGoingDown, stop, roadFree, cv, mut, cv2, mut2, kolejka);
            auta->push_back(car);
            std::thread *t1 = new std::thread(&Car::drive, std::ref(car));
            watki->push_back(t1);
        }
    }

};

#endif