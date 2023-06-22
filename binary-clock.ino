const int   hourPin = 13;
const int minutePin = 12;
const int secondPin = 11;
const int units[3] = {
  secondPin,
  minutePin,
    hourPin,
};

const int d2p3 = 8;
const int d2p2 = 7;
const int d2p1 = 6;
const int digitSnd[3] = {
  d2p1, d2p2, d2p3,
};

const int d1p4 = 5;
const int d1p3 = 4;
const int d1p2 = 3;
const int d1p1 = 2;
const int digitFst[4] = {
  d1p1, d1p2, d1p3, d1p4,
};

const int swUpper = A0;
const int swDowner = A2;
const int led1 = 10;
const int led2 = 9;
const int swNext = A1;



void setTimeUnit(int unit) {
  for (int i=0; i<3; i++) {
    digitalWrite(units[i], unit==units[i]);
  }
}
void ftdWrite(int time) {
  int val;
  int viewD2 = time / 10;
  for (int i=0; i<3; i++) {
    val = (int)((viewD2 >> i) % 2);
    digitalWrite(digitSnd[i], !val);
  }
  int viewD1 = time % 10;
  for (int i=0; i<4; i++) {
    val = (int)((viewD1 >> i) % 2);
    digitalWrite(digitFst[i], !val);
  }
}
void display(unsigned long time) {
  switch (millis() / 6 % 3) {
  case 0:
    setTimeUnit(hourPin);
    ftdWrite(int(time / 60 / 60));
    break;
  case 1:
    setTimeUnit(minutePin);
    ftdWrite(int(time / 60 % 60));
    break;
  case 2:
    setTimeUnit(secondPin);
    ftdWrite(int(time % 60));
    break;
  }
}



void setup() {
  for (int i=2; i<=13; i++) {
    pinMode(i, OUTPUT);
  }
}

unsigned long currTime;
long accTime = 0;
int selectedMode = 2; // 1:S, 2:H, 3:M


int tvalHow = 0;

void loop() {
  // Clock Reset
  currTime = millis() / 1000L;
  currTime %= 86400L;
  display(currTime+accTime < 100000? currTime+accTime : 86400+currTime+accTime);
  
  // Selected Mode viewer
  digitalWrite(led1, selectedMode / 2);
  digitalWrite(led2, selectedMode % 2);
  
  // Next Selected Mode
  delay(1);
  if (analogRead(swNext) >= 512) {
    switch (selectedMode) {
    case 1: //Second to Hour
      selectedMode = 2;
      break;
    case 2: //Hour to Minute
      selectedMode = 3;
      break;
    case 3: //Minute to Second
      selectedMode = 1;
      break;
    }
    while (analogRead(swNext) >= 512) {
      display(currTime+accTime < 100000? currTime+accTime : 86400+currTime+accTime);
    }
  }
  
  // Time Value Modifier
  if (analogRead(swUpper) >= 512) {
    // Value Up
    tvalHow = 1;
    while (analogRead(swUpper) >= 512) {
      display(currTime+accTime < 100000? currTime+accTime : 86400+currTime+accTime);
    }
  } else if (analogRead(swDowner) >= 512) {
    // Value Down
    tvalHow = -1;
    while (analogRead(swDowner) >= 512) {
      display(currTime+accTime < 100000? currTime+accTime : 86400+currTime+accTime);
    }
  }
  switch (selectedMode) {
  case 1: //Second
    accTime += 1 * tvalHow;
    break;
  case 2: //Hour
    accTime += 3600 * tvalHow;
    break;
  case 3: //Minute
    accTime += 60 * tvalHow;
    break;
  }
  accTime %= 86400;
  if (tvalHow)
    tvalHow = 0;
}
