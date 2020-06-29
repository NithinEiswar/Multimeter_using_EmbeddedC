//Author Name:NITHIN EISWAR.V
//Domain:EMBEDDED AND ELECTRONICS
//Functions:usart_init(),usart_write(),ADC_START()
#include<avr/io.h>

//Function Name:usart_init
//Description:configure USART at 9600bps, Asynchronous Mode, Normal Speed, No parity, 1 Stop bit and 8 bit of data.
//Example Call:usart_init();
void usart_init()
{
  UCSR0A = 0X00;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  UBRR0 = 103;
}
//Function Name:usart_write
//Description:print character
//Example Call:usart_write('a');
void usart_write(int data)
{
  while ( !(UCSR0A & (1 << UDRE0))) { }
  UDR0 = data;
}
//Function Name:send_string
//Description:print string
//Example Call:send_string("moon");
void send_string(char s[])
{
  int i = 0;

  while (s[i] != 0x00)
  {
    usart_write(s[i]);
    i++;
  }
}
//Function Name:ADC_start
//Description:Starting ADC,using channel 0 with Vref=Vcc with prescaling of 128
//Example Call:ADC_start()
void ADC_START()
{ ADCSRA = (1 << ADEN) | (1 << ADATE); //turning on the adc and enabling auto triggering mode
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //enable prescaler of 128
  ADMUX |= (1 << REFS0); //Vref=Vcc with adc channel 0
  ADCSRA |= (1 << ADSC); //starting ADC
}
//Function Name:getDecimal
//Description:function to extract decimal part of float
//Example call:getDecimal(12.5);
long getDecimal(float val)
{
  int intPart = int(val);
  long decPart = 1000 * (val - intPart); //I am multiplying by 1000 assuming that the foat values will have a maximum of 3 decimal places.
  //Change to match the number of decimal places you need
  if (decPart > 0)return (decPart);       //return the decimal part of float number if it is available
  else if (decPart < 0)return ((-1) * decPart); //if negative, multiply by -1
  else if (decPart = 0)return (00);       //return 0 if decimal part of float number is not available
}
int main(void)
{
  float Vout = 0.00;
  float Vin = 0.00;
  float R1 = 10000.00; // resistance of R1 (10K)
  float R2 = 1000.00; // resistance of R2 (1K)
  float R=10//shunt connected in series with the load
  int val = 0;
  int x = 0;
  int n = 0; //to print values after certain readings to improve accuracy
  Serial.begin(9600);//beginning the serial communication with a fixed baudrate=9600
  ADC_START();
  usart_init();
  while (1)
  {
    while (ADCSRA & (1 << ADIF) == 0);//condition to check whether ADC conversion is complete
    x = ADC;
    Vout = (x * 5.00) / 1024.00; // formula for calculating voltage out
    Vin = Vout / (R2 / (R1 + R2)); // formula for calculating voltage in i.e. GND
    current=Vin/R;
    n++;
    if (n < 2)
    { send_string("");
    }

    else {
      send_string("CURRENT=");
      String stringVal = "";
      stringVal += String(int(current)) + "." + String(getDecimal(current)); //combining both whole and decimal part in string with a fullstop between them
      char charVal[100];                      //initialise character array to store the values
      for (uint8_t i = 0; i < sizeof(charVal); i++) //passing the value of the string to the character array
      {
        charVal[i] = stringVal[i];
      }
      for (uint8_t i = 0; i < sizeof(charVal); i++) //display character array
      {
        usart_write(charVal[i]);
      }
      usart_write('\n');
    }    
    _delay_ms(1000); //for maintaining the speed of the output in serial monitor
  }
  return 0;
}
