#include "stdafx.h"
class Timer {
    bool clear = false;

public: 
    void setInterval(auto function, int interval);
    void stop();
};