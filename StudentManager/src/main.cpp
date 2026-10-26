#include "ConsoleApp.h"
#include <windows.h>
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    ConsoleApp app;
    return app.run();
}
