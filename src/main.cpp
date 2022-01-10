#include <Arduino.h>
#include <CmdMessenger.h>
#include <FSControls.h>

#include <shared_ptr.h>

// Example
#include "Example.h"
#include "ExampleVJoy.h"
#include "ExamplePins.h"

// Full VJoy example
#include "FullVJoy.h"

CmdMessenger comm(Serial);
std::shared_ptr<spaduino::FSControls> fscontrols;
spaduino::FSConfigManager configManager(spaduino::pins::CONFIG_PIN_1, spaduino::pins::CONFIG_PIN_2, spaduino::pins::CONFIG_PIN_3);

void UnhandledMessageReceived()
{
}

void setup() 
{
  Serial.begin(115200);

  comm.attach(UnhandledMessageReceived);

  std::shared_ptr<spaduino::Example> configExample (new spaduino::Example());
  configManager.addConfigImplementation(spaduino::FSConfigManager::FSConfigID::CONFIG_0, configExample);

  std::shared_ptr<spaduino::ExampleVJoy> configExampleVJoy (new spaduino::ExampleVJoy());
  configManager.addConfigImplementation(spaduino::FSConfigManager::FSConfigID::CONFIG_1, configExampleVJoy);

  std::shared_ptr<spaduino::FullVJoy> configFullVJoy (new spaduino::FullVJoy());
  configManager.addConfigImplementation(spaduino::FSConfigManager::FSConfigID::CONFIG_2, configFullVJoy);

  fscontrols = std::shared_ptr<spaduino::FSControls>(new spaduino::FSControls(F("{e3013572-3edb-4392-9b7a-1dfb4a44a737}"), comm));
  configManager.setup(fscontrols, comm);
}

void loop() 
{
  comm.feedinSerialData();
  fscontrols->loopOnce();
}