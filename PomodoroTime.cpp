#include "PomodoroTime.h"

void PomodoroTime::setTime(std::chrono::milliseconds time)
{
    auto sec_temp = std::chrono::duration_cast<std::chrono::seconds>(time);
    this->minutes = sec_temp.count() / 60;
    this->seconds = sec_temp.count() % 60;
}

const std::string PomodoroTime::getTime()
{
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}