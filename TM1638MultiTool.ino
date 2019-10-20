#include <TM1638plus.h>

#define STROBE_TM 4
#define CLOCK_TM 6
#define DIO_TM 7

TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM);

int binaryArray[] { 0,0,0,0,0,0,0,0 };
int decTotal = 0;
int loopSelect = 0;

#define isdigit(n) (n >= '0' && n <= '9')

const int tonePin = 12;  // arbitrary for arduino boards, set this to whatever you want

#define OCTAVE_OFFSET 0

// These values can also be found as constants in the Tone library (Tone.h)
int notes[] = { 0,
262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};

char *song = " ";

void setup() {
  tm.brightness(1);
  randomSeed((analogRead(3)));
  tm.reset();
  Serial.begin(9600);
  introScreen();
}

void loop() {
  if (tm.readButtons() == 1) {
    tone(12, 523, 500);
    delay(200);
    noTone(12);
    loopSelect = 1;
  }
  if (tm.readButtons() == 2) {
    tone(12, 587, 500);
    delay(200);
    noTone(12);
    binaryToDecimalStartup();
    loopSelect = 2;
  }
  if (tm.readButtons() == 4) {
    tone(12, 659, 500);
    delay(200);
    noTone(12);
    songMenu();
    loopSelect = 3;
  }
  if (tm.readButtons() == 8) {
    tone(12, 698, 500);
    delay(200);
    noTone(12);
    loopSelect = 4;
  }
  while (loopSelect == 1) {
    nameDisplay();
  }
  while (loopSelect == 2) {
    binaryToDecimal();
  }
  while (loopSelect == 3) {
    play_rtttlStartup();
  }
  while (loopSelect == 4) {
    keyboard();
  }
}

void introScreen() {
// Intro screens showing what each function is.
  tm.displayText("TM1638  ");
  for(int j = 0; j < 8; j++) {
    tm.setLED(j, 1);
    delay(50);
    if (j > 0) {
      tm.setLED(j - 1, 0);
      delay(50);
    }
  }
  tm.setLED(7, 0);
  delay(1300);
  tm.displayText("Multibox");
  for(int j = 0; j < 8; j++) {
    tm.setLED(j, 1);
    delay(50);
    if (j > 0) {
      tm.setLED(j - 1, 0);
      delay(50);
    }
  }
  tm.setLED(7, 0);
  delay(1800);
  tm.displayText("Select  ");
  for(int j = 0; j < 8; j++) {
    tm.setLED(j, 1);
    delay(50);
    if (j > 0) {
      tm.setLED(j - 1, 0);
      delay(50);
    }
  }
  tm.setLED(7, 0);
  delay(1300);
  tm.displayText("Function");
  for(int j = 0; j < 8; j++) {
    tm.setLED(j, 1);
    delay(50);
    if (j > 0) {
      tm.setLED(j - 1, 0);
      delay(50);
    }
  }
  tm.setLED(7, 0);
  delay(1800);
  tm.displayText("Name    ");
  tm.setLED(0, 1);
  delay(1500);
  tm.setLED(0, 0);
  tm.setLED(1, 1);
  tm.displayText("bin2dec ");
  delay(1500);
  tm.setLED(1, 0);
  tm.setLED(2, 1);
  tm.displayText("SongPlay");
  delay(1500);
  tm.setLED(2, 0);
  tm.setLED(3, 1);
  tm.displayText("Keyboard");
  delay(1500);
  tm.reset();
  for(int j = 0; j < 8; j++) {
    tm.setLED(j, 1);
    delay(50);
    if (j > 0) {
      tm.setLED(j - 1, 0);
      delay(50);
    }
  }
  tm.setLED(7, 0);
  
}

