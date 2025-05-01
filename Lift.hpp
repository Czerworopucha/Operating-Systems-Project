#ifndef LIFT
#define LIFT

#include <thread>
#include <ncurses.h>


class Lift{

    public:

    int y = 1;
    int x1 = 51;
    int x2 = 58;

    bool *liftDone;
    bool *liftGoingDown;
    bool *stop;

    std::condition_variable *cv2;
    std::mutex *mut2;

    Lift(bool *liftDone, bool *liftGoingDown, bool *stop, std::condition_variable *cv2, std::mutex *mut2){
        this->liftDone = liftDone;
        this->liftGoingDown = liftGoingDown;
        this->stop = stop;
        this->cv2 = cv2;
        this->mut2 = mut2;
    }

    void lift(){
        while(true){
            if(*stop){
                break;
            }
            *liftDone = 0;
            cv2->notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            *liftGoingDown = 1;
            for (y; y < 15; y++){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                y++;
            }
            *liftDone = 1;
            *liftGoingDown = 0;
            cv2->notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            y = 1;
        }
    }
};

#endif