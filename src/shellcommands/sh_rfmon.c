#include "os/serial_port.h"
#include "os/shell/shell.h"
#include "os/shell/shell_command_processor.h"
#include "os/shell/shell_cursor.h"
#include "os/shell/shell_keys.h"
#include "os/shell/shell_utils.h"
#include "os/system_time.h"
#include "rf_sensor.h"
#include "shell_command_processor.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ************************************************************************** */

#define PRINT_COOLDOWN 1000

bool attempt_print(void) {
    static system_time_t lastAttempt = 0;
    if (time_since(lastAttempt) > PRINT_COOLDOWN) {
        lastAttempt = get_current_time();

        printf("FWD: %9.4f", currentRF.forwardVolts);
        print(" | ");
        printf("REV: %9.4f", currentRF.reverseVolts);
        print(" | ");
        printf("FWatts: %9.4f", currentRF.forwardWatts);
        print(" | ");
        printf("RWatts: %9.4f", currentRF.reverseWatts);
        print(" | ");
        printf("Q: %9.4f", currentRF.matchQuality);
        print(" | ");
        printf("Freq: %u", currentRF.frequency);

        println("");
        return true;
    }
    return false;
}

/* ************************************************************************** */

int8_t rfmon_callback(char currentChar) {
    if (currentChar == 3) {
        return -1;
    }
    static bool prevIsPresent = false;

    if (prevIsPresent != currentRF.isPresent) {
        if (currentRF.isPresent) {
            println("RF is present!");
        } else {
            println("RF is absent!");
        }
    }

    if (currentRF.isPresent) {
        attempt_print();
    }

    prevIsPresent = currentRF.isPresent;

    return 0;
}

// setup
void sh_rfmon(int argc, char **argv) {
    println("entering rfmon");

    shell_register_callback(rfmon_callback);
}

// REGISTER_SHELL_COMMAND(sh_rfmon, "rfmon");