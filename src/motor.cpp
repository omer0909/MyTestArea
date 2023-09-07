#include <iostream>
#include <string>

#define EPSILON 0.00001

double Abs(double val) {
  return val < 0 ? -val : val;
}

int main() {
  double deltaTime = 0.0001;
  double pos = 10;
  double targetPos = 50;
  double force = 2;
  double speed = 0;

  while (true) {
    double forceDir = pos < targetPos ? force : -force;
    double time = speed / force;
    double length = speed * time - 0.5 * -force * time * time;

    speed += length > Abs(targetPos - pos) ? -forceDir * deltaTime : forceDir * deltaTime;

    double newPos = pos + speed * deltaTime;
    if ((pos < targetPos) == (newPos >= targetPos) || Abs(newPos - targetPos) < EPSILON) {
      break;
    }
    pos = newPos;

    // debug
    {
      static int index;
      if (!(index % 10000))
        std::cout << std::string(pos * 2, ' ') << "*" << std::endl;
      index++;
    }
  }
  std::cout << std::endl;
}
