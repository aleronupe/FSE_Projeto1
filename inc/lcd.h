#ifndef LCD_H_
#define LCD_H_

void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);

// added by Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);

void escreve_temperaturas_lcd(float temp_int, float temp_ref, double temp_amb);

#endif /* I2C_H_ */