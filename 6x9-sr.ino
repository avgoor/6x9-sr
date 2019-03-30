#define NROWS 6
#define NCOLS 9

#define LATCH 2
#define SER 3
#define CLK 4

/* 
   The matrix variable is a "frame-buffer",
   each byte here represents a column
   but since the display has only 6 rows
   2 less significant bits are useless
*/
byte matrix[NCOLS] = {
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100,
  0b11111100
};

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(CLK, OUTPUT);
  //Serial.begin(9600);
}

/*
  This is a silly function that pushes random dots on top of the matrix
  and shifts its content down making some unspecacular effect of faling dots
*/
void push_rnd() {
  unsigned long int randomNumber = random(0, 65535);
  for (int i = 0; i < NCOLS; i++) {
    matrix[i] = matrix[i] >> 1;
    matrix[i] |= (randomNumber >> i) << 7;
  };
};

void draw_cycle() {
  /* drawing by columns
     6 most significant bits are rows addresses
     all others are columns except the first one */
  unsigned int data = 0b0000001111111111;
  for (int y = 0; y < NCOLS; y++) {
    data = 0b0000001111111111;
    digitalWrite(LATCH, LOW);
    data |= (matrix[y] & 0xFC) << 8;  // fill the MSB with the row's data
    data &= ~(1UL << (1 + y));        // activate the Y-th column
                                      // it's reversed since the screen is reversed
    shiftOut(SER, CLK, LSBFIRST, data);
    shiftOut(SER, CLK, LSBFIRST, (data >> 8));
    digitalWrite(LATCH, HIGH);
  };
};

void loop() {
  // this cycle works as a delay
  for (int i = 0; i < 100; i++) {
    draw_cycle();
  };
  push_rnd();
}
