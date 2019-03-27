#define NROWS 6
#define NCOLS 9

#define LATCH 2
#define SER   3
#define CLK   4


char matrix[NROWS][NCOLS] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 1, 0, 0, 1},
  {1, 1, 1, 0, 0, 1, 1, 1, 1},
  {1, 0, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1}
};


void setup() {
  pinMode (LATCH, OUTPUT);
  pinMode (SER, OUTPUT);
  pinMode (CLK, OUTPUT);
  //  Serial.begin(9600);
}

void push_rnd() {
  char tmp;
  for (int i = NROWS - 1; i > 0; i--) {
    for (int j = 0; j < NCOLS; j++) {
      tmp = matrix[i - 1][j];
      matrix[i][j] = tmp;
    };
  };
  matrix[0][0] = random(0, 2);
  matrix[0][1] = random(0, 2);
  matrix[0][2] = random(0, 2);
  matrix[0][3] = random(0, 2);
  matrix[0][4] = random(0, 2);
  matrix[0][5] = random(0, 2);
  matrix[0][6] = random(0, 2);
  matrix[0][7] = random(0, 2);
  matrix[0][8] = random(0, 2);
};


void draw_cycle() {
  // drawing by columns
  unsigned int data = 0b1111111111000000; // first 6 bits are rows addresses all others are columns
  for (int y = 0; y < NCOLS; y++) {
    data = 0b1111111111000000;
    digitalWrite(LATCH, LOW);
    data |= matrix[0][y] << 0;
    data |= matrix[1][y] << 1;
    data |= matrix[2][y] << 2;
    data |= matrix[3][y] << 3;
    data |= matrix[4][y] << 4;
    data |= matrix[5][y] << 5;
    data ^= (1 << (14 - y)); // activating y-th column
    /*    char strx[64];
        sprintf(strx, "%08X:00ff=(%02X):ff00=(%02X)\n", data, data & 0x00FF, data & 0xFF00);
        Serial.write(strx);
    */
    shiftOut(SER, CLK, MSBFIRST, (data >> 8));
    shiftOut(SER, CLK, MSBFIRST, data);
    digitalWrite(LATCH, HIGH);
    //delay(2);
  };
};


void loop() {
  push_rnd();
  for (int i = 0; i < random(100, 200); i++) {
    draw_cycle();
  };
}
