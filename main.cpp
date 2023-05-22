#include "PomodoroTime.h"
#include "Pomodoro.h"

int main() {
    std::unique_ptr<Pomodoro> pomodoro(new Pomodoro(2, TaskType::TEST));
    pomodoro->startTimer();
    pomodoro->setNewPomodoro(3, TaskType::WORK);
    pomodoro->startTimer();

    return 0;
}


