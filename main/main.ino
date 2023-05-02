// Kirsten Alafriz | kalafriz@bu.edu | github.com/kalafriz

// TENS relay control pins
const int T1_PIN = 2;
const int T2_PIN = 3;
const int T3_PIN = 4;
const int T4_PIN = 5;

byte commandByte;
byte noteByte;
byte velocityByte;

byte noteOn = 144;
byte noteOff = 128;

bool sustain = false;
byte lastNote = 0;

const int MIDI_SAMPLE_RATE = 10; // Hz
const int duration = 10;
const int timeLen = duration * MIDI_SAMPLE_RATE;

//{randMute, randSound, randCtrl, patMute, patSound, patCtrl};
//int patMute[timeLen] = {50};
int pattern[duration] = {48, 48, 0, 52, 53, 53, 53, 0, 50, 48};
int data[timeLen] = {0};

int targets[4] = {48,50,52,53};

bool targetStates[4] = {false};

enum trials {rMute, rSound, rCtrl, pMute, pSound, pCtrl};
//light up led at pin 13 when receiving noteON message with note = 60

int trial = 0;  
int time = 0;
int timePrev = -1;

void setup(){
  Serial.begin(31250);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  pinMode(T1_PIN,OUTPUT);
  pinMode(T2_PIN,OUTPUT);
  pinMode(T3_PIN,OUTPUT);
  pinMode(T4_PIN,OUTPUT);
}

void loop(){
  delay(500);
  unsigned long tic;   
  /*
  // ----------- TRAINING RUNTIME --------------------------
  while(time < duration){
    tic = millis(); 
    timePrev = time;    
    time = (int)((tic)/(1000)); // time -> pattern data index (1Hz)
    if (time > timePrev){ // checking against sampling rate time
      tensOut(pattern[time-1]); 
    }     
  }
  digitalWrite(T1_PIN, LOW); 
  digitalWrite(T2_PIN, LOW); 
  digitalWrite(T3_PIN, LOW); 
  digitalWrite(T4_PIN, LOW); 
  /*/
  // ----------- PLAY TRIAL RUNTIME --------------------------
  while(time < timeLen){
    tic = millis(); 
    unsigned long toc = millis();
    timePrev = time;    
    time = (int)((tic)/(1000/MIDI_SAMPLE_RATE)); // time -> trial data index
    if (time > timePrev){ // checking against sampling rate time
      byte note = getMIDI();
      int val = 1000*targetStates[0]+100*targetStates[1]+10*targetStates[2]+targetStates[3];
      data[time] = val;  
      Serial.println(data[time]);  
    }     
  }
  
    
  trial ++;  
  delay(100);
  digitalWrite(13,LOW);//turn led off
}


byte getMIDI(){
  if (Serial.available() > 0){
    
    commandByte = Serial.read();//read first byte
    noteByte = Serial.read();//read next byte
    velocityByte = Serial.read();//read final byte
    if (commandByte == noteOn){
      if(noteByte == targets[0]){
        targetStates[0] = true;        
      }
      if(noteByte == targets[1]){
        targetStates[1] = true;        
      }
      if(noteByte == targets[2]){
        targetStates[2] = true;        
      }
      if(noteByte == targets[3]){
        targetStates[3] = true;        
      }
    }
    if (commandByte == noteOff){
      if(noteByte == targets[0]){
        targetStates[0] = false;        
      }
      if(noteByte == targets[1]){
        targetStates[1] = false;        
      }
      if(noteByte == targets[2]){
        targetStates[2] = false;        
      }
      if(noteByte == targets[3]){
        targetStates[3] = false;        
      }
    }
    
    return noteByte;
    
    
    //Serial.print(commandByte);
    //Serial.print(", ");
    //Serial.print(noteByte);
    //Serial.print(", ");
    //Serial.println(velocityByte);
  }
  return 0;
}

void tensOut(int note){
  Serial.println(note);
  digitalWrite(T1_PIN, LOW); 
  digitalWrite(T2_PIN, LOW); 
  digitalWrite(T3_PIN, LOW); 
  digitalWrite(T4_PIN, LOW); 
  if (note == targets[0]){
    digitalWrite(T1_PIN, HIGH);   
  } else if (note == targets[1]){
    digitalWrite(T2_PIN, HIGH);   
  } else if (note == targets[2]){
    digitalWrite(T3_PIN, HIGH);   
  } else if (note == targets[3]){
    digitalWrite(T4_PIN, HIGH);   
  }
}