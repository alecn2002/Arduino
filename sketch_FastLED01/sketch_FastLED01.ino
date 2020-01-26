#include "FastLED.h"

#define DELAY 200
#define LED_STEP 5
#define LED_PIN 6
#define BUTTON_PIN 3

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

  void pushBack(const T v)
  {
    for (size_t i = 0 ; i < N - 1 ; ++i)
    {
      line[i] = line[i+1];
    }
    line[N-1] = v;
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

    T next(bool dir)
    {
      const T last = RunningLine<T, N>::data()[dir ? N-1 : 0];
      if (dir) {
        RunningLine<T, N>::push(last);
      } else {
        RunningLine<T, N>::pushBack(last);
      }
      return last;
    }
};

CRGB init_array[] = { CRGB(128, 0, 0), CRGB(128, 64, 0), CRGB(128, 128, 0), 
                CRGB(64, 128, 64), CRGB(0, 128,  128), CRGB(0,  64, 128),
                CRGB(64, 0,  128), CRGB(128, 0, 64), CRGB(128, 0, 0), 
                CRGB(128, 0, 0) };

#define arraySize(a) (sizeof(a) / sizeof(*a))
#define NUM_LEDS arraySize(init_array)

RoundRobin<CRGB, NUM_LEDS> leds(init_array);

bool fwd = true;

void setup() { 
    FastLED.addLeds<NEOPIXEL, LED_PIN>(const_cast<struct CRGB*>(leds.data()), NUM_LEDS);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    fwd = !fwd;
  }
  leds.next(fwd); 
  FastLED.show(); 
  delay(DELAY);
}
