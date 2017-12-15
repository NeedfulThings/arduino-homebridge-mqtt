#include "Switch.h"

Switch::Switch(int outputPin) {
  pinMode(outputPin, OUTPUT);
  this->outputPin = outputPin;
  accessory.name = String(ESP.getChipId());
  service.name = "Switch";
  service.type = "Switch";
  on.name = "On";
}

void Switch::begin(IPAddress mqttServer) {
  homebridgeMqtt.onSetValueFromHomebridge([this](Accessory accessory, Service service, Characteristic characteristic) -> void {
    if (this->accessory.name != accessory.name || this->service.name != service.name || on.name != characteristic.name) {
      return;
    }
    if (characteristic.value) {
      Serial.println("ON");
      digitalWrite(outputPin, HIGH);
    } else {
      Serial.println("OFF");
      digitalWrite(outputPin, LOW);
    }
    on.value = characteristic.value;
  });
  homebridgeMqtt.connect(mqttServer);
  homebridgeMqtt.addAccessory(accessory, service);
  homebridgeMqtt.addService(accessory, service);
  homebridgeMqtt.getAccessory(accessory);
}

int Switch::getOnValue() {
  return on.value;
}