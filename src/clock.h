#include <chrono>

class Clock {
    private:
        std::chrono::high_resolution_clock::time_point start, end;
        bool stopped = false;
    public:
        Clock();
        void stop();
        double get_ms();
};