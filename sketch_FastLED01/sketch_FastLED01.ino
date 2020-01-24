#include "FastLED.h"

#define DELAY 50
#define LED_STEP 5


template<typename T, int N>
class RunningLine
{
public:
  RunningLine(const T init)
  {
    for (size_t i = 0 ; i < N ; ++i)
    {
      line[i] = init;
    }
  }

  void push(const T v)
  {
    for (size_t i = N - 1 ; i > 0 ; --i)
    {
      line[i] = line[i-1];
    }
    line[0] = v;
  }

  const T* data() const
  {
    return line;
  }
protected:
  T line[N];
};

template<typename T, int N>
class RoundRobin : public RunningLine<T, N>
{
  public:
    RoundRobin(const T* init_v) : RunningLine<T, N>(init_v[0])
    {
      for (size_t i = 0 ; i < N ; ++i)
      {
        RunningLine<T, N>::push(init_v[i]);
      }
    }

    T next()
    {
      const T last = RunningLine<T, N>::data()[N-1];
      RunningLine<T, N>::push(last);
      return last;
    }
};

CRGB init_array[] = { CRGB(128, 0, 0), CRGB(128, 64, 0), CRGB(128, 128, 0), 
                CRGB(64, 128, 64), CRGB(0, 128,  128), CRGB(0,  64, 128),
                CRGB(64, 0,  128), CRGB(128, 0, 64), CRGB(128, 0, 0), 
                CRGB(128, 0, 0)};

#define arraySize(a) (sizeof(a) / sizeof(*a))
#define NUM_LEDS arraySize(init_array)

RoundRobin<CRGB, NUM_LEDS> leds(init_array);

void setup() { 
    FastLED.addLeds<NEOPIXEL, 6>(const_cast<struct CRGB*>(leds.data()), NUM_LEDS); 
}

void loop() {
  leds.next(); 
  FastLED.show(); 
  delay(DELAY);
}
