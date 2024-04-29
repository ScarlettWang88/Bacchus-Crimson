
#include "nfc_reader.h"

int main() {
    nfc_init();
    while (1) {
        if (nfc_card_detected()) {
            char card_id[10];
            nfc_read_card_id(card_id);
            log_card_id(card_id);
        }
    }
    return 0;
}
