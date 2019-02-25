
![Mesh network example](img/mesh.svg "Mesh network example")

Also look here.
https://github.com/NordicPlayground/nRF51-ble-bcast-mesh

https://github.com/ARMmbed/mbed-os/tree/feature-nrf52-sdk15/features/FEATURE_BLE

1. Download the SDK and put it in the sdk subdirectory.
https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK-for-Mesh/Download#infotabs

2. Download nRFSDK https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/Download#infotabs

3. mkdir -p server/lib/mesh

3. mkdir -p  server/lib/nrf

4. 
    cp sdk/external/softdevice/s132_5.0.0/s132_nrf52_5.0.0_API/include/ble.h server/lib/mesh/
    cp sdk/examples/common/include/* server/lib/mesh 
    cp sdk/mesh/bearer/api/* server/lib/mesh
    cp sdk/mesh/bearer/include/* server/lib/mesh
    cp sdk/mesh/bearer/src/* server/lib/mesh
    cp sdk/examples/common/include/* server/lib/mesh
    cp sdk/mesh/*/api/* server/lib/mesh
    cp sdk/mesh/*/include/* server/lib/mesh
    cp sdk/mesh/*/src/* server/lib/mesh


5. https://github.com/ARMmbed/mbed-os/issues/8913

6. Pray for help

7. Forget all the above steps and watch 
https://www.youtube.com/watch?v=b0MxWaAjMco&index=4&list=PLx_tBuQ_KSqGHmzdEL2GWEOeix-S5rgTV