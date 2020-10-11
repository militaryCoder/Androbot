#include <chrono>

template <typename Clock, typename Rep = long long int>
class Timer
{
public:
    Timer() = default;

    void start();

    auto getElapsedTime()->std::chrono::duration<Rep>;

    auto reset()->std::chrono::duration<Rep>;

private:
    std::chrono::time_point<Clock> m_start;
};

template <typename Clock, typename Rep>
void Timer<Clock, Rep>::start()
{
    m_start = Clock::now();
}

template <typename Clock, typename Rep>
auto Timer<Clock, Rep>::getElapsedTime() -> std::chrono::duration<Rep>
{
    using namespace std::chrono;

    return duration_cast<duration<Rep>>(Clock::now().time_since_epoch() - m_start.time_since_epoch());
}

template <typename Clock, typename Rep>
auto Timer<Clock, Rep>::reset() -> std::chrono::duration<Rep>
{
    using namespace std::chrono;

    const duration<Rep> elapsed = getElapsedTime();
    m_start = Clock::now();

    return elapsed;
}