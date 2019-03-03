#include <NewPing.h>

#define SONAR_NUM     2
#define MAX_DISTANCE 300

#define echoPin1    11 //B3
#define trigPin1    10 //B2
#define echoPin2    6 //D6
#define trigPin2    5 //D5

#define LM1      A0 //C0
#define LM2      A1 //C1
#define RM1      A2 //C2
#define RM2      A3 //C3

int rangeFront = 0;
int rangeWall  = 0;

NewPing sonar[SONAR_NUM] = {
    NewPing(trigPin1, echoPin1, MAX_DISTANCE),
    NewPing(trigPin2, echoPin2, MAX_DISTANCE)
};

void setup(){
    Serial.begin(9600);

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    digitalWrite(trigPin1, LOW);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    digitalWrite(trigPin2, LOW);

    pinMode(LM1, OUTPUT);
    pinMode(LM2, OUTPUT);
    pinMode(RM1, OUTPUT);
    pinMode(RM2, OUTPUT);
}

int toCloseWall = 480;           
int toFarWall = 1000;
int toCloseFront = 700;

int cCMf;
int cCMs;

void loop(){
    Main:
    
    rangeFront = readRangeFront();  
    cCMf = sonar[0].convert_cm(rangeFront);
    Serial.print(rangeFront);
    Serial.print("ms ");
    Serial.print(cCMf);
    Serial.print("cm front");
    Serial.println();

    rangeWall = readRangeWall();
    cCMs = sonar[1].convert_cm(rangeWall);
    Serial.print(rangeWall);
    Serial.print("ms ");
    Serial.print(cCMs);
    Serial.print("cm Wall");
    Serial.println();

    if (rangeFront < 400){ 
        rangeFront = 3000;
    }
    if(rangeWall < 400){
        rangeWall = 3000;
    }
    
    if (rangeFront < toCloseFront){
        drive_backward();
        Serial.print(" Drive Back");
        delay(600);
        turn_left();
        Serial.print(" Turn left");
        turn_left();
        Serial.println(); 
        delay(350);    
        goto Main;
    }

    if(rangeWall > toCloseWall && rangeWall < toFarWall){ 
        drive_forward();
        delay(50);
        Serial.print(" Drive Forward");
        Serial.println();        
        goto Main;
    }

    if (rangeWall < toCloseWall){ 
        turn_left();
        Serial.print(" Turn Left");
        delay(100);
        drive_forward();
        Serial.print(" Drive Forward");
        Serial.println();
        goto Main;
    }

    if (rangeWall > toFarWall){ 
        turn_right();
        Serial.print(" Turn Right");
        delay(100);
        drive_forward();
        Serial.print(" Drive Forward");
        Serial.println();
        goto Main;
    } 
}

void motor_stop(){
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
}

void drive_forward(){   
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
}

void drive_backward(){   
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
}

void turn_left(){  
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
}

void turn_right(){
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
}

int readRangeFront(){
    delay(50);
    unsigned rangeFront = sonar[0].ping();
    sonar[0].timer_stop();
    return rangeFront;
}

int readRangeWall(){
    delay(50);
    unsigned rangeWall = sonar[1].ping();
    sonar[1].timer_stop();
    return rangeWall;
}