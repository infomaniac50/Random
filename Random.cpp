/*
  Random.cpp - Library for reading and debiasing input from a hardware random number bit generator
  Created by Derek Chafin, December 5, 2011
  Based on the program RNG v2 by Rob Seward
  Released into the public domain.
*/


#define DEFAULT_BITS 2

#include "Random.h"

Random::Random(int adc_pin, int status_pin, debias_method bias_removal)
{
  _adc_pin = adc_pin;
  _bias_removal = bias_removal;
  _status_pin = status_pin;
  pinMode(_status_pin, OUTPUT);
}

void Random::calibrate(byte adc_byte){
  _bins[adc_byte]++;
}

unsigned int Random::findThreshold(){
  unsigned long half;
  unsigned long total = 0;
  int i;

  for(i=0; i < BINS_SIZE; i++){
    total += _bins[i];
  }	

  half = total >> 1;
  total = 0;
  for(i=0; i < BINS_SIZE; i++){
    total += _bins[i];
    if(total > half){
      break;
    }	
  }
  return i;
}

void Random::blinkLed(){
  digitalWrite(_status_pin, HIGH);
  delay(100);
  digitalWrite(_status_pin, LOW);
}

int Random::calibrate()
{
  unsigned int increment = CALIBRATION_SIZE / 10;
  unsigned int num_increments = 0; //progress units so far
  unsigned int threshold;
  
  for(int i = 0; i < BINS_SIZE; i++)
  {
    _bins[i] = 0;
  }
  
  for(unsigned int i = 0; i < CALIBRATION_SIZE; i++)
  {
    threshold = (num_increments + 1) * increment;
    if(i > threshold){
      num_increments++;
      //Serial.print("*");
      blinkLed();
    }
    
    byte adc_byte = readInput();
    calibrate(adc_byte);
  }
  
  _threshold = findThreshold();
  Serial.println(_threshold);
  return _threshold;
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
  unsigned long start_time = millis();
  

  while (!_byte_ready)
  {
    byte adc_byte = readInput();
    boolean input_bool;
//    for (int i=0; i < DEFAULT_BITS; i++)
//    {
//      input_bool = bitRead(adc_byte, i);
    input_bool = (adc_byte < _threshold) ? 1 : 0;
    
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
//    }	
  }
  
  unsigned long end_time = millis();
  
  if ((end_time - start_time) > 250)
  {
    calibrate();
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