//
//    FILE: V_A_Meter_INA226_ATtiny85.ino
//  AUTHOR: J.P. Simoons
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/INA226_USI
//
//  needs INA226_USI.h, TinyLiquidCrystal.h and TinyWireM.h to installed
//
// Version 1.1: replace LiquidCrystal_attiny_I2C.h by Tiny_LiquidCrystal.h
//              use installed versions of INA226_USI and TinyWireM
//
#include <avr/pgmspace.h> 
#include <TinyLiquidCrystal_I2C.h>
#include <INA226_USI.h>

#define LCD_ADDR 0x27                  // I2C address of LCD display
#define LCD_WIDTH 8                    // Number of colums of LCD display
#define LCD_HEIGHT 2                   // Number of rows of LCD display

/* connection diagram ATtiny85:
 *         Reset PB5 1 +--V--+ 8 Vcc
 *               PB3 2 |     | 7 PB2 = I2C:SCL to LCD display & INA226
 *               PB4 3 |     | 6 PB1 
 *            Ground 4 +-----+ 5 PB0 = I2C:SDA to LCD display & INA226
 *
 * Note: SHUNT * MAX_CURRENT must be smaller than 81,92 mV
 */
#define INA_ADDR 0x40                  // I2C address of INA module
#define SHUNT 0.1                      // INA shunt resistor for current meter in ohms
#define MAX_CURRENT 0.5                // Max current in A to be measured
/*
 * Global data
 */
char lcd_img[LCD_HEIGHT][LCD_WIDTH+1];          /* LCD image */
/*
 * define objects
 */
TinyLiquidCrystal_I2C lcd(LCD_ADDR, LCD_WIDTH,LCD_HEIGHT);
INA226 INA(INA_ADDR);
 /*
 * Function prototype
 */
void write_lcd(void);
/*
 * Setup
 */
void setup()
{
  lcd.init();                                     /* initialyse LCD and I2C */
  lcd.backlight();                                /* turn on backlicht if not allready on */
//                           01234567
  sprintf_P(lcd_img[0],PSTR("VA meter"));
  sprintf_P(lcd_img[1],PSTR("  V1.1  "));
  write_lcd();
//  TinyWireM.begin();                             /* Required before INA.begin(), but already done by lcd.init() */
  if (!INA.begin() ) 
  {
    delay(1000);                                   /* Allow reading welcome message */
//                             012345678
    sprintf_P(lcd_img[0],PSTR("*Error! *"));
    sprintf_P(lcd_img[1],PSTR(" No INA! "));
    write_lcd();
    while (true){};                                /* could not connect to INA, give up */
//    return;                                      /* could not connect to INA, give up */
  }
  INA.setMaxCurrentShunt(MAX_CURRENT, SHUNT);
  delay(5000);                                     /* allow reading of the welcome message */
  return;
}
/*
 * main loop
 */
void loop()
{
  unsigned short i_current,i_volt;
  float bus_voltage, current;

  bus_voltage = INA.getBusVoltage();
  current = INA.getCurrent_mA();
/*
 * Arduino sprintf does not support floating point printing.
 * convert current into integer for printing.
 * split voltage into volts and milli volts for printing
 * if current > 1000 mA split current into Ampere and milli Ampere for printing
 *    01234567
 *    xx.xx  V
 *      xxx mA 
 * 
 *    xx.xx  V
 *    x.xxx  A 
 */
  i_current = current + 0.5;          // convert to integer for printing
  bus_voltage *= 100.0;               // units of 10 mV
  i_volt = bus_voltage + 0.5;         // convert to integer for printing
  sprintf (lcd_img[0],"%4d   V",i_volt);
  lcd_img[0][4] = lcd_img[0][3];         // make place for decimal dot
  lcd_img[0][3] = lcd_img[0][2];         // make place for decimal dot
   lcd_img[0][2] = '.';
  if (lcd_img[0][1] == ' ') lcd_img[0][1] = '0';               // .00 --> 0.00
  if (lcd_img[0][3] == ' ') lcd_img[0][3] = '0';               // 0. 0 --> 0.00
  sprintf (lcd_img[1]," %4d mA",i_current);
  if (i_current >= 1000)                                      // xxxx mA --> x.xxx A
  {
    lcd_img[1][6] = ' ';                                      // mA --> A
    lcd_img[1][0] = lcd_img[1][1];        // make place for decimal dot
    lcd_img[1][1] = '.';
  }
  write_lcd();
  delay(100);                                    // not more than 10 updates / sec for display
  return;
}
void write_lcd()
{
  unsigned char i; 
  for (i=0;i<LCD_HEIGHT;i++)
  {
    lcd_img[i][LCD_WIDTH] = '\0';                     /* terminate string */
    lcd.setCursor(0,i);                               /* set cursor to begin of right line */
    lcd.print(lcd_img[i]);                            /* display line */
  }
  return;
} 
