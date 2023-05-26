#pragma once

#include <string>

enum class TimerState {
    Pomodoro,
    ShortBreak,
    LongBreak
};


class Timer {
private:
    int minutes;
    int seconds;
    TimerState state;
    bool is_running;

    void decrement();


public:

    Timer();

    Timer(int minutes, int seconds, TimerState state);

    void update();

    std::string get_time();

    void start();

    bool running();

    void set_time(int minutes, int seconds, TimerState state);

    std::string get_state();
};

Timer::Timer() {}

Timer::Timer(int minutes, int seconds, TimerState state) {
    this->minutes = minutes;
    this->seconds = seconds;
    this->state = state;
    is_running = false;
}

void Timer::decrement() {
    if (is_running) {
        if (seconds == 0) {
            minutes--;
            seconds = 59;
        } else {
            seconds--;
        }
    }
}

void Timer::update() {
    if (minutes == 0 && seconds == 0) {
        is_running = false;
    } else {
        decrement();
    }
}

std::string Timer::get_time() {
    if (is_running) {
        return (minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes)) +
               ":" +
               (seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds));
    } else {
        return "00:00";
    }
}

void Timer::start() {
    is_running = true;
}

bool Timer::running() {
    return is_running;
}

void Timer::set_time(int minutes, int seconds, TimerState state) 
{
    this->minutes = minutes;
    this->seconds = seconds;
    this->state = state;
    is_running = false;
 
}

std::string Timer::get_state()
{
    if (is_running) {
        
        if (state == TimerState::Pomodoro) {
            return "Pomodoro";
        } else if (state == TimerState::ShortBreak) {
            return "Short Break";
        } else {
            return "Long Break";
        }

    } else {
        return "Not running";
    }
}