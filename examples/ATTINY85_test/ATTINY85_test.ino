//
//    FILE: ATTINY85_test.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test ATtiny in build
//     URL: https://github.com/RobTillaart/INA226_USI
//


uint8_t LED1 = 2;
uint8_t LED2 = 3;


void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

}


void loop()
{
  uint32_t now = millis();
  digitalWrite(LED1, now & 0x0100);
  digitalWrite(LED2, now & 0x0200);
}


//  -- END OF FILE --
