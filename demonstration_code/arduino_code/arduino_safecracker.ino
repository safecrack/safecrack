  #include <AccelStepper.h>


const int PWMA_PIN = 3;
const int PWMB_PIN = 11;
const int BRAKEA_PIN = 9;
const int BRAKEB_PIN = 8;
const int DIRA_PIN = 12;
const int DIRB_PIN = 13;

// these are buttons, oriented with wires at top
const int LEFT_PIN = 26;
const int RIGHT_PIN = 24;
const int CONFIRM_PIN = 22;

const int POSITION_COUNT = 100;
const int STEP_COUNT = 400;
// assert that no remainder or you're going to have a bad time
const int STEPS_PER_POSITION = STEP_COUNT/POSITION_COUNT;

const int HALL_EFFECT_PIN = 44;

const int PIEZO_PIN = 48;

AccelStepper stepper(AccelStepper::FULL2WIRE, DIRA_PIN, DIRB_PIN);

void setup() {
  Serial.begin(115200);
  
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(BRAKEA_PIN, OUTPUT);
  pinMode(BRAKEB_PIN, OUTPUT);
  
  pinMode(LEFT_PIN, INPUT);
  pinMode(CONFIRM_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  
  pinMode(HALL_EFFECT_PIN, INPUT);
  
  digitalWrite(PWMA_PIN, HIGH);
  digitalWrite(PWMB_PIN, HIGH);
  digitalWrite(BRAKEA_PIN, LOW);
  digitalWrite(BRAKEB_PIN, LOW);

  calibrateToZero();
  
  Serial.print("ready\n");
  return;
  /**/
  
  // maybe move to loop?
  boolean result = false;
  
  int increment = 2;
  for(
    long int i=2445; !result && i < 9999; i++
  ) {
    int first = i/100;
    int second = i%100;
    int third;
    for( third = second; result == false && third < POSITION_COUNT ; third += increment ) {
      result = tryCombination(first, second, third);
      // trying one combination
    }
    if( result ) break;
    for( third = 0; result == false && third < second; third += increment ) {
      result = tryCombination(first, second, third);
    }
  }
}

void clockwiseStep(int steps) {
  stepper.move(-steps);
  stepper.runToPosition();
}

void anticlockwiseStep(int steps) {
  clockwiseStep(-steps);
}

void clockwise(int distance) {
  clockwiseStep(STEPS_PER_POSITION * distance);
}

boolean checkCombination() {
  int magnetRead;
  int i;
  /* works! 100/600/600 ~ok too
  stepper.setAcceleration(100);
  stepper.setMaxSpeed(200);
  stepper.setSpeed(200);
  */
  
  clockwise(10);
  stepper.setAcceleration(100);
  stepper.setMaxSpeed(600);
  stepper.setSpeed(600);
  int increment = 3;
  for( i = 0; i < 3; i++ ) {
    magnetRead = !digitalRead(HALL_EFFECT_PIN);
    if( magnetRead == 1 ) {            
      Serial.print("wrong\n");
      break;      
    }
    /* Used this to get feedback - UP TO HERE */
    else if( magnetRead == 0) {
      /*Serial.print(magnetRead);
      digitalWrite(PIEZO_PIN, HIGH);
      delay(5000);
      digitalWrite(PIEZO_PIN, LOW);*/
      Serial.print("nothing\n");
    }
    /**/
    clockwise(increment);
  } 
  
  if( magnetRead == 1 ) {
    stepper.setAcceleration(10000);
    stepper.setMaxSpeed(600);
    stepper.setSpeed(600);
    anticlockwise(10);
    anticlockwise(i*increment);
  }
  /*
  else {
    stepper.setAcceleration(100);
    stepper.setMaxSpeed(200);
    stepper.setSpeed(200);
    for( ; i > 0; i-- ) {
      magnetRead = !digitalRead(HALL_EFFECT_PIN);
      if( magnetRead == 1 ) {
        break;
      }
      anticlockwise(3);
    }
  }
  */  
  return !magnetRead;
}

void anticlockwise(int distance) {
  clockwise(-distance);
}
int prevFirst = -1, prevSecond = -1, prevThird = -1;
boolean tryCombination(int first, int second, int third) {
  
  //Serial.print("Trying: ");
  //Serial.print(first);
  //Serial.print(",");
  //Serial.print(second);
  //Serial.print(",");
  //Serial.print(third);
  //Serial.print("\n");
  
  // order has **SWAPPED**, this is speed/speed/accel
  // 200 200 100
  // fast: 600 600 c (500/500 == bad too)
  
  //* FAST! 10k,600,600
  stepper.setAcceleration(10000);
  stepper.setMaxSpeed(700);
  stepper.setSpeed(700);
  /**/
  
  /* SLOW
  stepper.setAcceleration(100);
  stepper.setMaxSpeed(200);
  stepper.setSpeed(200);
  /**/
  
  if( //false &&
prevFirst == first && prevSecond == second && ( ( third > second && third > prevThird ) || ( third < second && ( (prevThird >= second) || (prevThird < second && third > prevThird) ) ) )
  ) {
    //Serial.print("shortcut baby!\n");
    // we must be at 0
    
    anticlockwise(third);
    
    if( third < second ) {
      anticlockwise(POSITION_COUNT);
      clockwise(POSITION_COUNT);
    }
    
    // change direction and go to zero
    //clockwise((POSITION_COUNT+(third-0))%POSITION_COUNT);
    clockwise(third);
    
    prevFirst = first;
    prevSecond = second;
    prevThird = third;
    return checkCombination();
    //return (third>29 && second == 20);
  }
  
  
  // rotate 3 times to reset
  anticlockwise(3 * POSITION_COUNT);
  
  // continue to first number
  // assumption here that numbers increase when rotating anticlockwise (opposite of clock hands)
  anticlockwise(first);
  
  // change direction and move to the second number
  // we add POSITION_COUNT to prevent negative numbers (which are possible because C)
  clockwise((POSITION_COUNT+(first-second))%POSITION_COUNT);
  
  // continue and rotate twice
  clockwise(2 * POSITION_COUNT);
  
  // change direction and move to the third number
  anticlockwise((POSITION_COUNT+(third-second))%POSITION_COUNT);

  // continue and rotate once
  anticlockwise(1 * POSITION_COUNT);
  
  // change direction and go to zero
  clockwise((POSITION_COUNT+(third-0))%POSITION_COUNT);
  //stepper.move(4 * first);
  //stepper.runToPosition();
  
  
  /* all together..
  anticlockwise((3 * POSITION_COUNT)+first);
  clockwise(((POSITION_COUNT+(first-second))%POSITION_COUNT)+(2 * POSITION_COUNT));
  anticlockwise(((POSITION_COUNT+(third-second))%POSITION_COUNT)+(1 * POSITION_COUNT));
  clockwise((POSITION_COUNT+(third-0))%POSITION_COUNT);
  /* end big dirty hack */
  
  /* for optimisation */
  prevFirst = first;
  prevSecond = second;
  prevThird = third;
  /**/
  
  return checkCombination();
}

// the only side effect of calling this is current position = 0
void calibrateToZero() {
  // consider where these should be and whether they should be undone before return
  stepper.setMaxSpeed(200);
  stepper.setSpeed(10);
  stepper.setAcceleration(10);
  
  for( ;; ) {
    if( digitalRead(CONFIRM_PIN) == HIGH ) {
      // TODO - should this be done here?
      stepper.setCurrentPosition(0);
      return;
    }
    
    if( digitalRead(LEFT_PIN) == HIGH ) {
      clockwiseStep(1);
    }
    else if( digitalRead(RIGHT_PIN) == HIGH ) {
      anticlockwiseStep(1);
    }
    else {
      //stepper.stop();
    }
  }
}

void loop() {
  //Serial.print(".\n");
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int first = Serial.parseInt();
    // do it again:
    int second = Serial.parseInt();
    // do it again:
    int third = Serial.parseInt();

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n' && first < 100 && second < 100 & third < 100 && first >= 0 && second >= 0 && third >= 0) {
      // print the three numbers in one string as hexadecimal:
      //Serial.print(red, HEX);
      //Serial.print(green, HEX);
      //Serial.println(blue, HEX);
      tryCombination(first,second,third);
      Serial.print("ready\n");
    }
  }
}
