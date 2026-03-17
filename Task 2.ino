#include <Arduino.h>

uint8_t A, B, R, S;

void setup() {
  Serial.begin(9600);
}

void loop() {

  if (!Serial.available())
    return;

  A = Serial.parseInt();   
  B = Serial.parseInt();   

  while (Serial.available())
    Serial.read();

  asm volatile(

    "mov r20, %[a]        \n\t"   
    "mov r21, %[b]        \n\t"   

    "sub r20, r21         \n\t"   

    "in  r22, __SREG__    \n\t"   

    "mov %[res], r20      \n\t"   
    "mov %[sreg], r22     \n\t"   

    : [res] "=r"(R), [sreg] "=r"(S)   
    : [a] "r"(A), [b] "r"(B)          
    : "r20", "r21", "r22"             
  );

  Serial.print("A=");
  Serial.print(A);

  Serial.print(" B=");
  Serial.print(B);

  Serial.print(" R=");
  Serial.print(R);

  Serial.print(" Z=");
  Serial.print(S<<1 &1);   

  Serial.print(" C=");
  Serial.println(S & 1);        

}


