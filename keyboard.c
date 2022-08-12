#include "kernel.h"
#include "keyboard.h"

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

unsigned int current_line=4;
unsigned int current_line_loc=0;
unsigned int current_loc=(4*160)+4;
unsigned int current_cmd_char=0;
char current_cmd[100];
unsigned char *current_kbmap=keyboard_map;

unsigned int put_char(unsigned char keycode){
    current_cmd[current_cmd_char] = current_kbmap[(unsigned char) keycode];
    current_cmd_char++;
	VIDPTR[current_loc++] = current_kbmap[(unsigned char) keycode]; //put pressed char
	VIDPTR[current_loc++] = 0x0f; //set char color
}

void _init_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));

	int count = 0;
	while(count < 6){ //while loop to properly space type lines
		current_loc++; //move current cursor location right one-half space
		count++; //increment count variable
	};
}

unsigned int newline(int new_num){
	while(new_num!=1){ //have to have it as !=1 instead of !=0 due to zero-indexing
		_init_newline();
		new_num = new_num - 1;
	};
}

void keyboard_handler_main(void)
{
	unsigned char status;
	char keycode;
    int cmd_status;

	/* write EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		if(keycode == F3_KEY_CODE) {
			call_reset_vector();
			return;
		}

		if(keycode == ENTER_KEY_CODE) { //if user presses ENTER key
            current_line++;
            current_cmd[current_cmd_char] = '\0';
            current_cmd_char = 0;
            cmd_status = run_cmdline((char*)current_cmd, current_line);
            current_line = cmd_status;
            current_line_loc=0;
            current_loc = current_line*160+4;
            kprint("> ", current_line, NULL, NULL);
			return;
		}

		if(keycode == 0x0E){ //if backspace is pressed, remove previous key entry
                current_cmd_char--;
				VIDPTR[current_loc--] = cap_keyboard_map[(unsigned char) 29];
				current_loc--;
				VIDPTR[current_loc] = cap_keyboard_map[(unsigned char) 29];
				return;
		}

	}
    put_char(keycode);
}

void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void kb_init(void)
{
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , 0xFD);
}