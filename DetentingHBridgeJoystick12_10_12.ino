// parameters for total the joystick range:
int actXMin = 127;  //minimum detenting range
int actXMax = 395;  //maximum detenting range
int actYMin = 190;  //minimum detenting range
int actYMax = 450;  //maximum detenting range

// set pin numbers for switch, joystick axes, and LED:
const int FSRXpos = A2;         // pin A2 joystick A1 bottom  
const int FSRXneg = A3;         // pin A3 joystick A1 top
const int FSRYpos = A4;         // pin A4 joystick A2 bottom  
const int FSRYneg = A5;         // pin A5 joystick A2 top

// input/output pin declarations
const int motorXPin1 = 11; // pin 15 on L293D A2
const int motorXPin2 = 5; // pin 10 on L293D A2
const int motorXenable = 2; // pin 9 on L293D A2

const int motorYPin1 = 3; // pin 7 on L293D A1
const int motorYPin2 = 10; // pin 2 on L293D A1
const int motorYenable = 12; // pin 1 on L293D A1

const int actPosX = A0; // pin A0 actuator positioning
const int actPosY = A1; // pin A1 actuator positioning
const int footPedal = 8; // pin A8 foot peddle input

//////Experimental feedback features////// NOTE: 0 is OFF for all features

//feature 1
int timeDelayFeedbackValue = 2; // delay value between registering another detent change

//feature 2
int positionFeedbackValue = 0; // delay value for forcing actuators to stay in position
                               // Note: this value improves positioning accuracy

//feature 3
float overshootFeedbackValue = 0.1;  // overshoot length values (through 1.0 to 0.1)
int overshootFeedbackDelay = 0; // delay inbetween overshoot and correct actuator positioning
                                  // Note: needs to be relatively high otherwise skipping overshoot skewing occurs
                                  // a delay of 0 disables feature 3 completely
float undershootFeedbackValue = 0.1;  // undershoot length values (through 1.0 to 0.1)
int undershootFeedbackDelay = 200; // delay inbetween undershoot and correct actuator positioning
                                   // Note: needs to be relatively high otherwise skipping overshoot skewing occurs

//feature 4
float stressFeedbackValue = 125; // increases force to move joystick (through increments of 1)

//feature 5
bool ActiveFeedbackBool = false; // enables active position correction

//feature 6
float activeFeedbackValue = 0; // percent distance +/- between where actuators should be
                                 // moves joystick if actuators don't detect force threshold 
                                 // Note: must be positive values; 0 disables this feature
//////////////////////////////////////////

int range = 25;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range/4;      // resting threshold
int center = range/2;         // resting position value

int detents = 8;          // number of set detents
int detentiveXDis = (actXMax - actXMin)/detents;  //distance traveled between each detent
int detentiveYDis = (actYMax - actYMin)/detents;  //distance traveled between each detent
int posX = detents/2;                          //detent incrementer and position initializer for X
int posY = detents/2;                          //detent incrementer and position initializer for Y
int posXLast = posX;                         //last X position locater
int posYLast = posY;                         //last Y position locater
int footPedalCount = 0;                          //counter for multiple foot pedal input
int nextPosX = (posX * detentiveXDis) + actXMin;  //calculates next position from detent incrementer, "posX"
int nextPosY = (posY * detentiveYDis) + actYMin;  //calculates next position from detent incrementer, "posY"
int lastPosX = (posXLast * detentiveXDis) + actXMin;  //calculates last position from dentent incrementer, "posXLast"
int lastPosY = (posYLast * detentiveYDis) + actYMin;  //calculates last position from dentent incrementer, "posYLast"
int cursorandpedalDelay = 10;                             //delay for end of matrix cursor movement
int footPedalDelay = 300;                        //delay before registering another food pedal press

int endOfDetentForce = 25; // value for reducing servo range for larger detent force on last + or - position
float detentLength2 = 0.05;  // detent2 feedback length values (through 1.0 to 0.1)
int endOfDetentForce2 = 25; // value for reducing servo range for detent2 larger detent force on last + or - position
int A2XForceThresh = 100 + (1.0 * stressFeedbackValue) ;  // FSRX+ (FSR1) on pin A2 first force threshold value
//100
int A2XForceThresh2 = 250;  // FSRX+ on pin A2 second force threshold value
int A3XForceThresh = 5 + (.65 * stressFeedbackValue);  // FSRX- (FSR2) on pin A3 first force threshold value
//65
int A3XForceThresh2 = 300;  // FSRX- on pin A3 second force threshold value
int A4YForceThresh = 175 + (2.05 * stressFeedbackValue);  // FSRY+ (FSR3) on pin A4 first force threshold value
//205
int A4YForceThresh2 = 300;  // FSRY+ on pin A4 second force threshold value
int A5YForceThresh = 185 + (2.45 * stressFeedbackValue);  // FSRY- (FSR4) on pin A5 first force threshold value
//245
int A5YForceThresh2 = 300;  // FSRX- on pin A5 second force threshold value

