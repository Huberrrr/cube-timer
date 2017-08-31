/*   
 Rubik's Cube Timer by Jon Huber
 TODO: PB Detection (DONE), Inspection + Buzzer (DONE), Different Cubes, CLEAN UP
*/

// LCD Setup
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// Variable Setup
const int button = 8;
const int led =  9;
const int buzzer = 10;
int buttonstate;
double pb = 60;


// MAIN SETUP
void setup() {

  // LCD Setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("timer ready");
  lcd.setCursor(2,1);
  lcd.print("pb is: ");
  lcd.print(pb);  

  // Pin Setup
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
}


// MAIN LOOP
void loop() {

  start:
  
  int pressed = 0;
  int inspectime = 0;
  buttonstate = digitalRead(button);
  lcd.setCursor(0,0);

  
  // Start Sequence
  if (buttonstate == LOW) {

    lcd.clear();

    // Inspection
    while (inspectime != 15) {
      buttonstate = digitalRead(button);
      lcd.setCursor(7,0);
      lcd.print(inspectime);
  
      // Inspection Warning
      if (inspectime == 8) {
        tone(buzzer, 262, 100);
      }
  
      //Button Detection
      if (buttonstate == LOW && inspectime > 1) {
        pressed = 1;
        lcd.setCursor(0,1);
        lcd.print("release to start");
   
        while (pressed == 1) {
          buttonstate = digitalRead(button);
  
          // Release Detection
          if (buttonstate == HIGH) {
            pressed = 0;
            tone(buzzer, 262, 100);
            timer();
            goto start;
          }
      
        }
    
      }
  
      inspectime++;
      delay(1000);
      
    }

    // Start If Inspection Runs Out
    inspectime++;
    lcd.setCursor(7,0);
    lcd.print(inspectime);
    tone(buzzer, 262, 100);
    inspectime = 0;
    timer();
  
  }
  
}


// COUNTING FUNCTION
void timer() {

  // Variable Setup
  int count = 1;
  int ledblink = 0;
  double timer = 0;
  buttonstate = digitalRead(button);

  // LCD Setup
  lcd.clear();
  lcd.setCursor(5,0);

  // Counting Loop
  while (true) {

    // Create and Print Time
    lcd.print(timer);
    timer += .01;
    delay(10);
    lcd.setCursor(5,0);

    // Stop Detection
    buttonstate = digitalRead(button);
    if (buttonstate == LOW) {
      
      digitalWrite(led, LOW);

      // PB Detection
      if (timer < pb) {
        lcd.setCursor(4,1);
        lcd.print("new pb");
        pb = timer;
      }

      // Sub-X Detection
      else if (timer < 5) {
        lcd.setCursor(3,1);
        lcd.print("sub 5 sec");
      } else if (timer < 10) {
        lcd.setCursor(3,1);
        lcd.print("sub 10 sec");
      } else if (timer < 30) {
        lcd.setCursor(3,1);
        lcd.print("sub 30 sec");
      } else if (timer < 60) {
        lcd.setCursor(3,1);
        lcd.print("sub 1 min");
      }

      // Return to Main Loop
      delay(5000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("timer ready");  
      lcd.setCursor(2,1);
      lcd.print("pb is: ");
      lcd.print(pb);    
      break;
    }

    // LED Blink
    if (ledblink == 100) {
      digitalWrite(led, HIGH);
      ledblink = 0;
    } else if (ledblink == 10) {
      digitalWrite(led, LOW);
    }
    ledblink++;
    
  }
  
}