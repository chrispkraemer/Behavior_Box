

int speaker = 2;
int button = 50;
int leftLED = 30;
int rightLED = 26;
int centerLED = 40;
int leftSensor = A0;
int rightSensor = A1;
double proximity = 0;
int state = 0;
int tone1 = 200;
int tone2 = 600;
int tone3 = 70;
unsigned long timer1,timer2,timer3;



void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  pinMode(speaker, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(centerLED, OUTPUT);
  

}

void loop() {
  switch(state){
    case 0:
      trialInit();
      break;
    case 1:
      preRelease();
      break;
    case 2:
      stimOnTime();
      break;
    case 3:
      wrongSpout();
      break;
    case 4:
      noSpout();
      break;
    case 5:
      correctSpout();
      break;
    case 6:
      badTrial();
      break;
    case 7:
      ITI();
      break;
    default:
      Serial.println("Error");
  }
  

}

void trialInit(){
  int start = 0;
  Serial.println("Wait for Press");
  while(digitalRead(button) != LOW){
    digitalWrite(centerLED, HIGH);
    delay(200);
    digitalWrite(centerLED, LOW);
    delay(200);
  }
  timer1 = micros();
  while(digitalRead(button) == LOW){
    if(start == 0){
      tone(speaker, 200, 1000);
      start++;
    }
    timer2 = micros();
  }
  
  timer2 -= timer1;
  
  if(timer2 >= 5000000){
    state = 2;
  } else {
    state = 1;
  }
  
  
  
}

void preRelease(){
  Serial.println("Pre-release trial");
  state = 6;
}

void stimOnTime(){
  Serial.println("Stim on Time");
  int lorR = random(0,2);
  switch(lorR) {
    case 0:
      leftOn();
      break;
    case 1:
      rightOn();
      break;
    default:
      Serial.println("Error: no side chosen");
      break;
  }
}

void leftOn(){
  Serial.println("Left LED on");
  digitalWrite(leftLED, HIGH);
  timer1 = micros();
  while(analogRead(leftSensor) >= 700){
    timer2 = micros();
    timer3 = timer2-timer1;
    if(timer3 > 1000000){
      digitalWrite(leftLED, LOW);
    }
    if(timer3 > 6000000){
      state = 4;
      return;
    }
    if(analogRead(rightSensor) < 600){
      digitalWrite(leftLED, LOW); 
      state = 3;
      return;
    }
  }

  digitalWrite(leftLED, LOW);
  state = 5;
  return;
}

void rightOn(){
  Serial.println("Right LED on");
  digitalWrite(rightLED, HIGH);
  timer1 = micros();
  while(analogRead(rightSensor) >= 600){
    timer2 = micros();
    timer3 = timer2-timer1;
    if(timer3 > 1000000){
      digitalWrite(rightLED, LOW);
    }
    if(timer3 > 6000000){
      state = 4;
      return;
    }
    if(analogRead(leftSensor) < 600){
      digitalWrite(leftLED, LOW);
      state = 3;
      return;
    }  
  }
  digitalWrite(rightLED, LOW);
  state = 5;
  return;
  
}

void wrongSpout(){
  Serial.println("Wrong Spout");
  state = 6;
}

void noSpout(){
  Serial.println("No Spout");
  state = 6;
}

void correctSpout(){
  Serial.println("Correct Spout");
  tone(speaker,tone2,1000);
  //water reward
  //illuminate center spout
  state = 7;;
}

void badTrial(){
  //light up room
  tone(speaker,tone3,3000);
  delay(3000);
  state = 7;
}

void ITI(){
  delay(3000);
  state = 0;
}