boolean directionXBool = true; // determines direction of detent for +X and -X directions
boolean directionYBool = true; // determines direction of detent for +X and -X directions

boolean modCheck = false; // flag enables reading for particular redefinable serial programmable value
boolean letterCheckBool = false; // flag enables redefining serial programmable values
char letterCheck = 'Z'; // used for redefining serial reprogrammable variables
                    // initialized to 'Z' because Z is not used

int Xdirection = -1; // reprogrammable X direction value
int Ydirection = 1; // reprogrammable Y direction value

boolean keyboardStrokeEnable = false; // determines if keyboard directional keys used for navigation
boolean mouseEnable = true; // determines if mouse used for navigation

int lastSwitchState = LOW;        // previous switch state

bool initialize = false;  // sets joystick to start in center
bool continuePress = false; // determines if foot pedal has been released after code loop cycling
bool countBetweenClicks = false; // determines if additional clicks are within footPedalDelay
int clickCount = 0; // counts numbers of additional clicks within footPedalDelay
bool mousePressActive = false; // disables single click if waiting for double click
int clickType = 3; // determines click mode 0-4 options
                        // first option outputs left click with a delay of footPedalDelay if holding down pedal
                        // second option outputs continous left click until pedal is released
                        // third option outputs a single left click and waits until footPedalDelay and then
                        //  outputs a continous left click until the pedal is released
                        // forth option outputs a single left click and waits for the foot pedal to be released
                        //  and another downpress of the pedal in the lenght of footPedalDelay and then outputs
                        //  a continous left click until the pedal is released
                        // fifth option outputs a single left click and waits for the foot pedal to be released
                        //  and another downpress of the pedal in the lenght of footPedalDelay and then outputs
                        //  a continous left click until the pedal is released also this option allows for single
                        //  click untila nd waits until footPedalDelay and then outputs a continous left click 
                        //  until the pedal is released

void setup() 
{ 
  // Open serial connection, 9600 baud
  Serial.begin(9600);

  // take control of the mouse:
  Mouse.begin();

  // take control of the keyboard:
  Keyboard.begin();

  // set all the other pins you're using as outputs:
  pinMode(actPosX, INPUT);
  pinMode(actPosY, INPUT);
  pinMode(footPedal, INPUT);
  pinMode(motorXPin1, OUTPUT);
  pinMode(motorXPin2, OUTPUT);
  pinMode(motorYPin1, OUTPUT);
  pinMode(motorYPin2, OUTPUT);

  // set motorXenable & motorYenable high so that motor can turn on:
  digitalWrite(motorXenable, HIGH);
  digitalWrite(motorYenable, HIGH);
} 
 
int x=0;
 
