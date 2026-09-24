#include "app_window.h"

int main() {
    AppWindow w;
    if (!w.create()) return 1;
    return w.run();
}
