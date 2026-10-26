#pragma once
#include "Student.h"
#include <vector>
class StudentStore {
public:
    const std::vector<Student>& all() const { return students_; }
    const Student* find(const std::string& id) const;
    bool add(const Student& s);
    bool update(const Student& s);
    bool remove(const std::string& id);
    bool load(const std::string& path, std::string& error);
    bool save(const std::string& path, std::string& error) const;
private:
    std::vector<Student> students_;
};