void loop() 
{ 
    // checks for % serial reprogramming

//    Se
    if(Serial.available())
      if(Serial.read()=='%')
        modCheck = true;

    // checks for reprogrammable char value input after mod '%[LETTER]'
    if((modCheck)&&(Serial.available()))
    {
      char letterCheckTemp = Serial.read();
      if((letterCheckTemp=='D')||(letterCheckTemp=='S')||(letterCheckTemp=='P')
      ||(letterCheckTemp=='C')||(letterCheckTemp=='E')||(letterCheckTemp=='I')
      ||(letterCheckTemp=='K')||(letterCheckTemp=='X')||(letterCheckTemp=='X')
      ||(letterCheckTemp=='Y'))
      {
        letterCheckBool = true;
        letterCheck = letterCheckTemp;
      }
      else
      {
        modCheck = false;
        letterCheckBool = false;
      }
    } 
  
    // checks for reprogrammable char value input after number '%[LETTER][NUMBER]'
    if((modCheck)&&(letterCheckBool)&&(Serial.available()))
    {
      int numberCheck = Serial.read();
      if((numberCheck=='0')||(numberCheck=='1')||(numberCheck=='2')
      ||(numberCheck=='3')||(numberCheck=='4')||(numberCheck=='5')
      ||(numberCheck=='6')||(numberCheck=='7')||(numberCheck=='8')
      ||(numberCheck=='9'))
      {
        int newDetent, newStress, newPosition, newCursor, newKeyboard, newMouse,
          newPedal, newIteration, newX, newY = numberCheck;
        switch(letterCheck)
        {
          case 'D':
          // redefine detents through GUI
            newDetent = numberCheck;
            if((newDetent<=12)&&(newDetent>=1))
              detents = newDetent;
            ResetDetents();
            break;
          case 'S':
          // redefine stress value for FSR force registration
            newStress = numberCheck;
            if((newStress<=10)&&(newStress>=0))
              stressFeedbackValue = newStress;
            break;
          case 'P':
          // redefine position feedback
            newPosition = numberCheck;
            if((newPosition<=10)&&(newPosition>=0))
              positionFeedbackValue = newPosition;
            break;            
          case 'C':
          // redefine for cursor movement and pedal delay
            newCursor = numberCheck;
            if((newCursor<=10)&&(newCursor>=0))
              cursorandpedalDelay = newCursor * 10;
            break;
          case 'E':
          // redefine pedal delay
            newPedal = numberCheck;
            if((newPedal<=10)&&(newPedal>=0))
              footPedalDelay = newPedal * 40;
            break;
          case 'I':
          // redefine pedal click mode
            newIteration = numberCheck;
            if((newIteration==0)||(newIteration==1)||(newIteration==2))
              clickType = newIteration;
            break;
          case 'K':
          // redefine keyboard arrow key toggle
            newKeyboard = numberCheck;
            if((newKeyboard==0)||(newKeyboard==1))
              keyboardStrokeEnable = newKeyboard;
            if(newKeyboard==0)
              mouseEnable = true;
            break;
          case 'M':
          // redefine mouse toggle
            newMouse = numberCheck;
            if((newMouse==0)||(newMouse==1))
              mouseEnable = newMouse;
            if(newMouse==0)
              keyboardStrokeEnable = true;
            break;
          case 'X':
          // redefine X direction
            newX = numberCheck;
            if(newX==1)
              Xdirection = 1;
            if(newX==0)
              Xdirection = -1;
            break;
          case 'Y':
          // redefine Y direction
            newY = numberCheck;
            if(newY==1)
              Ydirection = 1;
            if(newY==0)
              Ydirection = -1;
            break;
          default:
            break;
        }
        modCheck = false;
        letterCheckBool = false;
        letterCheck = 'Z';
      }
    }
  
    //prevents FSR readings from moving joystick above/below defined joystick range
    if(posX < 0)
      posX = 0;
    if(posX > detents)
      posX = detents;

    if(posY < 0)
      posY = 0;
    if(posY > detents)
      posY = detents;
      
    //initializes joystick start position



    if(!initialize)
      InitializeStartPosition();


/*
    // maintains active feedback
    if(activeFeedbackValue != 0)
      ActiveFeedbackRange();
*/

    Serial.println("footPedal");
    Serial.println(digitalRead(footPedal));  


    // if FSR reading is above forcethreshold move actuators in one detent in +X direction
    if((analogRead(FSRXpos) >= A2XForceThresh)&&(posX <= detents)&&(posX < detents))    
    {
      directionXBool = true;
            // increment X position
            posXLast = posX;
            posX++;
            // define next position for actuator to take
            nextPosX = (posX * detentiveXDis) + actXMin;
            lastPosX = (posXLast * detentiveXDis) + actXMin;
//PrintValues();
              // if delay >0 enable overshoot feedback
              if(overshootFeedbackDelay!=0)
                overshootFeedbackX();  // envokes overshoot feedback
              // move actuator to next position
              PositionX();
            // delay feedback between detents
            delay(timeDelayFeedbackValue);
            // calculate mouse movement
            if(mouseEnable)
              MouseMovementX();     
    }
    if((analogRead(FSRXneg) >= A3XForceThresh)&&(posX >= 1))    
    {
      directionXBool = false;
            // increment X position
            posXLast = posX;
            posX--;
            // define next position for actuator to take
            nextPosX = (posX * detentiveXDis) + actXMin;
//            lastPosX = (posXLast * detentiveXDis) + actXMin;
//PrintValues();
              // if delay >0 enable overshoot feedback
//              if(overshootFeedbackDelay!=0)
//                overshootFeedbackX();  // envokes overshoot feedback
              // move actuator to next position
              PositionX();
            // delay feedback between detents
            delay(timeDelayFeedbackValue);
            // calculate mouse movement
            if(mouseEnable)
              MouseMovementX();     
    }
    // if FSR reading is above forcethreshold move actuators in one detent in +X direction
    if((analogRead(FSRYpos) >= A4YForceThresh)&&(posY <= detents)&&(posY < detents))    
    {
      directionYBool = true;
            // increment X position
            posYLast = posY;
            posY++;
            // define next position for actuator to take
            nextPosY = (posY * detentiveYDis) + actYMin;
//            lastPosY = (posYLast * detentiveYDis) + actYMin;
//PrintValues();
              // if delay >0 enable overshoot feedback
//              if(overshootFeedbackDelay!=0)
//                overshootFeedbackY();  // envokes overshoot feedback
              // move actuator to next position
              PositionY();
            // delay feedback between detents
            delay(timeDelayFeedbackValue);
            // calculate mouse movement
            if(mouseEnable)
               MouseMovementY();
    }
        
    // if FSR reading is above forcethreshold move actuators in one detent in -X direction
    if((analogRead(FSRYneg) >= A5YForceThresh)&&(posY >= 1))
    {
        // increment X position
        posYLast = posY;
        posY--;
        directionYBool = false;
            // define next position for actuator to take
            nextPosY = (posY * detentiveYDis) + actYMin;
//            lastPosY = (posYLast * detentiveYDis) + actYMin;
//PrintValues();
              // if delay >0 enable overshoot feedback
 //             if(overshootFeedbackDelay!=0)
 //               overshootFeedbackY();  // envokes overshoot feedback
              // move actuator to next position
              PositionY();
            delay(timeDelayFeedbackValue);
        // calculate mouse movement
        if(mouseEnable)
          MouseMovementY();        
    }
    footPedalClick();
}

