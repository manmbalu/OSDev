#include "../kernel/util.h"
#include "screen.h"

uint16_t get_cursor_offset();
void set_cursor_offset(uint16_t offset);
void print_char(char c);
uint16_t get_offset(int col, int row);
uint8_t get_offset_row(int offset);
uint8_t get_offset_col(int offset);

uint8_t *video_memory = VIDEO_ADDRESS; 
uint8_t colors = 0x0f;

uint8_t cursor_x;
uint8_t cursor_y;

void kprint(char *c)
{
    int i = 0;
    while(c[i])
    {
        print_char(c[i++]);
    }
    set_cursor_offset(get_offset(cursor_x, cursor_y));
}

void itoa(int n, char str[]) {
    
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

// void hex_to_ascii(int n, char str[]) {
//     append(str, '0');
//     append(str, 'x');
//     char zeros = 0;

//     int32_t tmp;
//     int i;
//     for (i = 28; i > 0; i -= 4) {
//         tmp = (n >> i) & 0xF;
//         if (tmp == 0 && zeros == 0) continue;
//         zeros = 1;
//         if (tmp > 0xA) append(str, tmp - 0xA + 'a');
//         else append(str, tmp + '0');
//     }

//     tmp = n & 0xF;
//     if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
//     else append(str, tmp + '0');
// }

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

// void kprint_at(char *c, uint8_t col, uint8_t row)
// {
//     uint16_t offset = get_offset(col, row);

//     video_memory[offset] =  *c << 8 | colors;
//     // video_memory[offset + 1] =  colors;
// }

void kclear()
{   
    for(uint8_t i = 0; i < MAX_ROWS; i++)
    {
        for(uint8_t j = 0; j < MAX_COLS; j++)
        {
            uint16_t offset = get_offset(j, i);

            video_memory[offset] =  ' ';
            video_memory[offset + 1] =  colors;
        }
    }
}

void change_colors(uint8_t background, uint8_t foreground)
{
    colors = background << 4 | foreground;
}

void print_char(char c)
{
    uint16_t offset = get_offset(cursor_x, cursor_y);

    if(c == '\n')
    {
        cursor_y++;
        cursor_x = 0;
    }
    else
    {
        video_memory[offset] =  c;
        video_memory[offset + 1] =  colors;
        if(cursor_x < MAX_COLS)
            cursor_x++;
        else
        {
            cursor_y++;
            cursor_x = 0;
        }
    }

    if(cursor_y >= MAX_ROWS)
    {
        for(int row = 1; row < MAX_ROWS; row++)
        {
            memcpy((uint8_t*)(get_offset(0, row) + VIDEO_ADDRESS), (uint8_t*)(get_offset(0, row - 1) + VIDEO_ADDRESS), MAX_COLS * 2);
        }

        cursor_y--;

        for(uint8_t j = 0; j < MAX_COLS; j++)
        {
            offset = get_offset(j, 24);

            video_memory[offset] =  ' ';
            video_memory[offset + 1] =  colors;
        }
    }
}

uint16_t get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
uint8_t get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
uint8_t get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }

uint16_t get_cursor_offset()
{
    outb(REG_SCREEN_CTRL, 14);
    uint16_t offset = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 15);
    offset |= inb(REG_SCREEN_DATA);
}

void set_cursor_offset(uint16_t offset)
{
    offset /= 2;

    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, offset >> 8);
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, offset);
}
