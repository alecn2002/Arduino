#include "FastLED.h"

#define DELAY 80
#define LED_STEP 5
#define LED_PIN 6
#define BUTTON_PIN 3
#define NUM_LEDS (2*4*4)

template<typename T, int N>
class RunningLine
{
public:
  RunningLine(const T& init)
  {
    for (size_t i = 0 ; i < N ; ++i)
    {
      line[i] = init;
    }
  }

  const T& push(const T& v)
  {
    for (size_t i = N - 1 ; i > 0 ; --i)
    {
      line[i] = line[i-1];
    }
    return line[0] = v;
  }

  const T& pushBack(const T& v)
  {
    for (size_t i = 0 ; i < N - 1 ; ++i)
    {
      line[i] = line[i+1];
    }
    return line[N-1] = v;
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
    RoundRobin(const T* init_v, const size_t sz) : RunningLine<T, N>(init_v[0])
    {
      for (size_t cnt = 0 ; cnt < N ; ++cnt)
      {
        RunningLine<T, N>::push(init_v[cnt % sz]);
      }
    }

    const T& next(bool dir)
    {
      const T last = RunningLine<T, N>::data()[dir ? N-1 : 0];
      if (dir) {
        return RunningLine<T, N>::push(last);
      } else {
        return RunningLine<T, N>::pushBack(last);
      }
    }
};

template<int BPIN>
class Btn {
public:
  Btn() : was_high(false) {}

  bool check() {
    if (digitalRead(BPIN) == HIGH) {
      was_high = true;
      return false;
    } else {
      bool pressed = was_high;
      was_high = false;
      return pressed;
    }
  }
private:
  bool was_high;
};

const CRGB init_array[] = { CRGB(128, 0, 0), CRGB(128, 64, 0), CRGB(128, 128, 0), 
                CRGB(64, 128, 64), CRGB(0, 128,  128), CRGB(0,  64, 128),
                CRGB(64, 0,  128), CRGB(128, 0, 64), CRGB(128, 0, 0), 
                CRGB(0, 0, 0) };

#define arraySize(a) (sizeof(a) / sizeof(*a))

#define ARRAY_W_SZ(a) a, arraySize(a)

typedef RoundRobin<CRGB, NUM_LEDS> RR_LED;

#define RR_LED_DEF(nm, a) RR_LED nm(ARRAY_W_SZ(a))
#define RR_LED_INI(a) RR_LED(ARRAY_W_SZ(a))

RR_LED_DEF(leds, init_array);

CRGB init_red[] = {CRGB(4, 0, 0),  CRGB(16, 0, 0), CRGB(32, 0, 0),
                           CRGB(64, 0, 0), CRGB(96, 0, 0), CRGB(128, 0, 0),
                           CRGB(96, 0, 0), CRGB(64, 0, 0), CRGB(32, 0, 0),
                           CRGB(16, 0, 0),
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
};

CRGB init_green[] = {CRGB(0, 4, 0),  CRGB(0, 16, 0), CRGB(0, 32, 0),
                           CRGB(0, 64, 0), CRGB(0, 96, 0), CRGB(0, 128, 0),
                           CRGB(0, 96, 0), CRGB(0, 64, 0), CRGB(0, 32, 0),
                           CRGB(0, 16, 0),
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
};

CRGB init_blue[] = {CRGB(0, 0, 4),  CRGB(0, 0, 16), CRGB(0, 0, 32),
                           CRGB(0, 0, 64), CRGB(0, 0, 96), CRGB(0, 0, 128),
                           CRGB(0, 0, 96), CRGB(0, 0, 64), CRGB(0, 0, 32),
                           CRGB(0, 0, 16),
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
};


CRGB init_yellow[] = {CRGB(4, 4, 0),  CRGB(16, 16, 0), CRGB(32, 32, 0),
                           CRGB(64, 64, 0), CRGB(96, 96, 0), CRGB(128, 128, 0),
                           CRGB(96, 96, 0), CRGB(64, 64, 0), CRGB(32, 32, 0),
                           CRGB(16, 16, 0),
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
};

CRGB init_cyan[] = {CRGB(0, 4, 4),  CRGB(0, 16, 16), CRGB(0, 32, 32),
                           CRGB(0, 64, 64), CRGB(0, 96, 96), CRGB(0, 128, 128),
                           CRGB(0, 96, 96), CRGB(0, 64, 64), CRGB(0, 32, 32),
                           CRGB(0, 16, 16),
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
};

CRGB init_magenta[] = {CRGB(4, 0, 4),  CRGB(16, 0, 16), CRGB(32, 0, 32),
                           CRGB(64, 0, 64), CRGB(96, 0, 96), CRGB(128, 0, 128),
                           CRGB(96, 0, 96), CRGB(64, 0, 64), CRGB(32, 0, 32),
                           CRGB(16, 0, 16),
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, 
};


RR_LED leds_rr_init[] = {leds, 
                        RR_LED_INI(init_red),    RR_LED_INI(init_green), RR_LED_INI(init_blue),
                        RR_LED_INI(init_yellow), RR_LED_INI(init_cyan),  RR_LED_INI(init_magenta),
};

//RoundRobin<RR_LED, arraySize(leds_rr_init)> leds_rr(leds_rr_init);

bool fwd = true;
size_t cur_arr = 0;
Btn<BUTTON_PIN> btn;

void setup() { 
    FastLED.addLeds<NEOPIXEL, LED_PIN>(const_cast<struct CRGB*>(leds_rr_init[cur_arr].data()), NUM_LEDS);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if (btn.check())
  {
    if(!fwd) {
      cur_arr = (cur_arr + 1) % arraySize(leds_rr_init);
      FastLED.addLeds<NEOPIXEL, LED_PIN>(const_cast<struct CRGB*>(leds_rr_init[cur_arr].data()), NUM_LEDS);
    }
    fwd = !fwd;
  }
  leds_rr_init[cur_arr].next(fwd); 
  FastLED.show(); 
  delay(DELAY);
}