void InitializeStartPosition()
{
    PositionX();
    PositionY();
    delay(100);
    initialize=1;
}

void ResetDetents()
{
    InitializeStartPosition();
    detentiveXDis = (actXMax - actXMin)/detents;  //distance traveled between each detent
    detentiveYDis = (actYMax - actYMin)/detents;  //distance traveled between each detent
    posX = detents/2;                          //detent incrementer and position initializer for X
    posY = detents/2;                          //detent incrementer and position initializer for Y
    nextPosX = (posX * detentiveXDis) + actXMin;  //calculates next position from detent incrementer, "posX"
    nextPosY = (posY * detentiveYDis) + actYMin;  //calculates next position from detent incrementer, "posY"
}

void PositionX()
// sets previously defined positioning 
{
    // calculate position holding feedback
    for(int i=-1; i<positionFeedbackValue; i++)
    {
      if(directionXBool == true)
      {
        // account for positive distance and adjust until actual position is equal or greater than desired position 
        PositionXPos();
        // account for negative distance and adjust until actual position is equal or less than desired position 
        if((analogRead(FSRXpos) <= A2XForceThresh)) 
        PositionXNeg();
      }
      if(directionXBool == false)
      // note: this if c is for the other direction, this reduces the jerkiness of the actuator finding its position since it is operating in the opposite direction
      {
        x++;
        // account for negative distance and adjust until actual position is equal or less than desired position 
        PositionXNeg();
        // account for positive distance and adjust until actual position is equal or greater than desired position 
        if((analogRead(FSRXneg) <= A3XForceThresh)) 
          PositionXPos();
      }
//PrintValues();
    }
    digitalWrite(motorXPin1, LOW); // set pin 2 on L293D low
    digitalWrite(motorXPin2, LOW); // set pin 6 on L293D low
    // setting both pins to low ceases actuator movement
//PrintValues();
}

void PositionY()
// sets previously defined positioning 
{
    // calculate position holding feedback
    for(int i=-1; i<positionFeedbackValue; i++)
    {
      if(directionYBool == true)
      {
        // account for positive distance and adjust until actual position is equal or greater than desired position 
        PositionYPos();
        // account for negative distance and adjust until actual position is equal or less than desired position 
        if((analogRead(FSRYpos) <= A4YForceThresh))    
          PositionYNeg();
      }
      if(directionYBool == false)
      // note: this if c is for the other direction, this reduces the jerkiness of the actuator finding its position since it is operating in the opposite direction
      {
        // account for negative distance and adjust until actual position is equal or less than desired position 
        PositionYNeg();
        // account for positive distance and adjust until actual position is equal or greater than desired position 
        if((analogRead(FSRYneg) <= A5YForceThresh))
          PositionYPos();
      }
//PrintValues();
    }
    digitalWrite(motorYPin1, LOW); // set pin 2 on L293D low
    digitalWrite(motorYPin2, LOW); // set pin 6 on L293D low
    // setting both pins to low ceases actuator movement
//PrintValues();
}

void PositionXPos()
{
    while(nextPosX <= analogRead(actPosX))
    // while the calculated position is less than or not equal to actual position increase stroke length
    {
          digitalWrite(motorXPin1, HIGH); // set pin 2 on L293D high
          digitalWrite(motorXPin2, LOW); // set pin 6 on L293D low
    }
//PrintValues();
    while(nextPosX >= analogRead(actPosX))
    // while the calculated position is greater than or not equal to actual position decrease stroke length
    {
          digitalWrite(motorXPin1, LOW); // set pin 2 on L293D low
          digitalWrite(motorXPin2, HIGH); // set pin 6 on L293D high
    }
}

void PositionXNeg()
{
    while(nextPosX >= analogRead(actPosX))
    // while the calculated position is greater than or not equal to actual position decrease stroke length
    {
          digitalWrite(motorXPin1, LOW); // set pin 2 on L293D low
          digitalWrite(motorXPin2, HIGH); // set pin 6 on L293D high
    }
//PrintValues();
    while(nextPosX <= analogRead(actPosX))
    // while the calculated position is less than or not equal to actual position increase stroke length
    {
          digitalWrite(motorXPin1, HIGH); // set pin 2 on L293D high
          digitalWrite(motorXPin2, LOW); // set pin 6 on L293D low
    }
//PrintValues();
}

void PositionYPos()
{
    while(nextPosY <= analogRead(actPosY))
    // while the calculated position is less than or not equal to actual position increase stroke length
    {
          digitalWrite(motorYPin1, HIGH); // set pin 2 on L293D high
          digitalWrite(motorYPin2, LOW); // set pin 6 on L293D low
    }
//PrintValues();
    while(nextPosY >= analogRead(actPosY))
    // while the calculated position is greater than or not equal to actual position decrease stroke length
    {
          digitalWrite(motorYPin1, LOW); // set pin 2 on L293D low
          digitalWrite(motorYPin2, HIGH); // set pin 6 on L293D high
    }
}

