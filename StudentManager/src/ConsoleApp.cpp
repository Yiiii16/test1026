#include "ConsoleApp.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
namespace {
const std::string dataFile = "students.txt";
std::string ask(const std::string& prompt) {
    std::cout << prompt;
    std::string text;
    if (!std::getline(std::cin, text)) throw std::runtime_error("EOF");
    return text;
}
double score(const std::string& prompt) {
    for (;;) {
        std::istringstream input(ask(prompt));
        double value = 0;
        if ((input >> value) && (input >> std::ws).eof() && std::isfinite(value)
            && value >= 0 && value <= 100) return value;
        std::cout << "请输入 0 到 100 的有效分数。\n";
    }
}
void print(const Student& s) {
    std::cout << s.id << " | " << s.name << " | " << std::fixed << std::setprecision(2)
        << s.chinese << " | " << s.math << " | " << s.english
        << " | 总分 " << s.total() << " | 平均 " << s.average() << '\n';
}
}
void ConsoleApp::list(bool ranked) const {
    auto rows = store_.all();
    if (rows.empty()) { std::cout << "暂无学生，请先录入。\n"; return; }
    if (ranked) std::stable_sort(rows.begin(), rows.end(),
        [](const Student& a, const Student& b) { return a.total() > b.total(); });
    std::cout << "学号 | 姓名 | 语文 | 数学 | 英语 | 总分 | 平均分\n";
    for (const auto& s : rows) print(s);
}
void ConsoleApp::edit(bool adding) {
    Student s;
    s.id = ask("学号：");
    if (adding ? store_.find(s.id) != nullptr : store_.find(s.id) == nullptr) {
        std::cout << (adding ? "学号已存在。\n" : "未找到该学生。\n"); return;
    }
    s.name = ask("姓名：");
    if (s.id.empty() || s.name.empty()) { std::cout << "学号和姓名不能为空。\n"; return; }
    s.chinese = score("语文成绩：");
    s.math = score("数学成绩：");
    s.english = score("英语成绩：");
    if (adding ? store_.add(s) : store_.update(s)) {
        dirty_ = true; std::cout << "操作成功，请选择 8 保存。\n";
    }
}
void ConsoleApp::statistics() const {
    const auto& rows = store_.all();
    if (rows.empty()) { std::cout << "暂无数据。\n"; return; }
    double sum = 0;
    std::size_t passed = 0;
    for (const auto& s : rows) {
        sum += s.average();
        if (s.chinese >= 60 && s.math >= 60 && s.english >= 60) ++passed;
    }
    const auto count = static_cast<double>(rows.size());
    std::cout << std::fixed << std::setprecision(2) << "学生人数：" << rows.size()
        << "\n全班平均分：" << sum / count << "\n三科均及格比例："
        << 100.0 * static_cast<double>(passed) / count << "%\n";
}
bool ConsoleApp::save() {
    std::string error;
    if (!store_.save(dataFile, error)) { std::cout << error << '\n'; return false; }
    dirty_ = false;
    std::cout << "已保存至 " << std::filesystem::absolute(dataFile).string() << '\n';
    return true;
}
int ConsoleApp::run() {
    std::cout << "=== 学生成绩管理系统 ===\n";
    std::error_code ec;
    if (std::filesystem::exists(dataFile, ec)) {
        std::string error;
        if (!store_.load(dataFile, error)) {
            std::cout << error << "\n请修复或移走数据文件后再启动，避免覆盖已有数据。\n";
            return 1;
        }
    } else if (ec) { std::cout << "无法访问数据目录。\n"; return 1; }
    try {
        for (;;) {
            std::cout << "\n1 录入  2 列表  3 查询  4 修改  5 删除\n6 排名  7 统计  8 保存  0 退出\n";
            const auto choice = ask("请选择：");
            if (choice == "1") edit(true);
            else if (choice == "2") list(false);
            else if (choice == "3") {
                const auto* s = store_.find(ask("学号："));
                if (s) print(*s); else std::cout << "未找到该学生。\n";
            } else if (choice == "4") edit(false);
            else if (choice == "5") {
                const auto id = ask("要删除的学号：");
                if (!store_.find(id)) std::cout << "未找到该学生。\n";
                else if (ask("确认删除？输入 y：") == "y") {
                    store_.remove(id); dirty_ = true; std::cout << "已删除，请保存。\n";
                }
            } else if (choice == "6") list(true);
            else if (choice == "7") statistics();
            else if (choice == "8") save();
            else if (choice == "0") {
                if (!dirty_) return 0;
                const auto answer = ask("有未保存修改：s 保存退出 / d 放弃退出 / 其他返回：");
                if (answer == "d" || (answer == "s" && save())) return 0;
            } else std::cout << "无效选项，请重新输入。\n";
        }
    } catch (const std::runtime_error&) {
        std::cout << "\n输入已结束。" << (dirty_ ? "未保存修改未写入文件。\n" : "\n");
        return dirty_ ? 1 : 0;
    }
}
