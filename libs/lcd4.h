#define LCD_CMD_LINEONE     0x00
#define LCD_CMD_LINETWO     0x40

#define LCD_CMD_CLEAR           0b00000001
#define LCD_CMD_HOME            0b00000010
#define LCD_CMD_ENTRYMODE       0b00000110
#define LCD_CMD_DISPLAYOFF      0b00001000
#define LCD_CMD_DISPLAYON       0b00001100
#define LCD_CMD_FUNCTIONRESET   0b00110000
#define LCD_CMD_FUNCTIONSET4BIT 0b00101000
#define LCD_CMD_SETCURSOR       0b10000000

void LCDPREFIX_write(uint8_t what);
void LCDPREFIX_write_instruction(uint8_t what);
void LCDPREFIX_write_char(uint8_t what);
void LCDPREFIX_write_zchar(char* what);
void LCDPREFIX_write_a_line(char* what);
void LCDPREFIX_write_lines(char* line0, char* line1);
void LCDPREFIX_init();
