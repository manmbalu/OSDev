#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS   0xB8000
#define MAX_ROWS        25
#define MAX_COLS        80

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

#define BLACK           0x0
#define BLUE            0x1
#define GREEN           0x2
#define CYAN            0x3
#define RED             0x4
#define MAGENTA         0x5
#define BROWN           0x6
#define LIGHT_GRAY      0x7 
#define DARK_GRAY       0x8
#define LIGHT_BLUE      0x9
#define LIGHT_GREEN     0xA
#define LIGHT_CYAN      0xB
#define LIGHT_RED       0xC
#define LIGHT_MAGENTA   0xD
#define YELLOW          0xE
#define WHITE           0xF

struct Character
{
    uint8_t character;
    uint8_t colors;
};


void kclear();
void kprint_at(char *character, uint8_t col, uint8_t row);
void kprint(char *c);
void itoa(int n, char str[]);
int strlen(char s[]);

void change_colors(uint8_t background, uint8_t foreground);

#endif