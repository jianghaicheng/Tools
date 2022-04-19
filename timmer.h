#include <chrono>
#include <ctime>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>

#define TimerCreator(fileName)                                                 \
  std::make_shared<Timer>(__FILE__, __func__, __LINE__, fileName);
using std::chrono::milliseconds;
using std::chrono::system_clock;
class Timer {
private:
  std::string fileName_;
  std::string file_, func_;
  int line_;
  system_clock::time_point start_, end_;

public:
  Timer(std::string file, std::string func, int line, std::string fileName)
      : file_(file), func_(func), line_(line) {
    int id    = syscall(SYS_gettid);
    fileName_ = fileName + "@tid:" + std::to_string(id);
    auto pos  = file_.rfind('/');
    file_     = file_.substr(pos + 1, file_.size());
    start_    = system_clock::now();
  }
  void makeDir(std::string dir) {
    static bool firstTime = false;
    if (firstTime) {
      return;
    }
    firstTime = true;

    struct stat st = {0};
    if (stat(dir.c_str(), &st) == -1) {
      mkdir(dir.c_str(), 0700);
    } else {
      throw popart::error(
          "Timer's log dir exists, please rename it in case of data loss.");
    }
  };
  void end(std::string message) {
    end_               = system_clock::now();
    time_t time        = std::chrono::system_clock::to_time_t(end_);
    char *current_time = ctime(&time);
    std::string head =
        file_ + ":" + func_ + ":" + std::to_string(line_) + " | ";

    milliseconds timeInterval =
        std::chrono::duration_cast<milliseconds>(end_ - start_);
    std::string cost = std::to_string(timeInterval.count() / 1000.0) + "s ";

    std::ofstream out;
    std::string dir = "./timer_logs";
    makeDir(dir);
    out.open(dir + "/" + fileName_, std::ios::app);
    out << head << message << " totalTime: " << cost
        << +" @ " + std::string(current_time);
    out.close();
  }
};