void PositionYNeg()
{
    while(nextPosY >= analogRead(actPosY))
    // while the calculated position is greater than or not equal to actual position decrease stroke length
    {
          digitalWrite(motorYPin1, LOW); // set pin 2 on L293D low
          digitalWrite(motorYPin2, HIGH); // set pin 6 on L293D high
    }
//PrintValues();
    while(nextPosY <= analogRead(actPosY))
    // while the calculated position is less than or not equal to actual position increase stroke length
    {
          digitalWrite(motorYPin1, HIGH); // set pin 2 on L293D high
          digitalWrite(motorYPin2, LOW); // set pin 6 on L293D low
    }
//PrintValues();
}

void PrintValues()
// debugging serial monitor statements
{
    Serial.println("detentiveXDis");
    Serial.println(detentiveXDis);
  
    Serial.println("nextPosX");
    Serial.println(nextPosX);  
  
    Serial.println("actPosX");
    Serial.println(analogRead(actPosX));  
    
    Serial.println("posX");
    Serial.println(posX);

    Serial.println("detentiveYDis");
    Serial.println(detentiveYDis);

    Serial.println("nextPosY");
    Serial.println(nextPosY);    

    Serial.println("actPosY");
    Serial.println(analogRead(actPosY));
    
    Serial.println("posY");
    Serial.println(posY);
 }

void overshootFeedbackX()
// calculates overshoot for next positioning
{
  
  /*
  
    float ratioforward, ratioback;
    if (directionXBool)
    // calculates over and under shoot percentile for feedback in positive direction
      {
      ratioforward = 1 + overshootFeedbackValue;
      ratioback = 1 - undershootFeedbackValue;
      }
    if (!directionXBool)
    // calculates over and under shoot percentile for feedback in negative direction
      {
      ratioforward = 1 - overshootFeedbackValue;
      ratioback = 1 + undershootFeedbackValue;
      }
    int actualPos = nextPosX; // save next position to restore to after overshoot/undershoot
    nextPosX = (detentiveXDis * posX * ratioforward) + actXMin; // calculate oversshoot
//    if((nextPosX > actXMin) && (nextPosX < actXMax)) // if within joystick stroke range execute overshoot
      PositionX();
    delay(overshootFeedbackDelay); // delay between overshoot and next position

    nextPosX = (detentiveXDis * posX * ratioback) + actXMin; // calculate undershoot
//    if((nextPosX > actXMin) && (nextPosX < actXMax)) // if within joystick stroke range execute overshoot
      PositionX();
    nextPosX = actualPos; // restore next position in variable 
    delay(undershootFeedbackDelay); // delay between undershoot and next position

*/

}

void overshootFeedbackY()
// calculates overshoot for next positioning
{

/*

  float ratioforward, ratioback;
    if (directionYBool)
    // calculates over and under shoot percentile for feedback in positive direction
      {
      ratioforward = 1 + overshootFeedbackValue;
      ratioback = 1 - undershootFeedbackValue;
      }
    if (!directionYBool)
    // calculates over and under shoot percentile for feedback in negative direction
      {
      ratioforward = 1 - overshootFeedbackValue;
      ratioback = 1 + undershootFeedbackValue;
      }
    int actualPos = nextPosY; // save next position to restore to after overshoot/undershoot
    nextPosY = (detentiveYDis * posY * ratioforward) + actYMin; // calculate oversshoot
//    if((nextPosX > actXMin) && (nextPosX < actXMax)) // if within joystick stroke range execute overshoot
      PositionY();
    delay(overshootFeedbackDelay); // delay between overshoot and next position

    nextPosY = (detentiveYDis * posY * ratioback) + actYMin; // calculate undershoot
//    if((nextPosX > actXMin) && (nextPosX < actXMax)) // if within joystick stroke range execute overshoot
      PositionY();
    nextPosY = actualPos; // restore next position in variable 
    delay(undershootFeedbackDelay); // delay between undershoot and next position

*/

}

void ActiveFeedbackRange()
{
/*
  if((analogRead(actPosX) >= lastPosX * (1 + activeFeedbackValue))
    ||(analogRead(actPosX) <= lastPosX * (1 - activeFeedbackValue)))
    {
    Serial.println("analogRead(actPosX)");
    Serial.println(analogRead(actPosX));
    Serial.println("lastPosX with activeFeedbackValue");
    Serial.println(lastPosX * (1 + activeFeedbackValue));
//      ActiveFeedbackX();
    }
*/
/*    if((analogRead(actPosY) >= lastPosY * activeFeedbackValue)
    ||(analogRead(actPosY) <= lastPosY * (1 - activeFeedbackValue)))
    {
      ActiveFeedbackY();
    }
*/
}

