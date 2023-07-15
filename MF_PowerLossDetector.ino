#include <EEPROM.h>
const int batteryPin = A1;
const int stepPin = 7; 
const int dirPin = 6; 
const int enPin = 5;
const int buttonh = 22;
const int button1 = 24;
const int button2 = 26;
const int button3 = 28;
const int buttone = 23;
int buttonStatee = LOW;
int buttonStateh = LOW;
int buttonState1 = LOW;
int buttonState2 = LOW;
int buttonState3 = LOW;
int pos;

 void forward_Start(){
   digitalWrite(dirPin,LOW); 
  for(int x = 1; x < 501; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(600); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(600);
    }
 }
 
 void forward_StartSmall(){
    digitalWrite(dirPin,LOW);
    for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(600); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(600); 
  }
  delay(500);
 }

void moveforward(){
  buttonStatee = LOW; buttonStateh = LOW; buttonState1 = LOW; buttonState2 = LOW; buttonState3 = LOW;
   if(pos > 240){
              pos = 240;
              EEPROM.update(0, pos);
              maindrive();
            }
            
   digitalWrite(dirPin,LOW); 
        for(int x = 1; x < 501; x++) {
          int vol = analogRead(batteryPin);
          if(vol <= 1010){
            EEPROM.update(0, pos);
            EEPROM.update(2,1);
            Serial.println("Voltage drop detected.. stopping system. please reboot");
            maindrive();
          }
          else{
                        buttonStatee = digitalRead(buttone);
                          if(buttonStatee == HIGH){
                            delay(2000);
                            Serial.println(pos + "Estop detected: LOADING... maintenance mode" );
                            delay(2000);
                            Serial.println("Last saved Value: " + pos);
                            EEPROM.update(0, pos);
                            EEPROM.update(2,1);
                            maindrive();
                          }
                          else if(buttonStatee == LOW){
                                        if(x % 100 == 0){
                                          pos++;
                                            if(pos > 240){
                                              pos = 240;
                                              EEPROM.update(0, pos);
                                              maindrive();
                                            }
                                        }
                                    digitalWrite(stepPin,HIGH); 
                                    delayMicroseconds(600); 
                                    digitalWrite(stepPin,LOW); 
                                    delayMicroseconds(600);
                          }
          }
        }
    
}

void movebackward(){
   buttonStatee = LOW; buttonStateh = LOW; buttonState1 = LOW; buttonState2 = LOW; buttonState3 = LOW;
   digitalWrite(dirPin,HIGH); 
  for(int x = 1; x < 501; x++) { 
      int vol = analogRead(batteryPin);
          if(vol <= 1010){
            EEPROM.update(0, pos);
            EEPROM.update(2,1);
            Serial.println("Voltage drop detected.. stopping system. please reboot");
            maindrive();
          }
          else{
                  buttonStatee = digitalRead(buttone);
                        if(buttonStatee == HIGH){
                          delay(2000);
                          Serial.println(pos + "Estop detected: LOADING... maintenance mode" );
                          delay(2000);
                          Serial.println("Last saved Value: " + pos);
                          EEPROM.update(0, pos);
                          EEPROM.update(2,1);
                          maindrive();
                        }
                        else if(buttonStatee == LOW){
                                if(x % 100 == 0){
                                      pos--;
                                        if(pos < 0){
                                        pos = 0;
                                        EEPROM.update(0, pos);
                                        maindrive();
                                      }
                                    }
                            digitalWrite(stepPin,HIGH); 
                            delayMicroseconds(600); 
                            digitalWrite(stepPin,LOW); 
                            delayMicroseconds(600);
                        }
          }
    }
  
  if(pos < 0){
      pos = 0;
      EEPROM.update(0, pos);
      buttonStateh = LOW;
      maindrive();
    }
}


