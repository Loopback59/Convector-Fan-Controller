/*
        Device            :  PIC16F18424
        Driver Version    :  2.00
*/


#include "mcc_generated_files/mcc.h"
#include "print.h"
#include "stdio.h"

volatile unsigned char seconds = 58;
volatile unsigned char minutes = 59;
volatile unsigned char hours = 23;
volatile unsigned char timechanged = 0;

uint8_t  I2C1_Read1ByteRegister(i2c1_address_t address, uint8_t reg);
uint16_t I2C1_Read2ByteRegister(i2c1_address_t address, uint8_t reg);
void I2C1_Write1ByteRegister(i2c1_address_t address, uint8_t reg, uint8_t data);
void I2C1_Write2ByteRegister(i2c1_address_t address, uint8_t reg, uint16_t data);
void I2C1_WriteNBytes(i2c1_address_t address, uint8_t *data, size_t len);
void I2C1_ReadNBytes(i2c1_address_t address, uint8_t *data, size_t len);
void I2C1_ReadDataBlock(i2c1_address_t address, uint8_t reg, uint8_t *data, size_t len);


#define AIRSENSOR       0x48
#define WATERSENSOR     0x49

// Should be in EEPROM later on
unsigned char FanOffTemp = 25;    // CMD = A, If room or Air temp is above 25 degrees we turn the fan off
unsigned char WaterTempMIN = 30;  // CMD = B, If watertemp are below this value the fan wont run at all
bool Stop = false;  //

uint16_t eeAddress = 0xF000; 
        

uint8_t ReadTemp(unsigned char sensor)
{
    uint8_t value;

    //I2C1_Write1ByteRegister(sensor,0, 0);
    //value = I2C1_Read2ByteRegister(sensor, 0);
    value = I2C1_Read1ByteRegister(sensor, 0);
    //value = I2C1_Read2ByteRegister(sensor, 1);
    // convert the two bytes to one int. 11 bits
    //tmp = (int) (((I2CBuffer[0]<<8) + I2CBuffer[1]) >> 8);

    // Skip the second byte! We only need whole degrees.
    return value;
}

void printMenu(void) {
    print_crlf();
    putstrln("------------------  M E N U E --------------------");
    putstrln("S, Stop or start fan");
    putstrln("+, increase max temp normally 5-20-33 degrees (Tm)");
    putstrln("-, decrease max temp");
    putstrln("W, increase min-watertemp too start fan (Wm)");
    putstrln("Q, decrease min-watertemp");
    putstrln("?, Help - this text");
    putstrln("--------------------------------------------------");
    print_crlf();
}


void main(void)
{
    uint8_t  AirTemp, WaterTemp, c;
    uint16_t DC = 0, Diff;
    
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    //INTERRUPT_GlobalInterruptDisable();
    INTERRUPT_PeripheralInterruptEnable();
    //INTERRUPT_PeripheralInterruptDisable();

    putstr("\nHello World!\n");
 
            
    FanOffTemp = DATAEE_ReadByte(eeAddress);
    if (FanOffTemp == 255) FanOffTemp = 25;     // When programming EEPROM gets 0xFF
    WaterTempMIN = DATAEE_ReadByte(eeAddress + 1);
    if (WaterTempMIN == 255) WaterTempMIN = 35;

    printMenu();
    
            
    while (1)
    {
        AirTemp = ReadTemp(AIRSENSOR);
        WaterTemp = ReadTemp(WATERSENSOR);

        if ((WaterTemp > WaterTempMIN) && (!Stop)) {
            if (AirTemp < FanOffTemp) { // We don't run fan if room air is hotter then 25 degrees.
                Diff = FanOffTemp - AirTemp;
                if (Diff >= 1)  DC =  100;  // DC Value is in % with 1 decimal 500 = 50.0 %
                if (Diff >= 2)  DC =  400;  // 40%
                if (Diff >= 3)  DC =  600;  // 60%
                if (Diff >= 4)  DC =  800;
                if (Diff >  5)  DC = 1000;
                IO_RC2_SetHigh();
            } else {
                DC = 0;  // Stop Fan
                IO_RC2_SetLow();
            }
        } else {
            PWM6_LoadDutyValue(0);
            DC = 0;  // Stop Fan - heater not on
            IO_RC2_SetLow();
        }

        PWM6_LoadDutyValue(DC);

        print_byte_2dec(hours);         putchar(':');
        print_byte_2dec(minutes);       putchar(':');
        print_byte_2dec(seconds);
 
        putstr(" Air: ");      print_int_dec(AirTemp);
        putstr(" Water: ");   print_int_dec(WaterTemp);
        putstr(" DC: ");           print_int_dec(DC/10);
        putstr("% FanOffTemp=");   print_byte_2dec(FanOffTemp);
        putstr(" WaterTempMIN="); print_byte_2dec(WaterTempMIN);
        print_crlf();

        bool CommandGiven = false;
        if (EUSART1_is_rx_ready()) {
            c = EUSART1_Read();
            CommandGiven = true;
            print_crlf();
            putstr("Command = '");
            putchar(c);
            putstrln("'");

            if ( c == '?') 
                printMenu();

            if ( c == 'S')
            {
              Stop = !Stop;
              if (Stop)
                  putstrln("Stop fan");
              else
                  putstrln("Start fan");
            }
            if ( c == '+')
            {
              if (++FanOffTemp > 33)
                FanOffTemp = 33;
            }
            if ( c == '-')
            {
              if (--FanOffTemp < 5)
                FanOffTemp = 5;
            }
            if ( c == 'W')
            {
              if (++WaterTempMIN > 30)
                WaterTempMIN = 30;
            }
            if ( c == 'Q')
            {
              if (--WaterTempMIN < 5)
                WaterTempMIN = 5;
            }

            // Save to EEPROM at every comand given
            DATAEE_WriteByte(eeAddress,     FanOffTemp);
            DATAEE_WriteByte(eeAddress + 1, WaterTempMIN);

            // Clear buffer
            while (EUSART1_is_rx_ready())
              c = EUSART1_Read();
        }    
        
        
        if (!CommandGiven)  // Don't delay if we just gave a command
            __delay_ms(997);

    }
}


