#include "Pomodoro.h"
#include "PomodoroTime.h"

//void task(Pomodoro& pomodoro);

void clearConsole() {
    //cout << "\033c";
    system("cls");
}

Pomodoro::Pomodoro(int time, TaskType type) 
{
    this->time = std::chrono::milliseconds(time * 60000);
    this->type = type;
    this->pomodoroTime = PomodoroTime();
}
const std::chrono::milliseconds Pomodoro::getTime() {
    return time;
}
const TaskType Pomodoro::getType() {
    return type;
}

const std::string Pomodoro::typePomodoro()
{
    return type == TaskType::WORK ? "Work" : type == TaskType::REST ? "Rest" : "Test";
}

void Pomodoro::startTimer() {
    this->pomodoroTime.setTime(this->time);
    auto fut = std::async(std::launch::async,  [this] {this->task();});
}

void Pomodoro::setNewPomodoro(int time, TaskType type) {
    this->time = std::chrono::milliseconds(time * 60000);
    this->type = type;
}

void Pomodoro::task() {
    auto time = this->getTime();
    auto now = std::chrono::high_resolution_clock::now();
    //PomodoroTime t = PomodoroTime();
    std::chrono::milliseconds interval(1000);
    
    while (std::chrono::high_resolution_clock::now() - now < time) {
        //clearConsole();
        auto temp = time - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);
        this->pomodoroTime.setTime(temp);
        std::cout << "Remain: " << this->typePomodoro() << " " << this->pomodoroTime.getTime() << " sec." << std::endl;
        std::this_thread::sleep_for(interval);
    }

    std::cout << "Time is up." << std::endl;    
}

std::string Pomodoro::getPomodoroTime()
{
    return this->pomodoroTime.getTime();
}