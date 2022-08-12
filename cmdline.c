#include "kernel.h"

int run_cmdline(char* cmd, unsigned int line){
    if (strcmp(cmd, "colortest") == 0){
        color_test();
        initialize_cmdprompt();
        return 4;
    } else if (strcmp(cmd, "mbinfo") == 0){
        print_multiboot_info();
        initialize_cmdprompt();
        return 4;
    } else if (strcmp(cmd, "clear") == 0){
        initialize_cmdprompt();
        return 4;
    } else if (strcmp(cmd, "reset") == 0){
        call_reset_vector();
    } else if (strcmp(cmd, "help") == 0){
        kprint("help: colortest, mbinfo, reset, clear", line, NULL, NULL);
        return line+1;
    }
    return line+1;
}