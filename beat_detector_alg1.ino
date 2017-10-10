#include <Adafruit_NeoPixel.h>
#include <CircularBuffer.h>
#include <math.h>

constexpr unsigned window_size = 128;
int8_t       capture[window_size];    // Audio capture buffer
volatile byte samplePos = 0;     // Buffer position counter

constexpr unsigned history_size = 64;



using buf_t = CircularBuffer<uint32_t, history_size> ;
buf_t energy_history;

constexpr auto led_pin = 6;
constexpr auto num_led = 1;

Adafruit_NeoPixel
  strip = Adafruit_NeoPixel(num_led, led_pin, NEO_GRB + NEO_KHZ800);
  
unsigned samples_since_beat;

byte color;

void setup() {
  Serial.begin(230400);
  strip.begin();
  
  setup_adc();
  samples_since_beat = 0;
  color = 0;
}

void setup_adc() {
    // Init ADC free-run mode; f = ( 8MHz/prescaler ) / 13 cycles/conversion 
  ADCSRB = _BV(MUX5);                  // Free run mode,v high MUX bit
  ADMUX  = 0b100 | // Channel 12 sel
          _BV(ADLAR); //left adjust
  ADCSRA = _BV(ADEN)  | // ADC enable
           _BV(ADSC)  | // ADC start
           _BV(ADATE) | // Auto trigger
           _BV(ADIE)  | // Interrupt enable
           _BV(ADPS2) | _BV(ADPS1); //// 64:1 / 13 = 9615 Hz

  DIDR2  = 1 << 4; // Turn off digital input for ADC pin
  TIMSK0 = 0;                // Timer0 off
  sei(); // Enable interrupts

}

uint32_t sample_energy() {
  uint32_t e =0;
  for(auto& s:capture) {
    e+=s*s;
  }
  return e;
}

double buf_average(const buf_t& hist_buf) {
  double sum = 0;
  for (auto i=0; i<hist_buf.size(); i++) {
    sum+=hist_buf[i];
  }
  return sum/hist_buf.size();
}

double buf_variance(const buf_t& hist_buf, const double buf_avg) {
  double accum = 0;
  for (auto i=0; i<hist_buf.size(); i++) {
    accum+=(hist_buf[i]-buf_avg)*(hist_buf[i]-buf_avg);
  }
  return accum/hist_buf.size();
}

constexpr unsigned beat_separation = 10;

constexpr double alpha = -0.0000015;
// -0.0025714;
constexpr double beta = 1.5142857;

void loop() {

  while(ADCSRA & _BV(ADIE)); // Wait for audio sampling to finish

  auto e = sample_energy();

  energy_history.push(e);

  const double hist_avg_e = buf_average(energy_history);
  const double hist_variance = buf_variance(energy_history, hist_avg_e);


  const double hist_std_dev = sqrt(hist_variance);

  if (e > hist_avg_e + hist_std_dev && samples_since_beat >= beat_separation) {
    samples_since_beat = 0;
     Serial.println(1);
     color+=35;
  }
  else {
    samples_since_beat++;
    Serial.println(0);
  }
 
  strip.setPixelColor(0,Wheel(color));
  
   strip.show(); // Update strip
  
  samplePos = 0;                   // Reset sample counter
  ADCSRA |= _BV(ADIE); 

}


constexpr int8_t dc_offset = static_cast<int>(1.25/3.3*255);

ISR(ADC_vect) { // Audio-sampling interrupt
  int8_t              sample         = ADCH; // 0-255
  capture[samplePos] = sample-dc_offset;
  if(++samplePos >= window_size) ADCSRA &= ~_BV(ADIE); // Buffer full, interrupt off
}


// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

