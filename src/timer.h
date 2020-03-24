#ifndef TIMER_H
#define TIMER_H

class Timer
{
    private:
    //The clock time when the timer started
    int start_ticks;

    //The ticks stored when the timer was paused
    int paused_ticks;

    bool paused;
    bool started;

    public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    int get_ticks();

    bool is_started();
    bool is_paused();
};

#endif
