#include "kernel.h"

char *vidptr = (char*)0xb8000;
int colors[] = {0x0f, 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x70, 0x8f, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xf0};
char *color_names[] = {
    "\t\t\t\t\t\t\t\t\tblack", 
    "\t\t\t\t\t\t\t\t\tblue", 
    "\t\t\t\t\t\t\t\t\tgreen", 
    "\t\t\t\t\t\t\t\t\tcyan", 
    "\t\t\t\t\t\t\t\t\tred", 
    "\t\t\t\t\t\t\t\t\tmagenta", 
    "\t\t\t\t\t\t\t\t\tbrown", 
    "\t\t\t\t\t\t\t\t\twhite", 
    "\t\t\t\t\t\t\t\t\tgrey", 
    "\t\t\t\t\t\t\t\t\tlight_blue", 
    "\t\t\t\t\t\t\t\t\tlight_green", 
    "\t\t\t\t\t\t\t\t\tlight_cyan", 
    "\t\t\t\t\t\t\t\t\tlight_red", 
    "\t\t\t\t\t\t\t\t\tlight_magenta", 
    "\t\t\t\t\t\t\t\t\tyellow", 
    "\t\t\t\t\t\t\t\t\twhite"
};

void clear_screen(unsigned int after_line){
    unsigned int cursor_pos = (after_line*80*2);
    for (unsigned int i=cursor_pos; i< MAX_LEN; i+=2){
        vidptr[i] = 0x00;
        vidptr[i+1] = 0x00;
    }
}

unsigned int kprint(char *message, int line, int *color, int *right_offset) //print string to console, similar to printk in linux
		{

            int cursor_pos = (line*80*2);
            if (color == NULL)
                color = ((int *)0x0f);
            if (right_offset == NULL)
                right_offset = ((int*) 0);
			while(*message!=0)
				{
					if(*message=='\n') // check for a new line
					{
						line++;
						cursor_pos = (line*80*2);
						*message++;
					} else if (*message=='\t') {
                        cursor_pos += 4;
                        *message++;
                    }else {
                        cursor_pos = cursor_pos + (int)right_offset;
						vidptr[cursor_pos]=*message;
                        right_offset = (int*)0;
						*message++;
						cursor_pos++;
						vidptr[cursor_pos]=(int)color;
						cursor_pos++;
					};
				};

				line = line + 1;

			return(1);
		};

void color_test(){
    int cpos = 0;
    while(cpos < 14){
        for (unsigned int i=0; i<MAX_LEN; i+=2){
            kprint("\t\t\t\t\t\t\t\t\tKevKernel VGA color_test", 12, NULL, NULL);
            kprint(color_names[cpos], 13, NULL, NULL);
            vidptr[i] = 0x00;
            vidptr[i+1] = colors[cpos];
        }
        wait(5e7);
        cpos += 1;
    }
}

void draw_smile(){
    /*
    draw a repeating stripe of all 8-bit VGA colors
    */
    int cpos = 0;
    for (unsigned int i = 2500; i < 2530; i+=2){
        vidptr[i] = 0x00;
        vidptr[i+1] = colors[cpos];
        //vidptr[i+2] = 0x00;
        //vidptr[i+3] = colors[cpos];
        cpos +=1;
        if (cpos > 15){
            cpos = 0;
        }
    }
    for (unsigned int i = 2818; i < 2850; i+=2){
        vidptr[i] = 0x00;
        vidptr[i+1] = colors[cpos];
        //vidptr[i+2] = 0x00;
        //vidptr[i+3] = colors[cpos];
        cpos +=1;
        if (cpos > 15){
            cpos = 0;
        }
    }
    for (unsigned int i = 3138; i < 3170; i+=2){
        vidptr[i] = 0x00;
        vidptr[i+1] = colors[cpos];
        //vidptr[i+2] = 0x00;
        //vidptr[i+3] = colors[cpos];
        cpos +=1;
        if (cpos > 15){
            cpos = 0;
        }
    }
    for (unsigned int i = 3458; i < 3490; i+=2){
        vidptr[i] = 0x00;
        vidptr[i+1] = colors[cpos];
        //vidptr[i+2] = 0x00;
        //vidptr[i+3] = colors[cpos];
        cpos +=1;
        if (cpos > 15){
            cpos = 0;
        }
    }
}

void enable_cursor(unsigned int cursor_start, unsigned int cursor_end)
{
	write_port(0x3D4, 0x0A);
	write_port(0x3D5, (read_port(0x3D5) & 0xC0) | cursor_start);
 
	write_port(0x3D4, 0x0B);
	write_port(0x3D5, (read_port(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
	unsigned int pos = y * 80 + x;
 
	write_port(0x3D4, 0x0F);
	write_port(0x3D5, (unsigned int) (pos & 0xFF));
	write_port(0x3D4, 0x0E);
	write_port(0x3D5, (unsigned int) ((pos >> 8) & 0xFF));
}

void disable_cursor()
{
    write_port(0x3D4, 0x0A);
	write_port(0x3D5, 0x20);
}