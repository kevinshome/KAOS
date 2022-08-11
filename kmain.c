#include "kernel.h"
#include "multiboot.h"

struct k_cfg
{
    int run_colortest;
};

struct k_cfg kernel_config;

void init_config(){
    kernel_config.run_colortest=0;
}

void wait(unsigned int time){

    for (unsigned int x=0; x<time; x++){
        ;
    }
}

int run_cmd(char* cmd){
    if (strcmp(cmd, "kern") == 0){
        return 0;
    } else if (strcmp(cmd, "color_test") == 0){
        kernel_config.run_colortest=1;
        return 0;
    } else {
        return 1;
    }
}

void parse_cmdline(char* cmdline){
    char current_str[100];
    int num = 0;
    while(*cmdline!=0){
        if (*cmdline == ' '){
            current_str[num] = '\0';
            num = 0;
            run_cmd((char*)current_str);
            *cmdline++;
        }
        current_str[num]=*cmdline;
        *cmdline++;
        num++;
    }
    run_cmd((char*)current_str);
}

void kmain(const void* multiboot_struct){
    // disable VGA text mode cursor
    init_config();
    disable_cursor();
    clear_screen(0);
    const multiboot_info_t* mb_info = multiboot_struct;            /* Make pointer to multiboot_info_t struct */
    char* mb_cmdline = (char*)mb_info->cmdline;
    parse_cmdline(mb_cmdline);

    // run VGA color_test
    if (kernel_config.run_colortest == 1)
        color_test();
        clear_screen(0);

    // print welcome message
    kprint("running kevkernel 0.0.0 pre-alpha", 0, NULL, NULL);
    kprint("unexpected behaviors may occur", 1, NULL, NULL);
    kprint("\t\t\t\t\t\t\t\t\tWelcome to KevKernel :)", 12, NULL, NULL);
    wait(2e8);
    clear_screen(2);
    kprint("\t\t\t\t\t\t\t\t\tA custom kernel by kevinshome.", 12, NULL, NULL);
    wait(2e8);
    clear_screen(2);
    kprint("\t\t\t\t\t\t\t\t\tend_of_execution:0x00000000", 12, NULL, NULL);
    draw_smile();
    while(1);
}
