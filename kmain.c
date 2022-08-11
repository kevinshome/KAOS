#include "kernel.h"
#include "multiboot.h"

struct k_cfg
{
    int run_colortest;
    int suppress_splash_screen;
    int show_mb_info;
};

struct k_cfg kernel_config;

void init_config(){
    kernel_config.run_colortest=0;
    kernel_config.suppress_splash_screen=0;
    kernel_config.show_mb_info=0;
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
    } else if (strcmp(cmd, "nosplash") == 0){
        kernel_config.suppress_splash_screen=1;
        return 0;
    } else if (strcmp(cmd, "mbinfo") == 0){
        kernel_config.show_mb_info=1;
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

int print_multiboot_info(const multiboot_info_t* mb_info){
    const struct multiboot_apm_info* mb_apm = (void*) mb_info->apm_table;
    const struct multiboot_mmap_entry* mb_mmap = (void*) mb_info->mmap_addr;
    if (mb_info==NULL)
        return 1;
    int *magenta = ((int*)0x0d);
    int *cyan = ((int*)0x0b);

    while (1){
        // multiboot_info_t
        kprint("Variables from multiboot_info_t struct:\n------------", 4, magenta, NULL);
        kprint("boot_loader_name:", 6, cyan, NULL);
        kprint(((char*) mb_info->boot_loader_name), 6, cyan, ((int*) 36));
        kprint("boot_device:", 7, cyan, NULL);
        kprint(itoa(mb_info->boot_device), 7, cyan, ((int*) 26));
        kprint("cmdline:", 8, cyan, NULL);
        kprint(((char*) mb_info->cmdline), 8, cyan, ((int*) 18));
        kprint("mem_lower:", 9, cyan, NULL);
        kprint(itoa(mb_info->mem_lower), 9, cyan, ((int*) 22));
        kprint("mem_upper:", 10, cyan, NULL);
        kprint(itoa(mb_info->mem_upper), 10, cyan, ((int*) 22));
        kprint("flags:", 11, cyan, NULL);
        kprint(itoa(mb_info->flags), 11, cyan, ((int*) 14));
        wait(5e8);
        clear_screen(4);
        draw_smile();

        // multiboot_apm_info
        kprint("Variables from multiboot_apm_info struct:\n------------", 4, magenta, NULL);
        kprint("version:", 6, cyan, NULL);
        kprint(itoa(mb_apm->version), 6, cyan, ((int*) 18));
        kprint("cseg:", 7, cyan, NULL);
        kprint(itoa(mb_apm->cseg), 7, cyan, ((int*) 12));
        kprint("offset:", 8, cyan, NULL);
        kprint(itoa(mb_apm->offset), 8, cyan, ((int*) 16));
        kprint("cseg_16:", 9, cyan, NULL);
        kprint(itoa(mb_apm->cseg_16), 9, cyan, ((int*) 18));
        kprint("dseg:", 10, cyan, NULL);
        kprint(itoa(mb_apm->dseg), 10, cyan, ((int*) 12));
        kprint("flags:", 11, cyan, NULL);
        kprint(itoa(mb_apm->flags), 11, cyan, ((int*) 14));
        kprint("cseg_len:", 12, cyan, NULL);
        kprint(itoa(mb_apm->cseg_len), 12, cyan, ((int*) 20));
        kprint("cseg_16_len:", 13, cyan, NULL);
        kprint(itoa(mb_apm->cseg_16_len), 13, cyan, ((int*) 26));
        kprint("dseg_len:", 14, cyan, NULL);
        kprint(itoa(mb_apm->dseg_len), 14, cyan, ((int*) 20));
        wait(5e8);
        clear_screen(4);
        draw_smile();
        
        // multiboot_mmap_entry
        kprint("Variables from multiboot_mmap_entry struct:\n------------", 4, magenta, NULL);
        kprint("size:", 6, cyan, NULL);
        kprint(itoa(mb_mmap->size), 6, cyan, ((int*) 12));
        kprint("addr:", 7, cyan, NULL);
        kprint(itoa(mb_mmap->addr), 7, cyan, ((int*) 12));
        kprint("len:", 8, cyan, NULL);
        kprint(itoa(mb_mmap->len), 8, cyan, ((int*) 10));
        kprint("type:", 9, cyan, NULL);
        kprint(itoa(mb_mmap->type), 9, cyan, ((int*) 12));
        wait(5e8);
        clear_screen(4);
        draw_smile();

    }
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
    if (kernel_config.suppress_splash_screen == 0){
        kprint("Welcome to KevKernel :)", 12, NULL, ((int*)36));
        wait(2e8);
        clear_screen(2);
        kprint("A custom kernel by kevinshome.", 12, NULL, ((int*)36));
        wait(2e8);
    }
    clear_screen(2);
    draw_smile();
    if (kernel_config.show_mb_info == 1)
        print_multiboot_info(mb_info);
    kprint("end_of_execution:0x00000000", 23, NULL, ((int*)100));
    while(1);
}