void ActiveFeedbackX()
{
/*  
    while((nextPosX <= analogRead(actPosX))
	&&((analogRead(FSRXpos) <= A2XForceThresh))
	&&((analogRead(FSRXneg) <= A3XForceThresh)&&(posX <= 10)))
    // while the calculated position is less than or not equal to actual position increase stroke length
    {
          digitalWrite(motorXPin1, HIGH); // set pin 2 on L293D high
          digitalWrite(motorXPin2, LOW); // set pin 6 on L293D low
    }
//PrintValues();
    while(nextPosX >= analogRead(actPosX)
	&&((analogRead(FSRXpos) <= A2XForceThresh))
	&&((analogRead(FSRXneg) <= A3XForceThresh)&&(posX >= 1)))
    // while the calculated position is greater than or not equal to actual position decrease stroke length
    {
          digitalWrite(motorXPin1, LOW); // set pin 2 on L293D low
          digitalWrite(motorXPin2, HIGH); // set pin 6 on L293D high
    }
*/
}

void ActiveFeedbackY()
{
/*
  while((nextPosY >= analogRead(actPosY))
	&&((analogRead(FSRYpos) <= A4YForceThresh))
	&&((analogRead(FSRYneg) <= A5YForceThresh)&&(posY >= 1)))
    // while the calculated position is greater than or not equal to actual position decrease stroke length
    {
          digitalWrite(motorYPin1, LOW); // set pin 2 on L293D low
          digitalWrite(motorYPin2, HIGH); // set pin 6 on L293D high
    }
    while((nextPosY <= analogRead(actPosY))
	&&((analogRead(FSRYpos) <= A4YForceThresh))
	&&((analogRead(FSRYneg) <= A5YForceThresh)&&(posY <= 10)))
    // while the calculated position is less than or not equal to actual position increase stroke length
    {
          digitalWrite(motorYPin1, HIGH); // set pin 2 on L293D high
          digitalWrite(motorYPin2, LOW); // set pin 6 on L293D low
    }
*/
}

void MouseMovementX()
// calculates movement for cursor on screen
{
    if (directionXBool)
    {
      // cursor movement for +X value
      Mouse.move(map(nextPosX * Xdirection, 0, 672, 0, range), 0 , 0); //-xReading    
//      Mouse.move(Xdirection, 0 , 0); //xReading
      KeyboardXCalc();
      DetentEndXMovement();
    }  
    if (!directionXBool)
    {
      // cursor movement for -X value
      Mouse.move(map(-nextPosX * Xdirection, 0, 672, 0, range), 0 , 0); //-xReading    
//      Mouse.move(-Xdirection, 0, 0); //-xReading
      KeyboardXCalc();
      DetentEndXMovement(); 
//      Mouse.move(-map(nextPosX, 0, 672, 0, range), 0 , 0); //-xReading    
    }  
}

void MouseMovementY()
// calculates movement for cursor on screen
{
    if (directionYBool)
    {
      // cursor movement for +Y value
      Mouse.move(0, map(nextPosY * Ydirection, 0, 672, 0, range), 0); //-xReading    
//      Mouse.move(0, Ydirection, 0); //-yReading
      KeyboardXCalc();
      DetentEndYMovement();
    }  
    if (!directionYBool)
    {
      // cursor movement for -Y value
      Mouse.move(0, map(-nextPosY * Ydirection, 0, 672, 0, range), 0); //-xReading    
//      Mouse.move(0, -Ydirection, 0); //yReading
      KeyboardXCalc();
      DetentEndYMovement();
    }  
}

