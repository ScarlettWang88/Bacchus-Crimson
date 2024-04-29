
#include "barcode_scanner.h"

int main() {
    barcode_scanner_init();
    while (1) {
        if (barcode_detected()) {
            char barcode_data[20];
            barcode_read_data(barcode_data);
            process_barcode_data(barcode_data);
        }
    }
    return 0;
}
