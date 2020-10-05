#define RST 2
#define CE 4
#define DC 5
#define DIN 6
#define CLK 7
#define LIGHT 3
 
#include "LCD5110_Logwriter.h"


LCD5110 lcd(RST,CE,DC,DIN,CLK);
int line=0;

void setup() {
  pinMode(LIGHT, OUTPUT);     
  digitalWrite(LIGHT, HIGH); 
  lcd.begin();
  lcd.println("     LCD    ");
  delay(1000);
  lcd.println("     5110   ");
  delay(1000);
  lcd.println("   Logwriter");
  delay(1000);
  lcd.println("1234567890ABCD");
  delay(1000);
  lcd.println("Ueberlanger Text");
  delay(1000);
}

void loop() {
  
  lcd.print("Zeile ");
  lcd.println(line);
  delay(1000);
  line++;
}
