/*Camila and Dan's Barbie car alarm project January 31, 2016

  Background: Barbie had a really nice convertible sports car from her boyfriend Max Steel,
  but every time she would park it at the beach Ken and his buddies would be messing around
  with her car. She decided that it was time she had a car alarm installed so that if Ken
  or anyone messed with her car, EVERYONE would be looking at him, and the Cops would 
  hopefully arrest him as well. 
  
  My Daughter and I thought we could help Barbie with her problem.
  
  The scope of this project involved all the things an actual car alarm would do. 
    1) Turn alarm system on and off with a remote
    1.1) Beep when the alarm is turned on or off
    2) Alarm when on and the vehicle is bumped
    3) Run the alarm for a short time before turning it off (unless bumped again)
    4) Override for the alarm if accidentally bumped (Alarm system off stops alarm)
  
  We used:
    passiver buzzer  (+ - S)**
    tilt switch (tried ball, but mercury worked better) (+ - S) w/LED
    button(for tests) (+ S and 10k pull-down)
    IR detector   (+ - S) w/LED
    remote control
    5 AA batteries fed in to GND and VIN pins on sensor board (You could hook to DC port as well)
          !!! Do NOT use batteries and your USB for power !!! 
     ** Note: (+ - S) means that the sensor required + (Voltage) - (Ground) and S (Sense Pin)
  
  Our IR Transmitter is the MakeIt:Robot Remote Control IR transmitter from Radio Shack But with a little testing
  you could use any IR transmitter e.g. "extra" buttons on the TV remote. (I promise Barbie won't care.)

  This is a great reference for the IR receivers: https://arduino-info.wikispaces.com/IR-RemoteControl
  
  Some of these parts were from the "37 Sensor Kit" available from various places online 
  We also used an "Arduino sensor shield v4.0" break-out board to make connections easier for the sensors

  TO DO: Add a two color blinking LED to show when alarm is armed (RED) and not (GREEN)
 
  Written for and tested on Arduino Uno  1.6.4 
*/


#include <NewTone.h>    // We used Tim Eckel's NewTone library, because Tone timer interferes with IRremote timer
                        //  from https://bitbucket.org/teckel12/arduino-new-tone/wiki/Home
#include <IRremote.h>   // IRremote library from http://z3t0.github.io/Arduino-IRremote/

int receivepin = 11;    // set the IR receive pin number
int     buzzer = 8 ;    // set the buzzer pin number
int  buttonpin = 5 ;    // set the button pin number
int  wigglepin = 6 ;    // set the wiggle sensor pin number
int      alarm = 0 ;    // alarm on and off status
int     wiggle = 0 ;    // alarm wiggle trigger status

IRrecv irrecv(receivepin);   // initialize the IR receiver
decode_results results;

void setup ()
{
  Serial.begin(57600);             // set the serial port for text notifications of status while testing 

  pinMode (buttonpin, INPUT);      // set the button pin to input
  pinMode (wigglepin, INPUT);      // set the wiggle pin to INPUT
  pinMode (buzzer, OUTPUT)  ;      // set the buzzer pin to OUTPUT

  irrecv.enableIRIn(); // Start the IR receiver

  alarm=0;   // Alarm Off
  wiggle=0;  // Wiggle Sensor Off
  
  Serial.println("I Started!!!");
}
void loop (){
 
  if (irrecv.decode(&results)) 
    {                                                        // Turn the alarm OFF
    if((String(results.value,HEX)=="962814ca") && (alarm)){  // Check is OFF button and alarm is ON
     Serial.println("Alarm Off");
     beep();
     alarm=0;
     delay(1000);
    }
   else
    {                                                        // Turn the alarm ON
    if((String(results.value,HEX)=="8d2a4baf") && !(alarm)){ // Check is ON button and alarm is OFF
     Serial.println("Alarm On ");
     beep();
     alarm=1;
     delay(1000);
        }
     }      
    irrecv.resume(); // Receive the next value
    }  
  
  
 
//  IF YOU WANT TO DO THIS WITHOUT IR, CONNECT A BUTTON TO buttonpin AND USE CODE THIS INSTEAD.
//     if (digitalRead(buttonpin)) {                 //do this if the test button is pushed
//     Serial.print("Button Pressed ");
//     beep();
//     onoff();
//     delay(1000);
//  }
//else                                          //otherwise do this if the button is not pressed
  {                                             // if the else is commented out then we're going to do this regardless:
    if ((alarm) && (digitalRead(wigglepin))){   //if your alarm is turned on and there is wiggling then do this
    Serial.println("Wiggle!!!");
        wiggle=1;                               //start the wiggle counter
        siren();                                // OH NO KEN IS MESSING WITH THE CAR! TURN ON THE SIREN!!!
       } 
    else                                        //otherwise if your alarm is turned and not wiggling then do this
       {
         if ((alarm) && (0 < wiggle) && (wiggle < 10)) {
              wiggle=wiggle+1;                  //increment the wiggle counter because we aren't wiggling but we are alarming
              siren();                          //if the siren hasn't gone off 10 times do it again!!! GO AWAY KEN!
         }
       }
     }     
}  // end loop
   //-------------------------------------------------------------------------
   // begin functions

void siren(){                                  // this is the siren routine rising beep tone 
for (int i=500 ; i<3000; i++) {
  NewTone(buzzer, i, 10);
} 
  delay(200);                                  // wait 200ms before we do this again...
}



void beep(){                                   // this is the beeper routint for double beep
  NewTone(buzzer, 800, 150);                   // sometimes its easier for a 9 year old to understand/write
  delay(200);                                  // two lines of code that do the same thing
  NewTone(buzzer, 800, 150);
 }



void onoff(){                                  // ALARM TOGGLE ROUTINE
  alarm=!(alarm);                              // if the alarm is on- turn it off. And if it is off- turn it on
  if (alarm) {
    Serial.println("Alarm On");
    wiggle=0;
  }
  else
  {
    Serial.println("Alarm Off");
    wiggle=0;
  }  
}  //  end functions
   // ------------------------------------
