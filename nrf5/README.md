
![Mesh network example](img/mesh.svg "Mesh network example")


1. Download the SDK and put it in the sdk subdirectory.
https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK-for-Mesh/Download#infotabs

2. Download nRFSDK https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/Download#infotabs

3. mkdir server/lib/mesh

3. mkdir server/lib/nrf

4. cp sdk/mesh/core/src/* server/lib/mesh
   cp sdk/mesh/core/api/* server/lib/mesh/
   cp sdk/external/softdevice/s132_5.0.0/s132_nrf52_5.0.0_API/include/ble.h server/lib/mesh/
   cp sdk/mesh/bearer/api/* server/lib/mesh
   cp sdk/mesh/bearer/include/* server/lib/mesh
   cp sdk/mesh/bearer/src/* server/lib/mesh


5. https://github.com/ARMmbed/mbed-os/issues/8913

6. Pray for help