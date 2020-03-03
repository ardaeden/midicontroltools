#ifndef _midiControlToolsH
#define _midiControlToolsH

#include <Arduino.h>

#define BUTTON_DEBOUNCE_DELAY 20
#define POT_TRESHOLD 5
#define PILLAR_TRESHOLD 5

typedef void (*pushPullCallbackFunc)(void);
typedef void (*toggleCallbackFunc)(bool toggleState);
typedef void (*potCallbackFunc)(uint8_t potValue);

enum mct_button_type {  CLICKBUTTON,
                        PUSHPULLBUTTON,
                        TOGGLEBUTTON };

class MCT_Button {
  public:
    MCT_Button(uint8_t mctButtonPin, uint8_t mctButtonType);
    uint8_t getButtonType();
    void checkButton();
    void attachButtonClick(pushPullCallbackFunc newFunction);
    void attachButtonPush(pushPullCallbackFunc newFunction);
    void attachButtonPull(pushPullCallbackFunc newFunction);
    void attachButtonToggle(toggleCallbackFunc newFunction);

  private:
    uint8_t _mctButtonType;
    uint8_t _mctButtonPin;
    uint8_t _mctButtonState;
    uint8_t _mctOldButtonState;
    uint8_t _mctButtonDebounceReading;
    uint8_t _mctDebounceButtonState;
    uint8_t _mctLastDebounceButtonState;
    uint8_t _mctButtonToggleState;
    unsigned long _mctLastDebounceTime;
    pushPullCallbackFunc _buttonClick = NULL;
    pushPullCallbackFunc _buttonPush = NULL;
    pushPullCallbackFunc _buttonPull = NULL;
    toggleCallbackFunc _buttonToggle = NULL;
    uint8_t _mctButtonDebounceAndRead();
};


class MCT_Pot {
  public:
    MCT_Pot(uint8_t mctPotPin);
    void checkPot();
    void attachPotChange(potCallbackFunc newFunction);
  private:
    uint8_t _mctPotPin;
    int _mctPotReading;
    int _mctPotOldReading = 0;
    uint8_t _mctPotMapped;
    potCallbackFunc _potChange = NULL;
};


class MCT_Pillar {
  public:
    MCT_Pillar(uint8_t mctPillarPin, uint8_t mctPillarLedPin);
    void checkPillar();
  private:
  uint8_t _mctPillarPin;
  uint8_t _mctPillarLedPin;
  int _mctPillarReading;
  int _mctPillarOldReading;
};
#endif
