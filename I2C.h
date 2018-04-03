/* 
 * File:   I2C.h
 * Author: Simon
 *
 * Created on 18. März 2018, 17:58
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    void lcd_cmd(char Package);
    void lcd_setup(void);
    void lcd_printChar(char Package);
    void heartbeat(void);
    void lcd_setCursor(char x, char y);
    void lcd_printStr(const char *s);
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

