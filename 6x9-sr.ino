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
  0x60,0x90,0x90,0x88,0x44,0x88,0x90,0x90,0x60
};

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(CLK, OUTPUT);
  //Serial.begin(9600);
}

/*
  This is a silly function that pushes random dots on top of the matrix
  and shifts its content down making some unspectacular effect of falling dots
*/
void push_rnd() {
  unsigned long int randomNumber = random(0, 65535);
  for (short int i = 0; i < NCOLS; i++) {
    matrix[i] = matrix[i] >> 1;
    matrix[i] |= (randomNumber >> i) << 7;
  };
};

void rollDown() {
  byte tmp;
  for (short i = 0; i < NCOLS; i++){
    tmp = (matrix[i] & 0x1) << 7; // take the last line and shift it up to the MSB
    matrix[i] = matrix[i] >> 1;
    matrix[i] |= tmp;
  };
};

void rollUp() {
  byte tmp;
  for (short i = 0; i < NCOLS; i++){
    tmp = (matrix[i] & 0x80) >> 7; // take the first line and shift it up to the LSB
    matrix[i] = matrix[i] << 1;
    matrix[i] |= tmp;
  };
};

void draw_cycle() {
  /* drawing by columns
     6 most significant bits are rows addresses
     all others are columns except the first one */
  byte data_msb, data_lsb;
  for (short y = 0; y < NCOLS; y++) {
    data_msb = 0b00000011;
    data_lsb = 0b11111111;
    digitalWrite(LATCH, LOW);
    data_msb |= (matrix[y] & 0xFC);   // fill the MSB with the row's data
    if (y > 6) {
      data_msb &= ~(0x01 << (y - 7)); // columns are split between two shift registers
    } else {
      data_lsb &= ~(0x01 << (1 + y)); // activate the Y-th column
                                      // it's reversed since the screen is reversed
    };
    shiftOut(SER, CLK, LSBFIRST, data_lsb);
    shiftOut(SER, CLK, LSBFIRST, data_msb);
    digitalWrite(LATCH, HIGH);
  };
};

short count = 0;
short direction = 0; // 0 - down, 1 - up

void loop() {
  // this cycle works as a delay
  for (short i = 0; i < 50; i++) {
    draw_cycle();
  };
  if (count++ % 8 == 0) direction = direction > 0 ? 0 : 1;
  if (0 == direction) {
    rollDown();
  } else {
    rollUp();
  };
}

