/*
  Random.h - Library for reading and debiasing input from a hardware random number bit generator
  Created by Derek Chafin, December 5, 2011
  Based on the program RNG v2 by Rob Seward
  Released into the public domain.
*/

#ifndef Random_h
#define Random_h

#include "Arduino.h"

#define BINS_SIZE 256
#define CALIBRATION_SIZE 50000

enum debias_method
{
  NO_BIAS_REMOVAL,
  EXCLUSIVE_OR,
  VON_NEUMANN
};

class Random
{
  public:
    Random(int adc_pin, int status_pin, debias_method bias_removal);
    int calibrate();
    byte process();
  private:
    void blinkLed();
    void calibrate(byte adc_byte);
    unsigned int findThreshold();
    void exclusiveOr(byte input);
    void vonNeumann(byte input);
    void buildByte(boolean input);
    byte readInput();
    debias_method _bias_removal;
    boolean _byte_ready;
    byte _random_bits;
    int _status_pin;
    int _adc_pin;
    byte _threshold;
    unsigned int _bins[BINS_SIZE];
};
#endif
