/*
Original RNG circuit and code by Rob Seward 2008-2009
Project Page: http://robseward.com/misc/RNG2/
License: http://creativecommons.org/licenses/by-nc/2.5/
*/

/*
Random.cpp - A library for reading and de-biasing input from a hardware random bit generator.
Author: Derek Chafin
Version: 1.5
Modified: October 24, 2012 
Project Page: http://www.coding-squared.com/blog/2011/12/arduino-hardware-random-number-generator/
License
This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
*/


#define DEFAULT_BITS 0b11

#include "Random.h"
//begin public constuctors
Random::Random(int adc_pin, debias_method bias_removal){
  _adc_pin = adc_pin;
  _bias_removal = bias_removal;
}
//end public constructors

//begin private methods
byte Random::read_input(){
  //read the pin
  int adc_value = analogRead(_adc_pin);
  
  //return the two least significant bits
  return (byte)(adc_value && DEFAULT_BITS);
}

unsigned int Random::find_threshold(unsigned int* bins){
  unsigned long half;
  unsigned long total = 0;
  unsigned int i;
  
  //add the total counts from all bins
  for(i=0; i < BINS_SIZE; i++){
    total += bins[i];
  }
  
  //divide by two to get half of the total count
  half = total >> 1;
  //zero the total again
  total = 0;
  //for all the bins
  for(i=0; i < BINS_SIZE; i++){
    //keep adding up the total
    total += bins[i];
    //until the total is greater than half
    if(total > half){
      //exit the loop
      break;
    }  
  }
  //return the bin that is at half count
  return i;
}

byte Random::exclusive_or(byte input){
  static boolean flip_flop = 0;
  flip_flop = !flip_flop;
  return flip_flop ^ input;
}

byte Random::von_neumann(byte input){
  static boolean previous = 0;
  static boolean flip_flop = 0;
  byte output;
  
  if(flip_flop){
    if(input == 1 && previous == 0){
      output = 0b0;  
      flip_flop = !flip_flop;
    }
    else if (input == 0 && previous == 1){
      output = 0b1;
      flip_flop = !flip_flop;
    }
    else
    {
      output = 0b11;
    }
  }
  else
  {
    output = 0b11;
    previous = input;
    flip_flop = !flip_flop;
  }
  
  return output;
}

inline byte Random::debias(byte input){
  //do bias removal
  switch(_bias_removal){
    case VON_NEUMANN:
      return von_neumann(input);
    case EXCLUSIVE_OR:
      return exclusive_or(input);
    case NO_BIAS_REMOVAL:
      return input;        
  }
}
//end private methods

//begin public methods
unsigned int Random::calibrate(){
  unsigned int* bins = (unsigned int*)malloc(BINS_SIZE * sizeof(unsigned int));
  
  //set all value counts to zero  
  for(int i = 0; i < BINS_SIZE; i++)
  {
    bins[i] = 0;
  }
  
  //loop until i equals the calibration sample size 
  for(unsigned int i = 0; i < CALIBRATION_SIZE; i++)
  {
    //get a sample
    byte adc_byte = read_input();
    //add one count to the bin of the sample value
    bins[adc_byte]++;
  }
  
  //find the threshold
  _threshold = find_threshold(bins);
  
  free(bins);
  
  return _threshold;
}

byte Random::get_bit(){
  byte output_bit = 0b11;

  //keep reading bits
  while(output_bit == 0b11)
  {
    //read the rng and debias if nescessary
    output_bit = debias((read_input() < _threshold) ? 0b1 : 0b0);    
  }
  return output_bit;
}

boolean Random::get_boolean(){
  byte part = get_bit();
  return part == 0b1;
}

byte Random::get_byte(){
  byte number = 0b0;
  
  for (int i = 0; i < sizeof(byte); i++)
  {
    byte part = get_bit();
    number |= part;
    number <<= 1;
  }
  
  return number;
}

int Random::get_int(){
  int number = 0b0;
  
  for (int i = 0; i < sizeof(int) / sizeof(byte); i++)
  {
    byte part = get_byte();
    number |= part;
    number <<= sizeof(byte);
  }
  
  return number;
}

long Random::get_long(){
  long number = 0b0;
  
  for (int i = 0; i < sizeof(long) / sizeof(byte); i++)
  {
    byte part = get_byte();
    number |= part;
    number <<= sizeof(byte);
  }
  
  return number;
}
//end public methods