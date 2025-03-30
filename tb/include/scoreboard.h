#ifndef SCOREBOARD_H
#define SCOREBOARD_H

class Scoreboard {
  public:
    Scoreboard() : total(0), passed(0), failed(0) {}

    void reset() {
        total = 0;
        passed = 0;
        failed = 0;
    }

    void pass() {
        total++;
        passed++;
    }

    void fail() {
        total++;
        failed++;
    }

    unsigned getTotal() const { return total; }
    unsigned getPassed() const { return passed; }
    unsigned getFailed() const { return failed; }

  private:
    unsigned total;
    unsigned passed;
    unsigned failed;
};

#endif
