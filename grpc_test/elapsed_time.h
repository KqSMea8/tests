#pragma once
#include <sys/time.h>
#include <iostream>
#include <string>

class ElapsedTime {
public:
    ElapsedTime(bool print=true)
        :print_(print){
        gettimeofday(&t0_, 0);
    }

    virtual~ ElpasedTime() {
        std::thread::id this_id = std::this_thread::get_id();
        GetElapsed();
        if(print) {
            std::cout << String() << std::endl;
        }
    }

    std::string String(){
        std::stringstream ss;
        ss << "time:" << t_wait_
            << "ms, thread_id:" << this_id;

        return ss.str();
    }

    double GetElapsed(){
        gettimeofday(&t1_, 0);
        t_wait_ = double((t1_.tv_sec - t0_.tv_sec) * 1000.0 +
                           (t1_.tv_usec - t0_.tv_usec) / 1000.0);
        return t_wait_;
    }

    void add
private:
    bool print_;
    struct timeval t0_;
    struct timeval t1_;
    double t_wait_ = 0.0;
};


