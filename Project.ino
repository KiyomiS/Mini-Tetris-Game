
/* Your Name & E-mail: Kiyomi Sugita ksugi006@ucr.edu
* Lab Section: 21
* Assignment: Custom Project
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link:  https://youtu.be/qQ-lcR4PfdQ
*/
// C++ code
//
#include "LedControl.h"
LedControl lc=LedControl(12,11,10,1);

//joystick
int VRx = A0;
int VRy = A1;
int SW = 2;
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

//display
byte a[8]={B10000001,B10000001,B10000001,B10000001,B10000001,B10000001,B10000001,B10000001};
byte curr = B00000000; //is the current shape you're working with
int currr[8];
int game[8][8] { {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1}, {1,0,0,0,0,0,0,1} }; // array of arrays showing current state of game.
byte dot = B10010001; //a dot shape
byte def = B10000001; //default line.
//keeps track of current spot in a
int current = 0;
//keep track of current spot horizontally
int currenth = 3;
int botmax = 7; //lowest shape can go
byte below = B00000000;
int endgame = 0;

//buzzer
const int buzzer = 9;
//song count
int scount  = 0;
//moving down count
int dcount = 0;

int p = 0;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

void printgamestate() {
  //used to print out the state in form of 0's and 1's. should be equivalent to what you see on LCD display.
  for(int m = 0; m < 8; m++){
    for(int p = 0; p < 8; p++) {
      Serial.print(game[m][p]);
    }
    Serial.print('\n');
  }
  Serial.print(current);
  Serial.print('\n');
};

