
![Mesh network example](img/mesh.svg "Mesh network example")


# Building the examples

clone the ble-mesh branch of the esp-idf

git clone https://github.com/espressif/esp-idf/
cd esp-idf
git checkout -b feature/esp-ble-mesh-v0.5  origin/feature/esp-ble-mesh-v0.5

export ESP


https://github.com/espressif/esp-idf/issues/819

# Flash probems

On my both my devC and the m5Stack , I got some flash problems with this application.
Also note that the example uses lower than normal serial speed. I would guesss this is a workaround.

https://github.com/m5stack/M5Stack/issues/27

I suspect it has something to do with the application code setting gpio internal pullup for the detection of button presses.

The fix is to put the esp32 in boot mode (Boot button) before starting the application code.
For boards without boot button you will have to connect your own. (Connect GPIO0 and GND).
https://github.com/espressif/arduino-esp32/issues/333

https://github.com/espressif/arduino-esp32/issues/1497


https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

# Other info

https://github.com/NordicPlayground/nRF51-ble-bcast-mesh

https://github.com/ARMmbed/mbed-os/tree/feature-nrf52-sdk15/features/FEATURE_BLE

