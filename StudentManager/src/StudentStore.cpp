#include "StudentStore.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
namespace {
bool valid(const Student& s) {
    auto score = [](double n) { return std::isfinite(n) && n >= 0 && n <= 100; };
    return !s.id.empty() && !s.name.empty() &&
        s.id.find_first_of("\r\n") == std::string::npos &&
        s.name.find_first_of("\r\n") == std::string::npos &&
        score(s.chinese) && score(s.math) && score(s.english);
}
}
const Student* StudentStore::find(const std::string& id) const {
    for (const auto& s : students_) if (s.id == id) return &s;
    return nullptr;
}
bool StudentStore::add(const Student& s) {
    if (!valid(s) || find(s.id)) return false;
    students_.push_back(s); return true;
}
bool StudentStore::update(const Student& s) {
    if (!valid(s)) return false;
    for (auto& item : students_) if (item.id == s.id) { item = s; return true; }
    return false;
}
bool StudentStore::remove(const std::string& id) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&](const Student& s) { return s.id == id; });
    if (it == students_.end()) return false;
    students_.erase(it); return true;
}
bool StudentStore::load(const std::string& path, std::string& error) {
    error.clear();
    std::ifstream input(path);
    if (!input) { error = "无法打开数据文件。"; return false; }
    StudentStore pending;
    std::string line;
    int number = 0;
    while (std::getline(input, line)) {
        ++number;
        if (line.empty()) continue;
        Student s;
        std::istringstream row(line);
        if (!(row >> std::quoted(s.id) >> std::quoted(s.name) >> s.chinese >> s.math >> s.english)
            || !(row >> std::ws).eof() || !pending.add(s)) {
            error = "第 " + std::to_string(number) + " 行无效或学号重复，未加载。";
            return false;
        }
    }
    if (input.bad()) { error = "读取失败。"; return false; }
    students_ = std::move(pending.students_); return true;
}
bool StudentStore::save(const std::string& path, std::string& error) const {
    error.clear();
    const auto temp = path + ".tmp", backup = path + ".bak";
    std::ofstream output(temp, std::ios::trunc);
    if (!output) { error = "无法创建临时文件。"; return false; }
    output << std::setprecision(17);
    for (const auto& s : students_)
        output << std::quoted(s.id) << ' ' << std::quoted(s.name) << ' '
               << s.chinese << ' ' << s.math << ' ' << s.english << '\n';
    output.close();
    if (!output) { error = "写入失败，原文件保持不变。"; return false; }
    std::error_code ec;
    const bool exists = std::filesystem::exists(path, ec);
    if (ec) { error = "无法检查数据文件。"; return false; }
    if (exists) {
        std::filesystem::remove(backup, ec);
        if (ec) { error = "无法更新备份。"; return false; }
        std::filesystem::rename(path, backup, ec);
        if (ec) { error = "无法备份原文件。"; return false; }
    }
    std::filesystem::rename(temp, path, ec);
    if (ec) {
        if (exists) { std::error_code restore; std::filesystem::rename(backup, path, restore); }
        error = "保存失败，请检查原文件或 .bak 备份。"; return false;
    }
    return true;
}
