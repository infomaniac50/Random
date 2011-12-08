/*
  Random.cpp - Library for reading and debiasing input from a hardware random number bit generator
  Created by Derek Chafin, December 5, 2011
  Based on the program RNG v2 by Rob Seward
  Released into the public domain.
*/

#define BINS_SIZE 256
#define CALIBRATION_SIZE 50000
#define DEFAULT_BITS 2

#include "Arduino.h"
#include "Random.h"

Random::Random(int adc_pin, debias_method bias_removal)
{
  _adc_pin = adc_pin;
  _bias_removal = bias_removal;
}

byte Random::readInput()
{
  //read the pin
  int adc_value = analogRead(_adc_pin);
  
  //get the two least significant bits
  byte adc_byte = adc_value >> DEFAULT_BITS;
  
  return adc_byte;
}

byte Random::process(){
  _byte_ready = false;
  while (!_byte_ready)
  {
    byte adc_byte = readInput();
    boolean input_bool;
  for (int i=0; i < DEFAULT_BITS; i++)
  {
    input_bool = bitRead(adc_byte, i);

      switch(_bias_removal){
      case VON_NEUMANN:
      vonNeumann(input_bool); 
      break;
      case EXCLUSIVE_OR:
        exclusiveOr(input_bool);
      break;
      case NO_BIAS_REMOVAL:
      buildByte(input_bool);
      break;
      }
  }	
  }
  
  return _random_bits;
}

void Random::exclusiveOr(byte input){
  static boolean flip_flop = 0;
  flip_flop = !flip_flop;
  buildByte(flip_flop ^ input);
}

void Random::vonNeumann(byte input){
  static int count = 1;
  static boolean previous = 0;
  static boolean flip_flop = 0;

  flip_flop = !flip_flop;

  if(flip_flop){
    if(input == 1 && previous == 0){
      buildByte(0);
    }
    else if (input == 0 && previous == 1){
      buildByte(1); 
    }
  }
  previous = input;
}

void Random::buildByte(boolean input){
  static int byte_counter = 0;
  static byte out = 0;

  if (input == 1){
    out = (out << 1) | 0x01;
  }
  else{
    out = (out << 1); 
  }
  byte_counter++;
  byte_counter %= 8;

  if(byte_counter == 0){
  _byte_ready = true;
  _random_bits = out;
  }
}