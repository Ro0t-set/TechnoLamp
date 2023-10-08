 #define LOG_OUT 1 // use the log output function
 #define FHT_N 256 // set to 256 point fht
 #define MIN_RANGE  2
 #define MAX_RANGE  8
 
 #include <math.h>
 #include <FHT.h> // include the library
 unsigned long number_of_cicle;
 int medium_value;
 void setup() {
   Serial.begin(115200); // use the serial port
   TIMSK0 = 0; // turn off timer0 for lower jitter
   ADCSRA = 0xe5; // set the adc to free running mode
   ADMUX = 0x40; // use adc0
   DIDR0 = 0x01; // turn off the digital input for adc0
   pinMode(13, OUTPUT);
 }
 
 void loop() {
  int last_volume = 0;
   while(1) { // reduces jitter
     cli();  // UDRE interrupt slows this way down on arduino1.0
     for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
       while(!(ADCSRA & 0x10)); // wait for adc to be ready
       ADCSRA = 0xf5; // restart adc
       byte m = ADCL; // fetch adc data
       byte j = ADCH;
       int k = (j << 8) | m; // form into an int
       k -= 0x0200; // form into a signed int
       k <<= 6; // form into a 16b signed int
       fht_input[i] = k; // put real data into bins
     }
     fht_window(); // window the data for better frequency response
     fht_reorder(); // reorder the data before doing the fht
     fht_run(); // process the data in the fht
     fht_mag_log(); // take the output of the fht
     sei();
     //Serial.write(255); // send a start byte
     //Serial.write(fht_log_out, FHT_N/2); // send out the data

     medium_value = 0;

     for(int i = MIN_RANGE; i< MAX_RANGE; i++){
      medium_value = medium_value + fht_log_out[i];
     }
     medium_value = medium_value/(MAX_RANGE-MIN_RANGE);

     if ((medium_value - (1500/last_volume) )  > last_volume){
      digitalWrite(13, HIGH);
      //Serial.println(log(medium_value));
      //Serial.println(log(last_volume));
      Serial.println("---------------");
      for(int i = MIN_RANGE; i< MAX_RANGE; i++){
      Serial.println(fht_log_out[i]);
     }
      
     }
     if (number_of_cicle > 10){
      digitalWrite(13, LOW);
      number_of_cicle=0;
     }

     if ((number_of_cicle%2) == 0){
      last_volume = medium_value;
     }

     number_of_cicle++;

     }
  }