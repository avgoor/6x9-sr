#define NROWS 6
#define NCOLS 9

#define LATCH 2
#define SER 3
#define CLK 4

/* the matrix is a "frame-buffer",
 * each byte here represents a column
 * but since the display has only 6 rows
 * 2 less significant bits are useless
 */
byte matrix[NCOLS] = {
    0b11111100,
    0b10100100,
    0b10100100,
    0b10011100,
    0b10000100,
    0b11100100,
    0b10100100,
    0b10100100,
    0b11111100};

void setup()
{
  pinMode(LATCH, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(CLK, OUTPUT);
  //Serial.begin(9600);
}

void push_rnd(){
  unsigned long int randomNumber = random(0,65535);
  for (int i = 0; i < NCOLS; i++){
    matrix[i] = matrix[i] >> 1; 
    matrix[i] |= (randomNumber >> i) << 7;
  };
};

void draw_cycle()
{
  /* drawing by columns
   * 6 less significant bits are rows addresses
   * all others are columns */
  unsigned int data = 0b0000001111111111;
  for (int y = 0; y < NCOLS; y++){
    data = 0b0000001111111111;
    digitalWrite(LATCH, LOW);
    data |= (matrix[y] & 0xFC) << 8;
    data ^= (1 << (1 + y)); // activating y-th column; it's reversed since screen is reversed
/*
     char strx[64];
        sprintf(strx, "%08X:00ff=(%02X):ff00=(%02X)\n", data, data & 0x00FF, data & 0xFF00);
        Serial.write(strx);
*/   
    shiftOut(SER, CLK, LSBFIRST, data);
    shiftOut(SER, CLK, LSBFIRST, (data >> 8));
    digitalWrite(LATCH, HIGH);
    //delay(2);
  };
};

void loop()
{
  push_rnd();
  for (int i = 0; i < 200; i++)
  {
    draw_cycle();
  };
}
