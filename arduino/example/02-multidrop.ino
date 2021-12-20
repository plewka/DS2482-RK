#include "DS2482-RK.h"

//SerialLogHandler logHandler;

DS2482 ds(Wire, 0);

DS2482DeviceListStatic<10> deviceList;
const unsigned long CHECK_PERIOD = 5000;
unsigned long lastCheck = 10000 - CHECK_PERIOD;
int channel = 0;

void setup() {
  Wire.begin(13, 16);
  Serial.begin(115200);
  Serial.println("Hallo");
  ds.setup();

//static DS2482DeviceReset   &run(DS2482 &parent,              std::function<void(DS2482DeviceReset&,   int status)> completion);
//static DS2482ChannelSelect &run(DS2482 &parent, int channel, std::function<void(DS2482ChannelSelect&, int status)> completion);

channel =3;
//  for (channel=0; channel<8; channel++){  
  DS2482DeviceReset::run(ds, [](DS2482DeviceReset&, int status) {
	DS2482ChannelSelect::run(ds, channel, [](DS2482ChannelSelect&, int status) {    
    Serial.println("deviceReset=" + String (status));
    DS2482SearchBusCommand::run(ds, deviceList, [](DS2482SearchBusCommand & obj, int status) {

      if (status != DS2482Command::RESULT_DONE) {
        Serial.println("DS2482SearchBusCommand status=" + String (status));
        return;
      }

      Serial.println("Found " + String(deviceList.getDeviceCount())) + " devices";
    });
  });
  });
//    }

  Serial.println("setup complete");
}


void loop() {

  ds.loop();

  if (millis() - lastCheck >= CHECK_PERIOD) {
    lastCheck = millis();

 // for (channel=0; channel<8; channel++)
 // {  
//	DS2482ChannelSelect::run(ds, 0, [](DS2482ChannelSelect&, int status) {    
    if (deviceList.getDeviceCount() > 0) {

      DS2482GetTemperatureForListCommand::run(ds, deviceList, [](DS2482GetTemperatureForListCommand&, int status, DS2482DeviceList & deviceList) {
        if (status != DS2482Command::RESULT_DONE) {
          Serial.println("DS2482GetTemperatureForListCommand status=" + String (status));
          return;
        }

        Serial.println("got temperatures!");

        for (size_t ii = 0; ii < deviceList.getDeviceCount(); ii++) {
          Serial.println (
                          (deviceList.getAddressByIndex(ii)).toString() +
                          " valid=" + String(deviceList.getDeviceByIndex(ii).getValid()) +
                          " C=" + String(deviceList.getDeviceByIndex(ii).getTemperatureC()) +
                          " F=" + String(deviceList.getDeviceByIndex(ii).getTemperatureF())
                         );
        }

      });

    }
  
    else {
      Serial.println("no devices found");
    }
 //     });      
 // }
  }
}

