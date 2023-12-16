#include "midicontroltools.h"

MCT_Button btn1(12, TOGGLEBUTTON); //Pin Number, Button Type
MCT_Pot pot1(A0);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Attach user defined callback functions here
  btn1.attachButtonPush(onBtn1Push);
  btn1.attachButtonPull(onBtn1Pull);
  btn1.attachButtonToggle(onBtn1Toggle);
  btn1.attachButtonClick(onBtn1Click);
  pot1.attachPotChange(onPot1Change);

}

void loop() {
  // put your main code here, to run repeatedly:
  btn1.update();
  pot1.update();
  delay(5);
}

void onBtn1Click() {
  Serial.println("Button clicked...");
}

void onBtn1Push() {
  Serial.println("Button PUSHED...");
}

void onBtn1Pull() {
  Serial.println("Button PULLED...");
}

void onBtn1Toggle(bool toggleState) {
  Serial.print("Button toggle state: ");
  Serial.println(toggleState);
}

void onPot1Change(uint8_t potValue) {
  Serial.print("Pot value: ");
  Serial.println(potValue);
}