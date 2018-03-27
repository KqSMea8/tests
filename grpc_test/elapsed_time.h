#pragma once
#include <sys/time.h>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

class ElapsedTime {
public:
    ElapsedTime(bool print=true)
        :print_(print){
        gettimeofday(&t0_, 0);
    }

    virtual~ ElapsedTime() {
        GetElapsed();
        if(print_) {
            std::cout << String() << std::endl;
        }
    }

    std::string String(){
        std::thread::id this_id = std::this_thread::get_id();

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

private:
    bool print_;
    struct timeval t0_;
    struct timeval t1_;
    double t_wait_ = 0.0;
};
