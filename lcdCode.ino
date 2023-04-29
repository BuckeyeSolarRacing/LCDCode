// A portion of this code was borrowed from a very helpful Youtube video, cited below:
// Bill, director. Using Rotary Encoders with Arduino. YouTube, YouTube, 23 Mar. 2019, https://www.youtube.com/watch?v=V1txmR8GXzE. Accessed 22 Apr. 2023. 

// ---------------------------------------------------
// SETUP BELOW
// ---------------------------------------------------

void setup()
{

  // Shift register for SevenSegment
  #define SS_SER_CON 2
  #define SS_CLK_CON 3
  pinMode(SER_CON, OUTPUT);
  pinMode(CLK_CON, OUTPUT);

  // Shift register for LCD
  #define LCD_SER_CON 
  #define LCD_CLK_CON
  pinMode(LCD_SER_CON, OUTPUT);
  pinMode(LCD_CLK_CON, OUTPUT);

  #define FirstLetter 7
  #define SecondLetter 8
  #define ThirdLetter 9
  #define FourthLetter 10

  pinMode(FirstLetter, OUTPUT);
  pinMode(SecondLetter, OUTPUT);
  pinMode(ThirdLetter, OUTPUT);
  pinMode(FourthLetter, OUTPUT);

  #define NumZero 0b00111111
  #define NumOne 0b00000110
  #define NumTwo 0b01011011
  #define NumThree 0b01001111
  #define NumFour 0b01100110
  #define NumFive 0b01101101
  #define NumSix 0b01111101
  #define NumSeven 0b00000111
  #define NumEight 0b01111111
  #define NumNine 0b01100111

  #define ENC_COUNT_REV 48   //CHANGE TO 48 pulses per rotation
  #define ENC_IN 13          //Maybe want to change to a GPIO port without an LED?

  pinMode(ENC_IN, INPUT_PULLDOWN);    //Default value is 0
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);

  Serial.begin(9600);
}

// ---------------------------------------------------
// MAIN LOOP BELOW
// ---------------------------------------------------

int analogInput;
volatile long encoderValue = 0;
int intervalLength = 250;      //1 second "refresh rate"
long previousMillis = millis();
long currentMillis = 0;
int rpm = 0;
float mph = 0;

void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > intervalLength)
  {
    previousMillis = currentMillis;
    rpm = (float)(encoderValue * 60 * 4 / ENC_COUNT_REV);
    mph = rpm * 0.047599888690754;
    encoderValue = 0;
  }  
/*
  Serial.print(encoderValue);
  Serial.print('\n');
  Serial.print(mph);
  Serial.print('\n');
*/
  int CheckDigit = 0;
  for (int DigitIndex = 0; DigitIndex < 4; ++DigitIndex)
  {
    digitalWrite(FirstLetter, 1);
    digitalWrite(SecondLetter, 1);
    digitalWrite(ThirdLetter, 1);
    digitalWrite(FourthLetter, 1);

    CheckDigit = ((int)mph / (int)pow(10, DigitIndex)) % 10;
    if (CheckDigit == 0)
    {
      sevenSegment_shift_out(NumZero, false);
    }
    else if (CheckDigit == 1)
    {
      sevenSegment_shift_out(NumOne, false);
    }
    else if (CheckDigit == 2)
    {
      sevenSegment_shift_out(NumTwo, false);
    }
    else if (CheckDigit == 3)
    {
      sevenSegment_shift_out(NumThree, false);
    }
    else if (CheckDigit == 4)
    {
      sevenSegment_shift_out(NumFour, false);
    }
    else if (CheckDigit == 5)
    {
      sevenSegment_shift_out(NumFive, false);
    }
    else if (CheckDigit == 6)
    {
      sevenSegment_shift_out(NumSix, false);
    }
    else if (CheckDigit == 7)
    {
      sevenSegment_shift_out(NumSeven, false);
    }
    else if (CheckDigit == 8)
    {
      sevenSegment_shift_out(NumEight, false);
    }
    else if (CheckDigit == 9)
    {
      sevenSegment_shift_out(NumNine, false);
    }

    if (DigitIndex == 0)
    {
      digitalWrite(FourthLetter, 0);
    }
    else if (DigitIndex == 1)
    {
      digitalWrite(ThirdLetter, 0);
    }
    else if (DigitIndex == 2)
    {
      digitalWrite(SecondLetter, 0);
    }
    else if (DigitIndex == 3)
    {
      digitalWrite(FirstLetter, 0);
    }

    delay(1);
  }
}

// ---------------------------------------------------
// SUBROUTINES BELOW
// ---------------------------------------------------

/**
Shift out function for Shift Register
DATA: the data
LCD Status: Boolean
  false for SevenSegment
  true for LCD

**/
void shift_out(int data, bool lcd_status)
{
  int i;
  int SER;
  int CLK;

  if (lcd_status) {
    SER = LCD_SER_CON;
    CLK = LCD_CLK_CON;
  } else {
    SER = SS_SER_CON;
    CLK = SS_CLK_CON;
  }


  for(i = 0; i < 8; i++)
  {
    digitalWrite(SER, ((data>>(7-i))&0x01));
    digitalWrite(CLK, 1);
    digitalWrite(CLK, 0);
  }
  digitalWrite(CLK, 1);
  digitalWrite(CLK, 0);
}

void updateEncoder()
{
  encoderValue++;
}
