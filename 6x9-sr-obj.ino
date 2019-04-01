
#define NROWS 6
#define NCOLS 9

#define LATCH 2
#define SER 3
#define CLK 4

/*
  The Matrix class is a "frame-buffer",
  each byte here represents a column
  but since the display has only 6 rows
  2 less significant bits are useless
*/

class Matrix {
  public:
    Matrix(byte data[NCOLS]);
    void drawCycle();
    void rotatePic();
    byte matrix[NCOLS];
};

Matrix::Matrix (byte data[]) {
  for (short i = 0; i < NCOLS; i++) {
    matrix[i] = data[i];
  };
};

void Matrix::rotatePic() {
  byte tmp;
  for (short i = 0; i < NCOLS; i++){
    tmp = (matrix[i] & 0x1) << 7; // take the last line and shift it up to the MSB
    matrix[i] = matrix[i] >> 1;
    matrix[i] |= tmp;
  };
};

void Matrix::drawCycle() {
  /* drawing by columns
     6 most significant bits are rows addresses
     all others are columns except the first one */
  byte data_msb, data_lsb;
  for (int y = 0; y < NCOLS; y++) {
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

/*
  This is a silly function that pushes random dots on top of the matrix
  and shifts its content down making some unspectacular effect of falling dots
*/
void push_rnd(Matrix *m) {
  unsigned long int randomNumber = random(0, 65535);
  for (int i = 0; i < NCOLS; i++) {
    m->matrix[i] = m->matrix[i] >> 1;
    m->matrix[i] |= (randomNumber >> i) << 7;
  };
};

void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(CLK, OUTPUT);
}

Matrix m ((byte []){0x60,0x90,0x90,0x88,0x44,0x88,0x90,0x90,0x60});

void loop() {
  for (int i = 0; i < 100; i++) {
    m.drawCycle();
  };
  m.rotatePic();
};