void DetentEndXMovement()
{
//Movement to corners from a centeral detent are below
/*
    while ((posX == detents)&&(posY == detents)
      &&(analogRead(FSRXneg) <= A3XForceThresh)
      &&(analogRead(FSRYneg) <= A5YForceThresh))
    {
      Mouse.move(Xdirection, Ydirection, 0); //-yReading
      KeyboardXCalc();
      KeyboardYCalc();
      footPedalClick();
    }

    while ((posX == detents)&&(posY == 0)
      &&(analogRead(FSRXneg) <= A3XForceThresh)
      &&(analogRead(FSRYpos) <= A4YForceThresh))
    {
      Mouse.move(Xdirection, -Ydirection, 0); //-yReading
      KeyboardXCalc();
      KeyboardYCalc();
      footPedalClick();
    }

    while ((posX == 0)&&(posY == detents)
      &&(analogRead(FSRXpos) <= A2XForceThresh)
      &&(analogRead(FSRYneg) <= A5YForceThresh))
    {
      Mouse.move(-Xdirection, Ydirection, 0); //-yReading
      KeyboardXCalc();
      KeyboardYCalc();
      footPedalClick();
    }
    while ((posX == 0)&&(posY == 0)
      &&(analogRead(FSRXpos) <= A2XForceThresh)
      &&(analogRead(FSRYpos) <= A4YForceThresh))
    {
      Mouse.move(-Xdirection, -Ydirection, 0); //-yReading
      KeyboardXCalc();
      KeyboardYCalc();
      footPedalClick();
    }
*/
  
//Movement for four sides && movement from side to a corner are  below 
  while ((posX == detents)&&(analogRead(FSRXneg) <= A3XForceThresh)
     &&(analogRead(FSRYpos) <= A4YForceThresh)
     &&(analogRead(FSRYneg) <= A5YForceThresh))
   {
     Mouse.move(Xdirection, 0, 0); //-yReading
     KeyboardXCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posX == detents)&&(posY == detents)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posX == detents)&&(posY == 0)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }

   }

   while ((posX == 0)&&(analogRead(FSRXpos) <= A2XForceThresh)
     &&(analogRead(FSRYpos) <= A4YForceThresh)
     &&(analogRead(FSRYneg) <= A5YForceThresh))
   {
     Mouse.move(-Xdirection, 0, 0); //-yReading
     KeyboardXCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posX == 0)&&(posY == detents)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(-Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posX == 0)&&(posY == 0)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(-Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }

   while ((posY == detents)&&(analogRead(FSRYneg) <= A5YForceThresh)
     &&(analogRead(FSRXpos) <= A2XForceThresh)
     &&(analogRead(FSRXneg) <= A3XForceThresh))
   {
     Mouse.move(0, Ydirection, 0); //-yReading
     KeyboardYCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posY == detents)&&(posX == detents)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posY == detents)&&(posX == 0)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(-Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }

   while ((posY == 0)&&(analogRead(FSRYpos) <= A4YForceThresh)
     &&(analogRead(FSRXpos) <= A2XForceThresh)
     &&(analogRead(FSRXneg) <= A3XForceThresh))
   {
     Mouse.move(0, -Ydirection, 0); //-yReading
     KeyboardYCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posY == 0)&&(posX == detents)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(-Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posY == 0)&&(posX == 0)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }
}

void DetentEndYMovement()
{
  while ((posY == detents)&&(analogRead(FSRYneg) <= A5YForceThresh)
     &&(analogRead(FSRXpos) <= A2XForceThresh)
     &&(analogRead(FSRXneg) <= A3XForceThresh))
   {
     Mouse.move(0, Ydirection, 0); //-yReading
     KeyboardYCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posY == detents)&&(posX == detents)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posY == detents)&&(posX == 0)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(-Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }

   while ((posY == 0)&&(analogRead(FSRYpos) <= A4YForceThresh)
     &&(analogRead(FSRXpos) <= A2XForceThresh)
     &&(analogRead(FSRXneg) <= A3XForceThresh))
   {
     Mouse.move(0, -Ydirection, 0); //-yReading
     KeyboardYCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posY == 0)&&(posX == detents)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posY == 0)&&(posX == 0)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(-Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }  
  
   while ((posX == detents)&&(analogRead(FSRXneg) <= A3XForceThresh)
     &&(analogRead(FSRYpos) <= A4YForceThresh)
     &&(analogRead(FSRYneg) <= A5YForceThresh))
   {
     Mouse.move(Xdirection, 0, 0); //-yReading
     KeyboardXCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posX == detents)&&(posY == detents)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posX == detents)&&(posY == 0)
       &&(analogRead(FSRXneg) <= A3XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }

   while ((posX == 0)&&(analogRead(FSRXpos) <= A2XForceThresh)
     &&(analogRead(FSRYpos) <= A4YForceThresh)
     &&(analogRead(FSRYneg) <= A5YForceThresh))
   {
     Mouse.move(-Xdirection, 0, 0); //-yReading
     KeyboardXCalc();
     footPedalClick();
     delay(cursorandpedalDelay);
     while ((posX == 0)&&(posY == detents)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYneg) <= A5YForceThresh))
     {
       Mouse.move(-Xdirection, Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
     while ((posX == 0)&&(posY == 0)
       &&(analogRead(FSRXpos) <= A2XForceThresh)
       &&(analogRead(FSRYpos) <= A4YForceThresh))
     {
       Mouse.move(-Xdirection, -Ydirection, 0); //-yReading
       KeyboardXCalc();
       KeyboardYCalc();
       footPedalClick();
     }
   }
}

int MouseXCalc()
{
    int currentMouseValue = map(nextPosX, 0, 672, 0, range);
    if (currentMouseValue > 0)
    {
      return 1;
    }
    if (currentMouseValue < 0)
    {
      return -1;
    }
}

int MouseYCalc()
{
    int currentMouseValue = map(nextPosY, 0, 672, 0, range);
    if (currentMouseValue>0)
    {
      return 1;
    }
    if (currentMouseValue<0)
    {
      return -1;
    }
}

int KeyboardXCalc()
{
    if(keyboardStrokeEnable)
    {
      if(directionXBool)
      {
        if(Xdirection==1)
        {
          Keyboard.press(KEY_RIGHT_ARROW);
          delay(100);
          Keyboard.release(KEY_RIGHT_ARROW);
        }
        if(Xdirection==-1)
        {
          Keyboard.press(KEY_LEFT_ARROW);
          delay(100);
          Keyboard.release(KEY_LEFT_ARROW);
        }
      }
      if(!directionXBool)
      {
        if(Xdirection==1)
        {
          Keyboard.press(KEY_LEFT_ARROW);
          delay(100);
          Keyboard.release(KEY_LEFT_ARROW);
        }
        if(Xdirection==-1)
        {
          Keyboard.press(KEY_RIGHT_ARROW);
          delay(100);
          Keyboard.release(KEY_RIGHT_ARROW);
        }
      }
    }
}

