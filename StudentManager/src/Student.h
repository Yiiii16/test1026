#pragma once
#include <string>
struct Student {
    std::string id;
    std::string name;
    double chinese = 0, math = 0, english = 0;
    double total() const { return chinese + math + english; }
    double average() const { return total() / 3; }
};
