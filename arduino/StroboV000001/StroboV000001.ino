 #define LOG_OUT 1 // use the log output function
 #define FHT_N 256 // set to 256 point fht
 #define MIN_BASS_RANGE  2
 #define MAX_BASS_RANGE  4

 #define MIN_MED_RANGE  7
 #define MAX_MED_RANGE  10
 
 #include <math.h>
 #include <FHT.h> // include the library
 unsigned long number_of_cicle;
 int medium_bass_value;
 int medium_med_value;
 int last_volume_bass = 0;
 int last_volume_med = 0;
 void setup() {
   Serial.begin(115200); // use the serial port
   TIMSK0 = 0; // turn off timer0 for lower jitter
   ADCSRA = 0xe5; // set the adc to free running mode
   ADMUX = 0x40; // use adc0
   DIDR0 = 0x01; // turn off the digital input for adc0
   pinMode(13, OUTPUT);
   pinMode(12, OUTPUT);
 }
 
 void loop() {
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


     medium_bass_value = 0;
     medium_med_value = 0;

     for(int i = MIN_BASS_RANGE; i< MAX_BASS_RANGE; i++){
      medium_bass_value = medium_bass_value + fht_log_out[i];
     }

    for(int i = MIN_MED_RANGE; i< MAX_MED_RANGE; i++){
      medium_med_value = medium_med_value + fht_log_out[i];
    }
     medium_bass_value = medium_bass_value/(MAX_BASS_RANGE-MIN_BASS_RANGE);

     medium_med_value = medium_med_value/(MAX_BASS_RANGE-MIN_BASS_RANGE);



     if ((medium_bass_value - (1700/last_volume_bass) )  > last_volume_bass){
      digitalWrite(13, HIGH);
     }

    if ((medium_med_value - (1300/last_volume_med) )  > last_volume_med){
      digitalWrite(12, HIGH);
    }

     if (number_of_cicle == 16){

       if ((medium_bass_value - (1500/last_volume_bass) )  < last_volume_bass){
          digitalWrite(13, LOW);
       }

        if ((medium_bass_value - (1200/last_volume_med) )  < last_volume_med){
          digitalWrite(12, LOW);
       }
      number_of_cicle=0;
     }

     if ((number_of_cicle%8) == 0){
      last_volume_bass = medium_bass_value;
      last_volume_med = medium_med_value;
      
      Serial.write(255); // send a start byte
      Serial.write(fht_log_out, FHT_N/2); // send out the data
     }

     number_of_cicle++;

     }
  }
