#pragma once
#include <chrono>
#include <string>
#include <future>
#include <iostream>
#include "PomodoroTime.h"

enum class TaskType {
    WORK,
    REST,
    TEST
};

class Pomodoro {
private:
    std::chrono::milliseconds time;
    TaskType type;
    PomodoroTime pomodoroTime;
public:
    Pomodoro(int time, TaskType type);
    const std::chrono::milliseconds getTime();
    const TaskType getType();
    const std::string typePomodoro();
    void startTimer() ;
    ~Pomodoro() {}
    void setNewPomodoro(int time, TaskType type);
    void task();
    std::string getPomodoroTime();
};