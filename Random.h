/*
  Random.h - Library for reading and debiasing input from a hardware random number bit generator
  Created by Derek Chafin, December 5, 2011
  Based on the program RNG v2 by Rob Seward
  Released into the public domain.
*/

#ifndef Random_h
#define Random_h

#include "Arduino.h"

enum debias_method
{
  NO_BIAS_REMOVAL,
  EXCLUSIVE_OR,
  VON_NEUMANN
};

class Random
{
  public:
    Random(int adc_pin, debias_method bias_removal);
    byte process();
  private:
    void exclusiveOr(byte input);
    void vonNeumann(byte input);
    void buildByte(boolean input);
    byte readInput();
    debias_method _bias_removal;
    boolean _byte_ready;
    byte _random_bits;
    int _adc_pin;
};
#endif