#include "system.h"

int main() {
    system_init();
    while (1) {
        system_check_sensors();
        system_perform_actions();
    }
    return 0;
}