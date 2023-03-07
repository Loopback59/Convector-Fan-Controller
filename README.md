# Convector-Fan-Controller
Controls the speed of a standard PC cooling fan depending on the heating water temperature.

The device controls a PC fan in an area you heating with a water based distributionsystem.

<b>Function</b><br>
The controller measures the temperature of the heatingwater and the incoming air to the fan.
If the fans incoming air (AirTempIn) is greater then ex 25 degrees the fan stops because the room is hot enough.
If the heating water is above 35 degrees the the fan is allowed to run. The fan speed is calculated from the differance between AirTempIn and MaxAirTemp. <br>Example:<br>
Room temp is 18 degrees and heating water has reached 38 degrees and you set the MaxAirTemp to 25: 25-20 = 5. We run fan at speed 5.
The acual PWM speed is i simple table of 1-5 PWM speeds where 5 is 100% speed and 1 is 10%.
The result is a fan at high speed and noice when you need to increase the heat a lot and a silent fan when it quit ok temperature :-)

<b>Hardware</b><br>
I guess all 4-pin fans for PC has the fourth pin as a PWM input. When hold to ground the fan stops in the most cases but I have notice that som fans still runs very slow even when the PWM input is grounded so I have added a FET transistor to switch off the power to the fan to save power. This can save som 20-60 mA @ 12V.

<b>Controller</b><br>
Any works. I've use some old ones that was tricky to program so I looked and found and fell in love with PIC16F18424 - just 14-pins! (like an ooold TTL DIP :-) So the attached schematics is based on this device. The MPLAB IDE and it's MCC makes life simple with this actually quite complicated PIC.

<b>Platform</b><br>
I have made four controllers so far with different u-controllers, bluetooth and wifi capabilities. All wireless methods consumes "a lot of power" ex a ESP-01 (ESP8266) is idleling at 60mA or 0.7W. I personally don't need to have a constant and exact info about room temperature in a sailing boat.

<b>Improvements</b><br>
PID calculations. Attractive but I have based my fan speeds on the noice level from the fans - which overcomplicates a PID solution for me.
Removable serial interface seems very effective as you can set speed and temperature parameters and then remove the interface. The serial interface could of course be a simple cable (watch the voltage!), bluetooth or wifi etc.<br>
A sleep routine would save some more mA but it should also wake up on serial interface use, hmmm...