const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum game {start, sgame, go, down, check, pause, left, right, gend} LCD;
int play (int state){
  switch (state) {
    case start: 
      state = sgame;
    break;

    case sgame:
      state = go;
    break;
    

    case go:
      if(endgame ==  0) {
        if(dcount >= 10) {
          state = check;
          dcount = 0;
        } else {
          if(xPosition > 900) {
            state = right;
          } else if (xPosition < 100) {
            state = left;
          } else if (yPosition > 900) {
            state = check;
          } else {
            state = go;
          }
          dcount++;
        }
      } else {
        state = gend;
      }
    break;

    case down:
      state = go;
      printgamestate();
    break;

    case pause:
      if (p < 5) {
        p++;
        state = pause;
      } else {
        p = 0;
        state = go;
      }
    break;

    case check:
      if(current < 7) {
        if (game[current + 1][currenth] == 1){
          current = 0;
          state = go;
        } else {
          state = down;
        }
      } else {
        current = 0;
        state = go;
      }
    break;

    case left:
      state = go;
    break;

    case right:
      state = go;
    break;
    
  }
  switch (state) {
    case start:

    break;
    
    case sgame:
      lc.setRow(0,0,a[0]);
      lc.setRow(0,1,a[1]);
      lc.setRow(0,2,a[2]);
      lc.setRow(0,3,a[3]);
      lc.setRow(0,4,a[4]);
      lc.setRow(0,5,a[5]);
      lc.setRow(0,6,a[6]);
      lc.setRow(0,7,a[7]);
    break;

    case go:
      if((current == botmax) || (current == 0)) {
        curr = dot;
        currr[4] = 1;
        below = a[current + 1];
        a[0] = curr;
        currenth = 3;
        current = 0;
        game[current][3] = 1;
        a[current] = B10010001;
        lc.setRow(0,0,curr);

      if((game[0][1] == 1 && game[1][1] == 1 && game[2][1] == 1 && game[3][1] == 1 && game[4][1] == 1 && game[5][1] == 1 && game[6][1] == 1 && game[7][1] == 1) ||
      (game[0][2] == 1 && game[2][2] == 1 && game[3][2] == 1 && game[4][2] == 1 && game[5][2] == 1 && game[6][2] == 1 && game[7][2] == 1 && game[1][2] == 1) ||
      (game[0][3] == 1 && game[2][3] == 1 && game[3][3] == 1 && game[4][3] == 1 && game[5][3] == 1 && game[6][3] == 1 && game[7][3] == 1 && game[1][3] == 1) ||
      (game[0][4] == 1 && game[2][4] == 1 && game[3][4] == 1 && game[4][4] == 1 && game[5][4] == 1 && game[6][4] == 1 && game[7][4] == 1 && game[1][4] == 1) ||
      (game[0][5] == 1 && game[2][5] == 1 && game[3][5] == 1 && game[4][5] == 1 && game[5][5] == 1 && game[6][5] == 1 && game[7][5] == 1 && game[1][5] == 1) ||
      (game[0][6] == 1 && game[2][6] == 1 && game[3][6] == 1 && game[4][6] == 1 && game[5][6] == 1 && game[6][6] == 1 && game[7][6] == 1 && game[1][6] == 1)){
        endgame = 1;
      }
      }
    break;


    case check:
    //used to check if there is already something below.
//      Serial.print(a[current] - below);
//      Serial.print(' ');
//      Serial.print(below);
//      Serial.print(' ');
//      Serial.print(curr);
//      Serial.print(' ');
//      Serial.print(a[current]);
//      Serial.print(' ');
//      Serial.print(curr - 129);
//      Serial.print('\n');

    break;

    case pause:
      current = 0;
      botmax = 7;
    break;

    case down:
      current++;
      a[current - 1] = a[current -1] - curr + 129;
      a[current] = curr | a[current];
      game[current][currenth] = 1;
      game[current - 1][currenth] = 0;

      if(game[7][1] == 1 && game[7][2] == 1 && game[7][3] == 1 && game[7][4] == 1 && game[7][5] == 1 && game[7][6] == 1) {
        for(int l = 7; l > 0; l--){
          for(int o = 7; o > 0; o--){
            game[l][o] = game[l-1][o];
          }
          a[l] = a[l-1];
        }
      }
      lc.setRow(0,0,a[0]);
      lc.setRow(0,1,a[1]);
      lc.setRow(0,2,a[2]);
      lc.setRow(0,3,a[3]);
      lc.setRow(0,4,a[4]);
      lc.setRow(0,5,a[5]);
      lc.setRow(0,6,a[6]);
      lc.setRow(0,7,a[7]); 
    break;

    case right:

      if (currenth < 6) {
        currenth++;
        if ((curr == B11000001) && (game[current][currenth] == 0 )) {
          curr = B10100001;
          game[current][currenth] = 1;
          game[current][currenth - 1] = 0;
        } else if ((curr == B10100001) && (game[current][currenth] == 0 )) {
          curr = B10010001;
          game[current][currenth] = 1;
          game[current][currenth - 1] = 0;
        } else if ((curr == B10010001) && (game[current][currenth] == 0 )) {
          curr = B10001001;
          game[current][currenth] = 1;
          game[current][currenth - 1] = 0;
        } else if ((curr == B10001001) && (game[current][currenth] == 0)) {
          curr = B10000101;
          game[current][currenth] = 1;
          game[current][currenth - 1] = 0;
        } else if ((curr == B10000101) && (game[current][currenth] == 0 )) {
          curr = B10000011;
          game[current][currenth] = 1;
          game[current][currenth - 1] = 0;
        }
        a[current] = curr;
        
      }
        lc.setRow(0,0,a[0]);
        lc.setRow(0,1,a[1]);
        lc.setRow(0,2,a[2]);
        lc.setRow(0,3,a[3]);
        lc.setRow(0,4,a[4]);
        lc.setRow(0,5,a[5]);
        lc.setRow(0,6,a[6]);
        lc.setRow(0,7,a[7]);
      Serial.print(currenth);
      Serial.print('\n');
      printgamestate();
    break;

    case left: 

      if(currenth > 1) {
        currenth--;
        if ((curr == B10000011) && (game[current][currenth] == 0)) {
          curr = B10000101;
          game[current][currenth] = 1;
          game[current][currenth + 1] = 0;
        } else if ((curr == B10000101) && (game[current][currenth] == 0)) {
          curr = B10001001;
          game[current][currenth] = 1;
          game[current][currenth + 1] = 0;
        } else if ((curr == B10001001) && (game[current][currenth] == 0)) {
          curr = B10010001;
          game[current][currenth] = 1;
          game[current][currenth + 1] = 0;
        } else if ((curr == B10010001) && (game[current][currenth] == 0)) {
          curr = B10100001;
          game[current][currenth] = 1;
          game[current][currenth + 1] = 0;
        } else if ((curr == B10100001) && (game[current][currenth] == 0)) {
          curr = B11000001;
          game[current][currenth] = 1;
          game[current][currenth + 1] = 0;
        }
        a[current] = curr;
        
      }
        lc.setRow(0,0,a[0]);
        lc.setRow(0,1,a[1]);
        lc.setRow(0,2,a[2]);
        lc.setRow(0,3,a[3]);
        lc.setRow(0,4,a[4]);
        lc.setRow(0,5,a[5]);
        lc.setRow(0,6,a[6]);
        lc.setRow(0,7,a[7]);
      Serial.print(currenth);
      Serial.print('\n');
      printgamestate();
    break;
  }
  return state;
}

enum playsong {start2, startsong, endsong} ps;
int s (int state){
  switch(state){
    case start2:
      state = startsong;
    break;

    case startsong:
      if (scount > 4) {
        state = endsong;
        noTone(buzzer);
        scount = 0;
      } else {
        scount++;
        state = startsong;
      }
    break;

    case endsong:
      if (scount > 4) {
        state = endsong;
        noTone(buzzer);
        scount = 0;
      } else {
        scount++;
        state = endsong;
      }
    break;
     
    default:
    break;
  }
  switch(state) {
    case startsong:
      if(scount == 0) {
        tone(buzzer,1000);
      } else if (scount == 1) {
        tone(buzzer, 800);
      } else if (scount == 2) {
        tone(buzzer, 600);
      } else if (scount == 3) {
        tone(buzzer, 400);
      }
    break;

    case endsong:
      if (endgame == 1) {
        if(scount == 0) {
          tone(buzzer,400);
        } else if (scount == 1) {
          tone(buzzer, 600);
        } else if (scount == 2) {
          tone(buzzer, 800);
        } else if (scount == 3) {
          tone(buzzer, 1000);
        }
      }
    break;
    
    default:
    break;
  }
  return state;
}
void setup() {
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);
  
  // other initializations
  unsigned char i = 0;

  tasks[i].state = start;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &play;
  i++;
  tasks[i].state = start2;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &s;

  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
}


void loop() {
  // put your main code here, to run repeatedly:
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  delay(100);
}
