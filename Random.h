/*
Original RNG circuit and code by Rob Seward 2008-2009
Project Page: http://robseward.com/misc/RNG2/
License: http://creativecommons.org/licenses/by-nc/2.5/
*/

/*
Random.h - A library for reading and de-biasing input from a hardware random bit generator.
Author: Derek Chafin
Version: 1.5
Modified: October 24, 2012 
Project Page: http://www.coding-squared.com/blog/2011/12/arduino-hardware-random-number-generator/
License
This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
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
