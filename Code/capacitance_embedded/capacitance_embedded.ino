//Author Name:NITHIN EISWAR.V
//Domain:EMBEDDED AND ELECTRONICS
//Functions:usart_init(),usart_write(),ADC_START()
//Variables:k

#include<avr/io.h>
#include<avr/interrupt.h>

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
double k = 0.0;//to track number of times timer 0 overflow occurs

//predefined function which is called when timer 0 overflows
ISR(TIMER0_OVF_vect) {
    ++k;
}
//Function Name:Timer_enable
//Description:to start the timer,no prescaling,and enabling overflow interrupt
//Example call:Timer_enable();
void Timer_enable()
{ TCCR0A = 0x00;
   TCCR0B= (1<<CS00);//No prescaling
   TIMSK0 = (1<<TOIE0);//Enabling overflow interrupt
}
int main()
{int x=0;

float resistorValue = 10000.0; //Remember, we've used a 10K resistor to charge the capacitor
double time1=0.0;//Time constant of the capacitor
float microFarads; 
float Farads;               
float nanoFarads;
int n=0;
DDRB=0b00100000;//Pin D13 is made as output pin 
PORTB=0x00;
Serial.begin(9600);//beginning the serial communication with a fixed baudrate=9600
ADC_START();
usart_init();
while(1)
  {k=0.0;
   PORTB=1<<5;//Making D13 pin high(5V)
   sei();//Enabling interrupt
   Timer_enable();
   TCNT0=0;//Starting the count

      while(x < 648)
      {
        while(ADCSRA&(1<<ADIF)==0);//condition to check whether ADC conversion is complete
        x=ADC;       
      }
    time1=(k/62500)+(TCNT0/16000000); 
    Farads = ((float)time1 / resistorValue) ;
    microFarads=Farads*1000000;
    n++;
    if (n < 2)
    { send_string("");
    }

    else {
    send_string("CAPACITANCE=");
    if (microFarads > 1)
    {  
      String stringVal = "";
      stringVal += String(int(microFarads)) + "." + String(getDecimal(microFarads)); //combining both whole and decimal part in string with a fullstop between them
      char charVal[100];                      //initialise character array to store the values
      for (uint8_t i = 0; i < sizeof(charVal); i++) //passing the value of the string to the character array
      {
        charVal[i] = stringVal[i];
      }
      for (uint8_t i = 0; i < sizeof(charVal); i++) //display character array
      {
        usart_write(charVal[i]);
      }    
       send_string("uF");   
       _delay_ms(100);    
     }
    else
    {
       nanoFarads = microFarads * 1000.0; 
       String stringVal = "";
      stringVal += String(int(nanoFarads)) + "." + String(getDecimal(nanoFarads)); //combining both whole and decimal part in string with a fullstop between them
      char charVal[100];                      //initialise character array to store the values
      for (uint8_t i = 0; i < sizeof(charVal); i++) //passing the value of the string to the character array
      {
        charVal[i] = stringVal[i];
      }
      for (uint8_t i = 0; i < sizeof(charVal); i++) //display character array
      {
        usart_write(charVal[i]);
      }  
       send_string("nF");          
       _delay_ms(100); 
     }
    }
  PORTB=0x00;
  DDRB=0b00100001;//Making D8 output pin along with D13
  PORTB=0x00;
  while(x >0)
      {
        while(ADCSRA&(1<<ADIF)==0);//condition to check whether ADC conversion is complete
        x=ADC;       
      }
  DDRB=0b00100000;//D13 is the only output pin and D8 has changed to input pin
  usart_write('\n');
  send_string("DISCHARGING.....");
  usart_write('\n');
  }
  return 0;
}
