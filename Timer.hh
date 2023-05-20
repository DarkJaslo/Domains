#ifndef Timer_hh
#define Timer_hh

#include <chrono>

class Timer{

public:
  Timer(std::string s, double* accumulator, bool pr = true)
  {
    print = pr;
    acc = accumulator;
    whatAmI = s;
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
    Stop();
  }

  void Stop()
  {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
    auto duration = end-start;
    double ms = duration*0.001;
    if(acc != nullptr) {
      (*acc) += ms;
    }

    if(print)std::cerr << "Time of " << whatAmI << ": " << ms << "ms" << std::endl;
  }
private:
  bool print;
  std::string whatAmI;
  double* acc = nullptr;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};

#endif