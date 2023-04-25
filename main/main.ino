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
const int timeLen = 30 * MIDI_SAMPLE_RATE;

//{randMute, randSound, randCtrl, patMute, patSound, patCtrl};
//int patMute[timeLen] = {50};
int pattern[timeLen] = {50}

short data[timeLen] = {0};

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
}

void loop(){
  delay(500);
  unsigned long tic;   


  // ----------- TRIAL RUNTIME --------------------------
  while(time < timeLen){
    tic = millis(); 
    unsigned long toc = millis();
    timePrev = time;    
    time = (int)((tic)/(1000/MIDI_SAMPLE_RATE)); // time -> trial data index
    if (time > timePrev){
      short val = 1000*targetStates[0]+100*targetStates[1]+10*targetStates[2]+targetStates[3];
      data[time] = val;  
      byte note = getMIDI();
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