int KeyboardYCalc()
{
    if(keyboardStrokeEnable)
    {
      if(directionYBool)
      {
        if(Ydirection==1)
        {
          Keyboard.press(KEY_DOWN_ARROW);
          delay(100);
          Keyboard.release(KEY_DOWN_ARROW);
        }
        if(Ydirection==-1)
        {
          Keyboard.press(KEY_UP_ARROW);
          delay(100);
          Keyboard.release(KEY_UP_ARROW);
        }
      }
      if(!directionYBool)
      {
        if(Ydirection==1)
        {
          Keyboard.press(KEY_UP_ARROW);
          delay(100);
          Keyboard.release(KEY_UP_ARROW);
        }
        if(Ydirection==-1)
        {
          Keyboard.press(KEY_DOWN_ARROW);
          delay(100);
          Keyboard.release(KEY_DOWN_ARROW);
        }
      }
    }
}

void footPedalClick()
{
/*    if(clickType == 0)
    {
      if((digitalRead(footPedal) == HIGH)&&(!continuePress))
      {
        Mouse.click(MOUSE_LEFT);
        continuePress = true;
      }
      if(digitalRead(footPedal) == LOW)
      {
        continuePress = false;
      }
      if((footPedalDelay == footPedalCount)&&(footPedalDelay !=0))
      {
        continuePress = false;
        footPedalCount = 0;
      }
      footPedalCount++;
    }
    if(clickType == 1)
    {
      if((digitalRead(footPedal) == HIGH)&&(!continuePress))
      {
        Mouse.press(MOUSE_LEFT);
      }
      if(digitalRead(footPedal) == LOW)
      {
        Mouse.release(MOUSE_LEFT);
      }
    }
    if(clickType == 2)
    {
      if((digitalRead(footPedal) == HIGH)&&(!continuePress))
      {
        Mouse.click(MOUSE_LEFT);
        continuePress = true;
      }
      if(digitalRead(footPedal) == LOW)
      {
        Mouse.release(MOUSE_LEFT);
        continuePress = false;
      }
      if((digitalRead(footPedal) == HIGH)&&(continuePress))
      {
        footPedalCount++;
        if(footPedalDelay == footPedalCount)
          Mouse.press(MOUSE_LEFT);
      }
    }

    if(clickType == 3)
    {
      if((clickCount>0)&&(digitalRead(footPedal) == HIGH))
      {
        Mouse.press(MOUSE_LEFT);
        clickCount=0;
        footPedalCount=0;
        countBetweenClicks = false;
        continuePress = false;
        mousePressActive = true;
      }
      if((digitalRead(footPedal) == HIGH)&&(!continuePress)&&(mousePressActive == false))
      {
        Mouse.click(MOUSE_LEFT);
        continuePress = true;
        countBetweenClicks = true;
      }
      if(digitalRead(footPedal) == LOW)
      {
        Mouse.release(MOUSE_LEFT);
        if(continuePress == true)
          clickCount++;
        continuePress = false;
        mousePressActive = false;
      }
      if(countBetweenClicks)
      {
        footPedalCount++;
      }
      if((footPedalCount >= footPedalDelay)&&(digitalRead(footPedal) == LOW))
      {
        clickCount=0;
        footPedalCount=0;
        countBetweenClicks = false;
      }
    }
    if(clickType == 4)
    {
      if((clickCount>0)&&(digitalRead(footPedal) == HIGH))
      {
        Mouse.press(MOUSE_LEFT);
        clickCount=0;
        footPedalCount=0;
        countBetweenClicks = false;
        continuePress = false;
        mousePressActive = true;
      }
      if((digitalRead(footPedal) == HIGH)&&(!continuePress)&&(mousePressActive == false))
      {
        Mouse.click(MOUSE_LEFT);
        continuePress = true;
        countBetweenClicks = true;
      }
      if(digitalRead(footPedal) == LOW)
      {
        Mouse.release(MOUSE_LEFT);
        if(continuePress == true)
          clickCount++;
        continuePress = false;
        mousePressActive = false;
      }
      if(countBetweenClicks)
      {
        footPedalCount++;
      }
      if((footPedalCount >= footPedalDelay)&&(digitalRead(footPedal) == LOW))
      {
        clickCount=0;
        footPedalCount=0;
        countBetweenClicks = false;
      }
      if((digitalRead(footPedal) == HIGH)&&(footPedalCount == footPedalDelay))
      {
        Mouse.press(MOUSE_LEFT);
        clickCount=0;
        footPedalCount=0;
        countBetweenClicks = false;
        continuePress = false;
        mousePressActive = true;
      }
    }
*/
}


void footPedalClickIteration()
{
}