void nameDisplay() {
  uint8_t positions7[] {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000};
  uint8_t positions0[] {0b00001000, 0b00010000, 0b00100000, 0b00000001, 0b00000010, 0b00000100};
  tm.setLED(0,0);
  tm.displayText(" WESLEY ");
  for(int i = 0; i <= 10; i++) {
    // noTone(12);
    for(int j = 0; j <= 5; j++) {
    tm.display7Seg(7, (positions7[j]));
    if (i < 8) {
      tm.setLED(i, 1);
      //if (i % 2 != 0) {
       // int randomNote(random(44, 999));
       // tone(12, randomNote, 30);
      //}     
    }
    tm.display7Seg(0, (positions0[j]));
    delay(80);
    tm.setLED(i, 0);
  }
  tm.display7Seg(7, 0b00000000);
  tm.display7Seg(0, 0b00000000);
 }
}



void binaryToDecimal() {
  if (tm.readButtons() == 128) {
    tone(12, 1047, 500);
    delay(200);
    noTone(12);
    if (binaryArray[7] == 0) {
      binaryArray[7] = 1;
    }
    else {
      binaryArray[7] = 0;
    }
    tm.setLED(7,(binaryArray[7]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 64) {
    tone(12, 988, 500);
    delay(200);
    noTone(12);
    if (binaryArray[6] == 0) {
      binaryArray[6] = 1;
    }
    else {
      binaryArray[6] = 0;
    }
    tm.setLED(6,(binaryArray[6]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 32) {
    tone(12, 880, 500);
    delay(200);
    noTone(12);
    if (binaryArray[5] == 0) {
      binaryArray[5] = 1;
    }
    else {
      binaryArray[5] = 0;
    }
    tm.setLED(5,(binaryArray[5]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 16) {
    tone(12, 784, 500);
    delay(200);
    noTone(12);
    if (binaryArray[4] == 0) {
      binaryArray[4] = 1;
    }
    else {
      binaryArray[4] = 0;
    }
    tm.setLED(4,(binaryArray[4]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 8) {
    tone(12, 698, 500);
    delay(200);
    noTone(12);
    if (binaryArray[3] == 0) {
      binaryArray[3] = 1;
    }
    else {
      binaryArray[3] = 0;
    }
    tm.setLED(3,(binaryArray[3]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 4) {
    tone(12, 659, 500);
    delay(200);
    noTone(12);
    if (binaryArray[2] == 0) {
      binaryArray[2] = 1;
    }
    else {
      binaryArray[2] = 0;
    }
    tm.setLED(2,(binaryArray[2]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 2) {
    tone(12, 587, 500);
    delay(200);
    noTone(12);
    if (binaryArray[1] == 0) {
      binaryArray[1] = 1;
    }
    else {
      binaryArray[1] = 0;
    }
    tm.setLED(1,(binaryArray[1]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }

  if (tm.readButtons() == 1) {
    tone(12, 523, 500);
    delay(200);
    noTone(12);
    if (binaryArray[0] == 0) {
      binaryArray[0] = 1;
    }
    else {
      binaryArray[0] = 0;
    }
    tm.setLED(0,(binaryArray[0]));
     decTotal = ((binaryArray[0] * 128) + (binaryArray[1] * 64) +
     (binaryArray[2] * 32) + (binaryArray[3] * 16) + (binaryArray[4] * 8)
     + (binaryArray[5] * 4) + (binaryArray[6] * 2) + (binaryArray[7] * 1));
    for (int j = 0; j <= 7; j++) {
      Serial.print((String(binaryArray[j]) + " "));
    }
  }
  
  if (decTotal < 100) {
    tm.displayASCII(5, '0');
  }
  if (decTotal >= 100 && decTotal < 200) {
    tm.displayASCII(5, '1');
  }
  if (decTotal >= 200) {
    tm.displayASCII(5, '2');
  }

  if (((decTotal % 100) / 10) == 0) {
    tm.displayASCII(6, '0');
  }
  if (((decTotal % 100) / 10) == 1) {
    tm.displayASCII(6, '1');
  }
  if (((decTotal % 100) / 10) == 2) {
    tm.displayASCII(6, '2');
  }
  if (((decTotal % 100) / 10) == 3) {
    tm.displayASCII(6, '3');
  }
  if (((decTotal % 100) / 10) == 4) {
    tm.displayASCII(6, '4');
  }
  if (((decTotal % 100) / 10) == 5) {
    tm.displayASCII(6, '5');
  }
  if (((decTotal % 100) / 10) == 6) {
    tm.displayASCII(6, '6');
  }
  if (((decTotal % 100) / 10) == 7) {
    tm.displayASCII(6, '7');
  }
  if (((decTotal % 100) / 10) == 8) {
    tm.displayASCII(6, '8');
  }
  if (((decTotal % 100) / 10) == 9) {
    tm.displayASCII(6, '9');
  }

  if (((decTotal % 100) % 10) == 0) {
    tm.displayASCII(7, '0');
  }
  if (((decTotal % 100) % 10) == 1) {
    tm.displayASCII(7, '1');
  }
  if (((decTotal % 100) % 10) == 2) {
    tm.displayASCII(7, '2');
  }
  if (((decTotal % 100) % 10) == 3) {
    tm.displayASCII(7, '3');
  }
  if (((decTotal % 100) % 10) == 4) {
    tm.displayASCII(7, '4');
  }
  if (((decTotal % 100) % 10) == 5) {
    tm.displayASCII(7, '5');
  }
  if (((decTotal % 100) % 10) == 6) {
    tm.displayASCII(7, '6');
  }
  if (((decTotal % 100) % 10) == 7) {
    tm.displayASCII(7, '7');
  }
  if (((decTotal % 100) % 10) == 8) {
    tm.displayASCII(7, '8');
  }
  if (((decTotal % 100) % 10) == 9) {
    tm.displayASCII(7, '9');
  }
  
  delay(100);
}
  


void binaryToDecimalStartup() {
  for(int j = 0; j < 8; j++){
    tm.setLED(j,1);
    delay(50);
    tm.setLED(j,0);
    delay(50);
  }
  tm.displayText("BinAry  ");
  delay(1500);
  tm.displayText("ConvErt ");
  delay(1500);
  tm.reset();
  
  tm.setLED(7, 1);
  tm.displayASCII(7, '1');
  delay(1100);
  tm.setLED(7, 0);
  tm.setLED(6, 1);
  tm.displayASCII(7, '2');
  delay(1100);
  tm.setLED(6, 0);
  tm.setLED(5, 1);
  tm.displayASCII(7, '4');
  delay(1100);
  tm.setLED(5, 0);
  tm.setLED(4, 1);
  tm.displayASCII(7, '8');
  delay(1100);
  tm.setLED(4, 0);
  tm.setLED(3, 1);
  tm.displayASCII(6, '1');
  tm.displayASCII(7, '6');
  delay(1100);
  tm.setLED(3, 0);
  tm.setLED(2, 1);
  tm.displayASCII(6, '3');
  tm.displayASCII(7, '2');
  delay(1100);
  tm.setLED(2, 0);
  tm.setLED(1, 1);
  tm.displayASCII(6, '6');
  tm.displayASCII(7, '4');
  delay(1100);
  tm.setLED(1, 0);
  tm.setLED(0, 1);
  tm.displayASCII(5, '1');
  tm.displayASCII(6, '2');
  tm.displayASCII(7, '8');
  delay(1100);
  
  tm.reset();
}

void play_rtttl(char *p)
{
  // Absolutely no error checking in here

  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  Serial.print("ddur: "); Serial.println(default_dur, 10);

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  Serial.print("doct: "); Serial.println(default_oct, 10);

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  Serial.print("wn: "); Serial.println(wholenote, 10);


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      Serial.print("Playing: ");
      Serial.print(scale, 10); Serial.print(' ');
      Serial.print(note, 10); Serial.print(" (");
      Serial.print(notes[(scale - 4) * 12 + note], 10);
      Serial.print(") ");
      Serial.println(duration, 10);
      tone(tonePin, notes[(scale - 4) * 12 + note]);
      delay(duration);
      noTone(tonePin);
    }
    else
    {
      Serial.print("Pausing: ");
      Serial.println(duration, 10);
      delay(duration);
    }
  }
}

void play_rtttlStartup() {
  int selection = 0;
  if (tm.readButtons() == 1) {
    delay(200);
    selection = 1;
  }
  if (tm.readButtons() == 2) {
    delay(200);
    selection = 2;
  }
  if (tm.readButtons() == 4) {
    delay(200);
    selection = 3;
  }
  if (tm.readButtons() == 8) {
    delay(200);
    selection = 4;
  }
  if (tm.readButtons() == 16) {
    delay(200);
    selection = 5;
  }
  if (tm.readButtons() == 32) {
    delay(200);
    selection = 6;
  }
  
  while (selection == 1) {
    char *song = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
    tm.displayText("Indiana ");
    play_rtttl(song);
  }
  while (selection == 2) {
    char *song = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
    tm.displayText("TakeOnMe");
    play_rtttl(song);
  }

  while (selection == 3) {
    char *song = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
    tm.displayText("Bond 007");
    play_rtttl(song);
  }
  while (selection == 4) {
    char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
    tm.displayText("A-Team  ");
    play_rtttl(song);
  }
  while (selection == 5) {
    char *song = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
    tm.displayText("Mission ");
    play_rtttl(song);
  }
  while (selection == 6) {
    char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
    tm.displayText("StarWars");
    play_rtttl(song);
  }
  
}

void songMenu() {
  tm.setLED(0, 1);
  tm.displayText("Indiana ");
  delay(1000);
  tm.setLED(0, 0);
  tm.setLED(1, 1);
  tm.displayText("TakeOnMe");
  delay(1000);
  tm.setLED(1, 0);
  tm.setLED(2, 1);
  tm.displayText("Bond 007");
  delay(1000);
  tm.setLED(2, 0);
  tm.setLED(3, 1);
  tm.displayText("A-Team  ");
  delay(1000);
  tm.setLED(3, 0);
  tm.setLED(4, 1);
  tm.displayText("Mission ");
  delay(1000);
  tm.setLED(4, 0);
  tm.setLED(5, 1);
  tm.displayText("StarWars");
  delay(1000);
  tm.reset();  
}

void keyboard() {
  tm.displayText("Keyboard");
  if (tm.readButtons() == 1) {
    tone(12, 523, 500);
    tm.setLED(0, 1);
    delay(200);
    noTone(12);
    tm.setLED(0, 0);
    delay(100);
  }

  if (tm.readButtons() == 2) {
    tone(12, 587, 500);
    tm.setLED(1, 1);
    delay(200);
    noTone(12);
    tm.setLED(1, 0);
    delay(100);
  }

  if (tm.readButtons() == 4) {
    tone(12, 659, 500);
    tm.setLED(2, 1);
    delay(200);
    noTone(12);
    tm.setLED(2, 0);
    delay(100);
  }

  if (tm.readButtons() == 8) {
    tone(12,698, 500);
    tm.setLED(3, 1);
    delay(200);
    noTone(12);
    tm.setLED(3, 0);
    delay(100);
  }

  if (tm.readButtons() == 16) {
    tone(12, 783, 500);
    tm.setLED(4, 1);
    delay(200);
    noTone(12);
    tm.setLED(4, 0);
    delay(100);
  }

  if (tm.readButtons() == 32) {
    tone(12, 880, 500);
    tm.setLED(5, 1);
    delay(200);
    noTone(12);
    tm.setLED(5, 0);
    delay(100);
  }

  if (tm.readButtons() == 64) {
    tone(12, 987, 500);
    tm.setLED(6, 1);
    delay(200);
    noTone(12);
    tm.setLED(6, 0);
    delay(100);
  }

  if (tm.readButtons() == 128) {
    tone(12, 1046, 500);
    tm.setLED(7, 1);
    delay(200);
    noTone(12);
    tm.setLED(7, 0);
    delay(100);
  }
}
