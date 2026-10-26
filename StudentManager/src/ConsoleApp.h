#pragma once
#include "StudentStore.h"
class ConsoleApp {
public:
    int run();
private:
    StudentStore store_;
    bool dirty_ = false;
    void list(bool ranked) const;
    void edit(bool adding);
    void statistics() const;
    bool save();
};
