/*
  Random.h - Library for reading and debiasing input from a hardware random number bit generator
  Based on the program RNG v2 by Rob Seward http://robseward.com/misc/RNG2/
  Created by Derek Chafin
  October 24, 2012
  License: Attribution-NonCommercial 3.0 Unported
*/

#ifndef Random_h
#define Random_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

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
  //public constructors
  public:
    Random(int adc_pin, debias_method bias_removal);
  //private methods
  private:    
    byte read_input();
    unsigned int find_threshold(unsigned int * bins);
    byte exclusive_or(byte input);
    byte von_neumann(byte input);
    byte debias(byte input);
  //private vars
    debias_method _bias_removal;
    int _adc_pin;
    unsigned int _threshold;
  //public methods
  public:
    unsigned int calibrate();
    byte get_bit();
    boolean get_boolean();
    byte get_byte();
    int get_int();
    long get_long();
};
#endif
