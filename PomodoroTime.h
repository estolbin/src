#pragma once

#include <chrono>
#include <string>

class PomodoroTime 
{
private:
    int minutes;
    int seconds;
public:
    void setTime(std::chrono::milliseconds time);
    const std::string getTime();
};