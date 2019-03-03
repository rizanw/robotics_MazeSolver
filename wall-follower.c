#include <NewPing.h>
#include <LiquidCrystal.h>

#define SONAR_NUM     2             //define jumlah ultrasonic sensor yang digunakan
#define MAX_DISTANCE 300            //Max distance between object and robot

 //define pin connections untuk sensor
 //sensor depan
#define echoPin1    11 //PB3 (pwm) 5 pd5
#define trigPin1    10 //PB2 (pwm) 6 pd6
//sensor samping
#define echoPin2    6 //PD6  (pwm) 11 pb3
#define trigPin2    5 //PD5 (pwm) 10 pb2

//input driver motor kiri
#define LM1      A0 //PB4 in1  //2 pd2
#define LM2      A1 //PB5 in2  //3 pd3
//input driver motor kanan
#define RM1      A2 //PB0 in3  //7 pd7
#define RM2      A3 //PD7 in4  //8 pb0

//inisialiasi awal
int rangeFront = 0;
int rangeWall  = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define a Newping array to measure the distance
NewPing sonar[SONAR_NUM] = {
    NewPing(trigPin1, echoPin1, MAX_DISTANCE),
    NewPing(trigPin2, echoPin2, MAX_DISTANCE)
};

void setup(){
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.print("START!");
    lcd.clear();

    pinMode(LM1, OUTPUT);
    pinMode(LM2, OUTPUT);
    pinMode(RM1, OUTPUT);
    pinMode(RM2, OUTPUT);

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    digitalWrite(trigPin1, LOW);

    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    digitalWrite(trigPin2, LOW);
}

//Initialze and define the values for distance between wall and robot
int toCloseWall = 1000; //def=1800           
int toFarWall = 1500; //def=2500
int toCloseFront = 1000; //def=1000

int cCMf;
int cCMs;

void loop(){
    Main:
    lcd.clear();
    
    //Read the sensor value dari function call
    rangeFront = readRangeFront();  
    //debug code
    cCMf = sonar[0].convert_cm(rangeFront);
    Serial.print(cCMf);
    Serial.print(" cm front");
    Serial.println();

    rangeWall = readRangeWall();
    //debug code
    cCMs = sonar[1].convert_cm(rangeWall);
    Serial.print(cCMs);
    Serial.print(" cm Wall");
    Serial.println();

    if (rangeFront < 400){ 
        rangeFront = 3000;
    }
    if(rangeWall < 400){
        rangeWall = 3000;
    }
    
    //kondisi untuk cek distance dari front and side sensor
    if(rangeWall >= toCloseWall && rangeWall <= toFarWall){ 
        drive_forward();
        Serial.print(" Drive Forward");
        Serial.println();        
        goto Main;
    }
    
    //Condition to check whether front sensor is close to robot
    if (rangeFront < toCloseFront){
        delay(100); 
        drive_backward();
        Serial.print(" Drive Back");
        delay(700);
        turn_left();
        Serial.print(" Turn left");
        turn_left();
        Serial.println(); 
        delay(1100)       
        goto Main;
    }

    //condition to check side wall is close to robot
    if (rangeWall < toCloseWall){ 
        delay(100);
        turn_left();
        Serial.print(" Turn Left");
        delay(500);
        drive_forward();
        Serial.print(" Drive Forward");
        Serial.println();
        goto Main;
    }

    //condition to check robot is far from side wall
    if (rangeWall > toFarWall){ 
        delay(100);
        turn_right();
        Serial.print(" Turn Right");
        delay(500);
        drive_forward();
        Serial.print(" Drive Forward");
        Serial.println();
        goto Main;
    } 
}

// function to stop the robot
void motor_stop(){
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
}

//function to drive the robot forward
void drive_forward(){   
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
}

//function to drive the robot backward
void drive_backward(){   
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
}

//function to turn the robot left
void turn_left(){  
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
}

//function to turn the robot right
void turn_right(){
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
}

//function to read the front sensor value
int readRangeFront(){
    delay(50);
    unsigned rangeFront = sonar[0].ping();
    sonar[0].timer_stop();
    return rangeFront;
}

// function to read the left sensor value
int readRangeWall(){
    delay(50);
    unsigned rangeWall = sonar[1].ping();
    sonar[1].timer_stop();
    return rangeWall;
}
