// Hellschreiber programm, das Serielle Daten in HELL-Zeichen umwandelt
#define MOD_PIN 13
byte pixel = 7;
byte col = pixel*7 ;
byte pos = 0;
byte cnt = 0;
byte arrayRead = 0;
byte row = 0;

// abcdefghijklmnopqrstuvwxyz 1234567890
//

byte Letters[37][7] = 
{
  {0, 127, 72, 72, 72, 127, 0}, //A
  {0, 127, 73, 73, 73, 54, 0},  //B
  {0, 28, 34,  65, 65, 65, 0},  //C
  {0, 127, 65, 65, 34, 28, 0},  //D
  {0, 127, 73, 73, 73, 65, 0},  //E
  {0, 127, 72, 72, 72, 64, 0},  //F
  {0, 28, 34, 65, 65, 73, 14},  //G
  {0, 127,8, 8,8, 127,0},       //H
  {0, 0, 0, 127, 0, 0, 0},      //I
  {0, 68, 66, 65, 66, 124, 0},  //J
  {0, 127, 8, 20, 34, 65, 0},   //K
  {0, 127, 1, 1 ,1 ,1, 0},      //L
  {0, 127, 32, 16 , 32, 127, 0},//M
  {0, 127, 32, 24, 4, 127, 0},  //N
  {0, 127, 65, 65, 65, 127, 0}, //O <- ACHTUNG!!! Hier steht O (BUCHSTABE (0))
  {0, 127, 72, 72, 72, 48, 0},  //P
  {0, 126, 66, 70, 126, 1, 2},  //Q
  {0, 127, 76, 74, 73, 48, 0},  //R
  {0, 49, 73, 73, 73, 70, 0},   //S
  {0, 64, 64, 127, 64, 64, 0},  //T
  {0, 126, 1, 1, 1, 126, 0},    //U
  {64, 48, 12, 3, 12, 48, 64},  //V
  {0, 126, 1, 2, 1, 126, 0},    //W
  {0, 99, 20, 8, 20, 99, 0},    //X
  {0, 112, 8, 7, 8, 112, 0},    //Y
  {0, 67, 69, 73, 81, 97, 0},    //Z
  {0, 0, 0, 0, 0, 0, 0}, //' '
  {0, 127, 65, 73, 65, 127, 0}, // 0
  {0, 0, 16, 32, 64, 127, 0,}, // 1
  {0, 33, 67, 69, 73, 49, 0}, // 2
  {0, 34, 65, 65, 73, 54, 0}, // 3
  {0, 124, 4, 15, 4, 4, 0}, // 4
  {0, 121, 73, 73, 73, 70, 0}, // 5
  {0, 127, 73, 73, 73, 79, 0}, // 6
  {0, 67, 68,72, 80, 96, 0}, // 7
  {0, 127, 73, 73, 73, 127, 0}, // 8
  {0, 120, 73, 73, 73, 127, 0}, // 9
};

#define messageLen 109
char message [] = "XXXX    868 MHZ AUTOMATIC FSK AND HELL 80 HELLSCHREIBER BEACON TEST abcdefghijklmnopqrstuvwxyz 1234567890    ";
byte messageCnt = 0;
boolean serial_or_beacon = 0;

#define FSK_HELL

#ifdef HELL_80
  #define delay_multiplier 500
#endif
#ifdef FSK_HELL
  #define delay_multiplier 1010
#endif

void setup()
{
  pinMode(MOD_PIN, OUTPUT);
  digitalWrite(MOD_PIN, 1);
  delay(500);
  digitalWrite(MOD_PIN, 0);
  delay(500);
  digitalWrite(MOD_PIN, 1);
  delay(500);
  digitalWrite(MOD_PIN, 0);
  delay(500);
  digitalWrite(MOD_PIN, 1);
  delay(500);
  digitalWrite(MOD_PIN, 0);
  
  Serial.begin(1200);
}

void transition_beep()
{
  digitalWrite(MOD_PIN, 1);
  delay(1000); // wait a second before going back
  digitalWrite(MOD_PIN, 0);
}

void loop()
{
  
  if(Serial.available())
  {
    if(serial_or_beacon == 1)
    {
      transition_beep();
      serial_or_beacon = 0;
    }
    pos = Serial.read();
    serial_or_beacon = 0;
  }
  else
  {
    if(serial_or_beacon == 0)
    {
      transition_beep();
      serial_or_beacon = 1;
      messageCnt = 0;
    }
    if (messageCnt > messageLen)
    {
      messageCnt=0;
    }
    pos = message[messageCnt];
    messageCnt++;
  }
  
  if(pos >= 'a' && pos <= 'z')
  {
    pos-=32;
  }
  if (pos == ' ')
  {
    pos = 26;
  }
  else if (pos >= 'A' && pos <= 'Z')
  {
    pos = pos-'A';
  }
  else if (pos >= '0' && pos <= '9')
  {
    pos -= '0';
    pos += 27;
  }
  else
  {
    pos = 26;
  }
  
  cnt = 0;
    
  while(cnt < pixel)
  {
    arrayRead = Letters[pos][cnt];
    row = 0;
    while(row < pixel+1)
    {
     if(arrayRead % 2)
     {
       //tone(13, 900);
       digitalWrite(MOD_PIN, 1);
     }
     else
     {
       //noTone(13);
       digitalWrite(MOD_PIN, 0);
     }
     arrayRead = arrayRead/2;
     //delayMicroseconds(pixel*1010);
     delayMicroseconds(pixel*delay_multiplier);
     row++;
    }
    cnt++;
  }
  digitalWrite(MOD_PIN, 0); 
  
 }
 