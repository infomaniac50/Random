##Arduino Random Library

###Author
Derek Chafin  
Version: 1.5  
Modified: October 24, 2012  
Website: [http://coding-squared.com/blog/][blog]  
Project Page: [Arduino Hardware Random Number Generator][blogrng]  

###Attribution
Original RNG circuit and code by Rob Seward 2008-2009  
Website: [http://robseward.com/][robmain]  
Project Page: [http://robseward.com/misc/RNG2/][robrng]  
License: [Creative Commons Attribution-NonCommercial 2.5 Generic License](http://creativecommons.org/licenses/by-nc/2.5/)  

###Description
A library for reading and de-biasing input from a hardware random bit generator.
This library reads output from a random bit generator using the ADC on the Arduino.
Uses avalanche breakdown from a reversed biased transistor to create quantum noise fluctuations.
The noise is amplified and then read as an analog voltage on the Arduino.
It has two bias removal methods, exclusive or (XOR) and Von Neumann code, as well the option for no bias removal at all.  

###License
This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.  
To view a copy of this license, visit [http://creativecommons.org/licenses/by-sa/3.0/](http://creativecommons.org/licenses/by-sa/3.0/).  

[blog]: http://www.coding-squared.com/blog/ "Coding Squared"
[blogrng]: http://www.coding-squared.com/blog/2011/12/arduino-hardware-random-number-generator/ "Arduino Hardware Random Number Generator"
[robmain]: http://robseward.com/ "Rob Seward"
[robrng]: http://robseward.com/misc/RNG2/ "RNG Version 2"