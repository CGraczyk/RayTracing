// progress_bar.hpp
#pragma once
#include <chrono>
#include <iostream>
#include <string>

class ProgressBar {
public:
  ProgressBar(size_t total, size_t barWidth = 50)
      : total(total), barWidth(barWidth), lastPercent(-1) {}

  void update(size_t current) {
    int percent = static_cast<int>((100.0 * current) / total);
    if (percent == lastPercent)
      return; // Skip redundant updates

    lastPercent = percent;
    size_t pos = (barWidth * current) / total;

    std::clog << "\r[8";
    for (size_t i = 0; i < barWidth; ++i) {
      if (i < pos)
        std::clog << "=";
      else if (i == pos)
        std::clog << ">";
      else
        std::clog << " ";
    }
    std::clog << "] " << percent << "%";
    std::clog.flush();
  }

  void done() {
    update(total);
    std::clog << std::endl;
  }

private:
  size_t total;
  size_t barWidth;
  int lastPercent;
};
