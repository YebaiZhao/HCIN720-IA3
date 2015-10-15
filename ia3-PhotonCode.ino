// This #include statement was automatically added by the Particle IDE.
#include "MMA8452Q.h"

// This #include statement was automatically added by the Particle IDE.
#include "MMA8452Q.h"
#define DELAY_OFFSET 11
/******************************************************************************
Modified from MMA8452Q_Basic.ino
*/

int button1;
int button2;
char publishString[128];
char publishStringA2[128];
String orient;
MMA8452Q accel1(0x1D);
//MMA8452Q accel2(0x1C);
unsigned long looptime=0;
unsigned long looptime2=0;

void beep(unsigned long hz, unsigned long ms);

void setup()
{
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2, OUTPUT);
  pinMode(D7, OUTPUT);
  Serial.begin(9600);
  Spark.function("led7",ledToggle);
  accel1.init(SCALE_2G, ODR_12);
}

void loop()
{
    
    button1=digitalRead(A0);
    button2=digitalRead(A1);
    //digitalWrite(D7,button1);
    if (looptime2+100 < millis()){ ///the local loop

        if (accel1.available()){
            accel1.read();
            printOrientationFor3D();
            //    Serial.println(); // Print new line every time.
            sprintf(publishString, "{\"Button1\": %u, \"Ori\": \"%s\", \"Button2\": %u, \"X\": %d, \"Y\": %d, \"Z\": %d}", button1, orient.c_str(), button2, accel1.x, accel1.y, accel1.z);
            //sprintf(publishString, "{\"Button1\": %u, \"Ori\": \"%s\", \"Button2\": %u}", button1, orient.c_str(), button2);
            Serial.println(publishString);
        }
        /*
       if (accel2.available()){
            accel2.read();
            printOrientationFor3D();
            //    Serial.println(); // Print new line every time.
            sprintf(publishStringA2, "{\"X\": %d, \"Y\": %d, \"Z\": %d}", accel2.x, accel2.y, accel2.z);
            //sprintf(publishString, "{\"Button1\": %u, \"Ori\": \"%s\", \"Button2\": %u}", button1, orient.c_str(), button2);
            Serial.println(publishStringA2);
        }
        */
        looptime2=millis();
    }    

    
    if (looptime+1000 < millis()){ ///the Cloud Loop

        Spark.publish("Data_2",publishString);
        looptime=millis();
    }
}


void printOrientationFor3D(){
    byte pl = accel1.readPL();
    
    switch (pl)
    {
    case PORTRAIT_U:
    //Serial.print("Portrait Up");
    orient="Up";
    //Spark.publish("Orientation","up",60);
    break;
    
    case PORTRAIT_D:
    //Serial.print("Portrait Down");
    orient="Down";
    //Spark.publish("Orientation","donw",60);
    break;
    
    case LANDSCAPE_R:
    //Serial.print("Landscape Right");
    orient="Right";
    //Spark.publish("Orientation","right",60);
    break;
    
    case LANDSCAPE_L:
    //Serial.print("Landscape Left");
    orient="Left";
    //Spark.publish("Orientation","left",60);
    break;
    
    case LOCKOUT:
    //Serial.print("Flat");
    orient="Flat";
    //Spark.publish("Orientation","flat",60);
    break;

  }
}

int ledToggle(String command) {
    unsigned long flashTimer=1024;
    
    beep(440, 550);
    
    if (command=="flash") {
        
        for( flashTimer; flashTimer>5; flashTimer=flashTimer/2 ){
                digitalWrite(D7,HIGH);
                delay(flashTimer/2);
                digitalWrite(D7,LOW);
                delay(flashTimer/2);
        }
        return 1;
    }
    
    
    else {
        return command.length();
    }
}

void beep(unsigned long hz, unsigned long ms) {
// reference: 440hz = 2273 usec/cycle, 1136 usec/half-cycle
    unsigned long us = (500000 / hz) - DELAY_OFFSET;
    unsigned long rep = (ms * 500L) / (us + DELAY_OFFSET);
    for (int i = 0; i < rep; i++) {
        digitalWrite(A2, HIGH);
        delayMicroseconds(us);
        digitalWrite(A2, LOW);
        delayMicroseconds(us);
    }
}
