# 学生成绩管理系统

C++17 控制台项目，使用传统 Visual Studio .sln 和 .vcxproj。

## 打开运行

双击 StudentManager.sln，选择 Debug / x64，按 Ctrl+F5 编译运行，F5 调试。
按本机 Visual Studio 2026 的 v145 工具集配置，需要“使用 C++ 的桌面开发”组件。

## 功能

录入、列表、按学号查询、修改、确认删除、按总分排名、班级平均分和三科均及格比例。
学号不可重复，姓名不可为空，成绩限制为 0–100。
选择 8 保存到工作目录的 students.txt，启动时自动加载，退出时提示未保存修改。
保存保留上一版 students.txt.bak；无效数据文件会阻止启动，避免覆盖。
数据文件、IDE 缓存和编译输出已通过本目录 .gitignore 忽略。

## 代码结构

- Student.h：学生信息、总分和平均分。
- StudentStore.h / .cpp：增删改查、校验和文件读写。
- ConsoleApp.h / .cpp：菜单、输入和统计。
- main.cpp：程序入口及 Windows 控制台 UTF-8 编码。

## 试用

输入 1，学号 1001，姓名 Alice，成绩 80、90、85。
输入 2 查看，6 排名，7 统计，8 保存，0 退出。
重新运行并输入 2，可验证数据已保存。

## 提交代码

在 Visual Studio 的“Git 更改”中查看新增文件，填写提交说明，全部提交，然后推送。
