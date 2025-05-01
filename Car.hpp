#ifndef CAR
#define CAR

#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>


class Car{
    //private:

    public:
        int loopLimit = 0;
        int loop = 0;
        int car_y = 0;
        int car_x = 0;
        int speed = 0;
        chtype car_symbol;
        chtype symbol;

        bool *liftDone;
        bool *liftGoingDown;
        bool *stop;
        bool inLift = 0;
        bool carDone = 0;
        bool inQueue = 0;

        bool *roadFree;
        std::condition_variable *cv;
        std::mutex *mut;

        std::condition_variable *cv2;
        std::mutex *mut2;

        std::deque<Car*> *kolejka;

        Car(bool *liftDone, bool *liftGoingDown, bool *stop, bool *roadFree, std::condition_variable *cv, std::mutex *mut, std::condition_variable *cv2, std::mutex *mut2, std::deque<Car*> *kolejka){
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<> dist(10, 50);
            speed = dist(rng);

            std::random_device rd1;
            std::mt19937 rng1(rd1());
            std::uniform_int_distribution<> dist1(65, 90);
            symbol = (char)(dist1(rng1));

            car_y = 3;
            car_x = 7;
            loopLimit = 4;
            car_symbol = symbol;

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

        void drive(){
            while(true){
                if(*stop){
                    break;
                }
                if (car_x < 53 && car_y == 3){
                    car_x++;
                }
                else if (car_x == 53 && car_y < 17){
                    car_symbol = ' ';
                    std::unique_lock<std::mutex> lock2(*mut2);
                    cv2->wait(lock2, [this] {return ((!*liftDone && !*liftGoingDown) || *stop);});
                    if(*stop){break;}
                    cv2->wait(lock2, [this] {return ((*liftDone && !*liftGoingDown) || *stop);});
                    car_y = 17;
                    car_symbol = symbol;
                }
                else if (car_x <= 53 && car_x > 7 && car_y == 17){
                    if (car_x == 41){
                        std::unique_lock<std::mutex> lock1(*mut);
                        inQueue = 1;
                        kolejka->push_back(this);
                        cv->wait(lock1, [this] {return (!inQueue || *stop);});
                        //if(*stop){break;}
                        *roadFree = 0;
                        
                    }
                    else if(car_x == 20){
                        *roadFree = 1;
                    }
                    car_x--;
                }
                else if (car_x == 7 && car_y <= 17){
                    car_y--;
                    if (car_y == 4){
                       loop++;
                       if (loop == loopLimit){
                        break;
                       }
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(speed));
            }
            car_symbol = ' ';
            carDone = 1;
        }
};

#endif