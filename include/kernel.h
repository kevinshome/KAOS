#define MAX_LEN 4000
#define NULL ((void *) 0)

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

extern char *itoa(int num);
extern void clear_screen(unsigned int after_line);
unsigned int kprint(char *message, int line, int *color, int *right_offset);
extern void color_test();
extern void draw_smile();
extern void enable_cursor(unsigned int cursor_start, unsigned int cursor_end);
extern void update_cursor(int x, int y);
extern void disable_cursor();
extern void wait(unsigned int time);
extern int strcmp(const char *s1, const char *s2);