void maintenance_Mode(){
   Serial.println("Press any among buttonh ~ button3 to return to maindrive after loading... LOADING...");
   Serial.println("Or turn off device here to manually set wheelchair to lowest height ... LOADING");
   delay(1000);
   buttonStatee = LOW; buttonStateh = LOW; buttonState1 = LOW; buttonState2 = LOW; buttonState3 = LOW;
   delay(1000);
   Serial.println("READY for input");
    buttonStatee = LOW; buttonStateh = LOW; buttonState1 = LOW; buttonState2 = LOW; buttonState3 = LOW;
   while( buttonStateh == LOW && buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW ){
         buttonState1 = digitalRead(button1); buttonState2 = digitalRead(button2);  buttonState3 = digitalRead(button3);
         buttonStateh = digitalRead(buttonh); 
         
              if(buttonStateh == HIGH || buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH){
                                Serial.println("Maintenance Canceled... Returning to maindrive... LOADING...");
                                EEPROM.update(2,0);
                                delay(500);
                                EEPROM.update(2,0);
                                delay(500);
                                buttonStatee = LOW; buttonStateh = LOW; buttonState1 = LOW; buttonState2 = LOW; buttonState3 = LOW;
                                delay(500);
                                Serial.println("Loading Complete... Press reset button on Arduino to go back to height adjustment system");
                                maindrive();
                }
                
        }
        
}
   
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  pinMode(buttonh, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(buttone, INPUT);
  //EEPROM.write(0,0); // indicates position data
  //EEPROM.write(1,0); // indicates initial operation was finished
  //EEPROM.write(2,0); // indicates estop was pressed
}


/*  ※WARNING : For purposes of editing code for maintenance,do not edit beyond this point
 * 
 */

void loop() {
     if(EEPROM.read(2) == 1){
      maintenance_Mode();
     }
    
      pos = EEPROM.read(0);
      buttonStateh = digitalRead(buttonh);
      if(EEPROM.read(1) != 1){
        Serial.println("Ready for Activation");
          if(buttonStateh == HIGH){   
              Serial.println("Initial Operation Activated. Wheelchair Ascending");
                  for(int i = 0; i < 7; i++){
                    Serial.println("1");
                    forward_Start(); 
                  }
                  for(int i = 0; i < 1; i++){
                    forward_StartSmall();
                  }       
              buttonStateh = LOW;
              Serial.println(pos);
              pos = 0;
            EEPROM.update(0, pos);
            EEPROM.update(1, 1);
          }
          else if(buttonStateh == LOW){
            
          }
          else{
            
          }
    }
    
    else if(EEPROM.read(1) == 1){
      Serial.println(pos);
      maindrive();
    }
}

void maindrive(){
  if(EEPROM.read(2) == 1){
      maintenance_Mode();
     }
  Serial.println("<Maindrive> Select level");
  EEPROM.update(2,0);
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  buttonState3 = digitalRead(button3);
  buttonStateh = digitalRead(buttonh);
  /*
   * If buttonh is high state
   */
    if(buttonStateh == HIGH){
        if(pos != 0){
          
          while(pos > 0){
                movebackward();
              }
           EEPROM.update(0, pos);
      }
      else if(pos == 0){
          maindrive();
      }  
  }

    else if(buttonState1 == HIGH){
       if(pos < 80){
            while(pos < 80){
              moveforward();
              }
            EEPROM.update(0, pos);
            }
      else if(pos > 80){
        while(pos > 80){        
                  movebackward();
            }
            EEPROM.update(0, pos);
      }
      else if(pos == 80){
          maindrive();
      }  
  }
  
  /*
   * If button2 is high state
   */
    else if(buttonState2 == HIGH){
       if(pos < 160){
            while(pos < 160){
                   moveforward();
            }
            EEPROM.update(0, pos);
        }
      else if(pos > 160){
        while(pos > 160){
                  movebackward();
                }
            EEPROM.update(0, pos);
      }
      else if(pos == 160){
          maindrive();
      }  
  }
  /*
   * If button3 is high state
   */
   else if(buttonState3 == HIGH){
        if(pos != 240){
          while(pos < 240){
                moveforward();
          }
          EEPROM.update(0, pos);
      }  
}


}
