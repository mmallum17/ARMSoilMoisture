/*
 * ssd1306.h
 *
 *  Created on: Dec 5, 2017
 *      Author: Marcus
 */

#ifndef SSD1306_H_
#define SSD1306_H_
/*Public Function Prototypes*/
void setCursorX(uint8_t position);
void setCursorY(uint8_t position);
uint8_t getCursorX();
uint8_t getCursorY();
void ssd1306Init();
void ssd1306WriteCommand(uint8_t command);
void clearScreen();
void updateScreen();
void ssd1306_DrawPixel(uint16_t x, uint16_t y, uint8_t color);
void ssd1306_WriteData(uint8_t* data, uint16_t count);
char ssd1306_WriteChar(char ch, uint8_t color);
char ssd1306_WriteString(char* str, uint8_t color);
#endif /* SSD1306_H_ */
