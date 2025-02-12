// Define LCD module connections.
sbit LCD_RS at RD1_bit;                 //Connect RS pin of LCD to RD1 pin of the MCU
sbit LCD_EN at RD3_bit;                 //Connect EN pin of LCD to RD3 pin of the MCU
sbit LCD_D4 at RD4_bit;                 //Connect D4 pin of LCD to RD4 pin of the MCU
sbit LCD_D5 at RD5_bit;                 //Connect D5 pin of LCD to RD3 pin of the MCU
sbit LCD_D6 at RD6_bit;                 //Connect D6 pin of LCD to RD3 pin of the MCU
sbit LCD_D7 at RD7_bit;                 //Connect D7 pin of LCD to RD7 pin of the MCU

// Pin direction
sbit LCD_RS_Direction at TRISD1_bit;    //Direction of RS pin
sbit LCD_EN_Direction at TRISD3_bit;    //Direction of EN pin
sbit LCD_D4_Direction at TRISD4_bit;    //Direction of data D4 pin
sbit LCD_D5_Direction at TRISD5_bit;    //Direction of data D5 pin
sbit LCD_D6_Direction at TRISD6_bit;    //Direction of data D6 pin
sbit LCD_D7_Direction at TRISD7_bit;    //Direction of data D7 pin
// End LCD module connections

 sbit IR_Tx at RA3_bit;

//varaible for frequency measure
 unsigned long RPM_Value,pk;
 unsigned int low_bit,high_bit;
 char        *RPM = "000Hz";
 
 
 //varaible for voltage measure
 int adc=0,x=0;
 float total_adc=0;
 float avg_adc_value=0, volts=0;
 char vol[10];
 char vol_cu[15];
 
//variable for current measurement
 int adc_cu=0,y=0;
 float cu_total_adc;
 float cu_avg_adc=0,current_volt=0,current_value=0;

 
 
 //function for display frequency
 void Display_RPM(unsigned long num){
  RPM[0] = num/100 + 48;
  RPM[1] = (num/10)%100 + 48;
  RPM[2] = (num/1)%10 + 48;
  Lcd_Out(1,12,"F:");
  Lcd_Out(1,14,RPM);  //****lcd 2nd row//
 }
 //function for frequency measurement
 void init(){
          ADCON1 = 0x0F;  // Disable Analog functions
          TRISC = 0x00;
          TRISB = 0x00;
          PORTA = 0x00;
          TRISA = 0b00010000;
          T0CON = 0b01101000; // TMR0 as 16-bit counter


           T0CON.TMR0ON = 1;
           TMR0L = 0;
           TMR0H = 0;
           IR_Tx = 1;

           Delay_ms(1000); // Wait for 1 sec
           IR_Tx = 0;
           T0CON.TMR0ON = 0;    // Stop the timer
           RPM_Value = (256*TMR0H + TMR0L);
           Display_RPM(RPM_Value);

 }
 //function for adc measurement
   float voltage(){
         adcon0=0b10000000;
         adcon1=0b11000000;
         TRISA=0b11111111;
         delay_ms(2);
         adc_init();
         delay_ms(3);
         total_adc=0;
         avg_adc_value=0;
   for(x=0;x<100;x++){
               adc=0;
               adc=adc_read(1);
               v_i[x]=adc;
               delay_ms(5);
               total_adc=(total_adc+adc);
             }

      avg_adc_value=(float)(total_adc/100);
        //convert to voltage
      volts=((float)(avg_adc_value*(230.00/3.78)*(5.00/1023.00)*2.38));
      if(volts<20){
      volts=0;
      }else{
      volts=volts;
      }
      floatTostr(volts,vol);
      lcd_out(2,1,"V:");
      lcd_out(2,3,vol);  ///****1st row 8 clmn///

       return   volts;
 }
 float current(){

         y=0;
         adc_cu=0;
         cu_total_adc=0;
         cu_avg_adc=0;
         current_volt=0;
         adc_init();
         delay_ms(20);
         
       for(y=0;y<100;y++){
               adc_cu=0;
               adc_cu=adc_read(2);
               delay_ms(5);
               I_i[y]=((adc_cu*5)/1023);
               cu_total_adc=(cu_total_adc+adc_cu);
             }
       cu_avg_adc=(float)(cu_total_adc/100);
        //convert to voltage
      current_volt=(abs)((float)(cu_avg_adc*(5.00/1023.00))); //in mV
      current_value = (float)(current_volt*6);
      floatTostr(current_volt,vol_cu);
      lcd_out(1,1,"I:");
      lcd_out(1,3,vol_cu);  ///****1st row 3 clmn///
       return current_value;
      }
 
 void main() {
          Lcd_Init();                       // Initialize LCD
          Lcd_Cmd(_LCD_CLEAR);             // CLEAR display
          Lcd_Cmd(_LCD_CURSOR_OFF);        // Cursor off


         while(1){

           init();
           delay_ms(1000);
           Vlt=voltage();
           delay_ms(20);
           Ict=current();
           delay_ms(1000);


         }
}