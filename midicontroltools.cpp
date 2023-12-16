#include "midicontroltools.h"

IIR::ORDER  filter_order  = IIR::ORDER::OD2; //define filter

/*-------------------*/
/*MCT_Button         */
/*-------------------*/
MCT_Button::MCT_Button(uint8_t mctButtonPin, uint8_t mctButtonType) {
  _mctButtonPin = mctButtonPin;
  _mctButtonType = mctButtonType;
  _mctLastDebounceButtonState = LOW;
  _mctLastDebounceTime = LOW;
  _mctButtonToggleState = LOW;
  //Considering the INPUT_PULLUP situation, button's old state must start reversed (HIGH) 
  _mctOldButtonState = HIGH;
  pinMode(_mctButtonPin, INPUT_PULLUP);
  digitalWrite(_mctButtonPin, HIGH);
}

void MCT_Button::update() {
  _mctButtonState = !_mctButtonDebounceAndRead();

  switch(_mctButtonType) {
    case CLICKBUTTON:
      if (_mctButtonState==1 && _mctOldButtonState==0) {
        if (_buttonClick) _buttonClick();
      }
      break;

    case PUSHPULLBUTTON:
      if (_mctButtonState==1 && _mctOldButtonState==0) {
        //Call the callback function here
        if(_buttonPush) _buttonPush();
      }

      else if (_mctButtonState==0 && _mctOldButtonState==1) {
        //Call the callback function here
        if(_buttonPull) _buttonPull();
      }
      break;

    case TOGGLEBUTTON:
      if (_mctButtonState==1 && _mctOldButtonState==0) {
        _mctButtonToggleState = !_mctButtonToggleState;
        //Call the callback function here
        if (_buttonToggle) _buttonToggle(_mctButtonToggleState);
      }
      break;
    }
    _mctOldButtonState = _mctButtonState;
}

uint8_t MCT_Button::getButtonType() {
  return _mctButtonType;
}

void MCT_Button::attachButtonClick(pushPullCallbackFunc newFunction) {
  _buttonClick = newFunction;  
}

void MCT_Button::attachButtonPush(pushPullCallbackFunc newFunction) {
  _buttonPush = newFunction;
}

void MCT_Button::attachButtonPull(pushPullCallbackFunc newFunction) {
  _buttonPull = newFunction;
}

void MCT_Button::attachButtonToggle(toggleCallbackFunc newFunction) {
  _buttonToggle = newFunction;
}

uint8_t MCT_Button::_mctButtonDebounceAndRead() {

  _mctButtonDebounceReading = digitalRead(_mctButtonPin);

  if (_mctButtonDebounceReading != _mctLastDebounceButtonState) {
    _mctLastDebounceTime = millis();
  }

  if ((millis() - _mctLastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
    _mctDebounceButtonState = _mctButtonDebounceReading;
  }

  _mctLastDebounceButtonState = _mctButtonDebounceReading;
  return _mctDebounceButtonState;
}

/*-------------------*/
/*MCT_Pot            */
/*-------------------*/
MCT_Pot::MCT_Pot(uint8_t mctPotPin) {
  _mctPotPin = mctPotPin;
  pinMode(_mctPotPin, INPUT);
}

void MCT_Pot::update() {
  _mctPotReading = analogRead(_mctPotPin);
  if (abs(_mctPotReading - _mctPotOldReading) > POT_TRESHOLD) {
    _mctPotMapped = _mctPotReading >> 3; //3 bits shift = divide by 8
    //Callback funtion here
    if (_potChange) _potChange(_mctPotMapped);
    _mctPotOldReading = _mctPotReading;
  }
}

void MCT_Pot::attachPotChange(potCallbackFunc newFunction) {
  _potChange = newFunction;
}

/*-------------------*/
/*MCT_Pillar         */
/*-------------------*/
MCT_Pillar::MCT_Pillar(uint8_t mctPillarPin, uint8_t mctPillarLedPin) {
  _f = new Filter(PILLAR_FILTER_CUTOFF_FREQ, PILLAR_FILTER_SAMPLING_TIME, filter_order);
  _mctPillarPin = mctPillarPin;
  _mctPillarLedPin = mctPillarLedPin;
  pinMode(_mctPillarPin, INPUT);
  pinMode(_mctPillarLedPin, OUTPUT);
}

void MCT_Pillar::update() {
  _mctPillarReading = analogRead(_mctPillarPin);

    if (_mctPillarReading > PILLAR_BOTTOM) _mctPillarReading = PILLAR_BOTTOM;
    if (_mctPillarReading < PILLAR_TOP) _mctPillarReading = PILLAR_TOP;
    _mctPillarFiltered = _f->filterIn(_mctPillarReading);
    if (abs(_mctPillarOldReading-_mctPillarFiltered) > PILLAR_TRESHOLD) {
      _mctPillarValue = map(_mctPillarFiltered, PILLAR_TOP, PILLAR_BOTTOM, 0, 127);
      //_mctPillarLedPwmValue = map(_mctPillarFiltered, PILLAR_TOP, PILLAR_BOTTOM, 0, 255);
      _mctPillarLedPwmValue = _mctPillarValue << 2;
      //Relimit filtered pillar value (maybe filter side effects)
      if (_mctPillarValue < 0) _mctPillarValue = 0;
      if (_mctPillarValue > 127) _mctPillarValue = 127;
      analogWrite(_mctPillarLedPin, _mctPillarLedPwmValue);
      //Callback function here:
      if (_pillarChange) _pillarChange(_mctPillarValue);
      _mctPillarOldReading = _mctPillarFiltered;
    }
}

void MCT_Pillar::attachPillarChange(potCallbackFunc newFunction) {
  _pillarChange = newFunction;
}