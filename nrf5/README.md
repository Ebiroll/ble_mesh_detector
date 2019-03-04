
![Mesh network example](img/mesh.svg "Mesh network example")

# Building with the segger embeded studio

0. Clone his directory
git clone https://github.com/Ebiroll/ble_mesh_detector

1. Download embedded studio
https://www.segger.com/downloads/embedded-studio

2. Dowload the nrf SDK for mesh
https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK-for-Mesh/Download#infotabs
nrf5SDKforMeshv310src.zip

Put the sdk in the top dir nordic_semi dir.

3. Download the SDK for nrf devices
https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/Download#infotabs
nRF5SDK_15.2.0_94b126 .zip

You should now have the following directories

    ble_mesh_detector/    
    nRF5_SDK_15.2.0_/  
    sdk/

Copy the detector example from this directory to the 
cp -r ble_mesh_detector/nrf5/ sdk/examples/detector

Open the file nordic_semi\sdk\examples\detector\sensor\sensor_nrf52832_xxAA_s132_6_1_0


# Building with gcc arms toolchain

You need to have the arms tools installed.

    # Ubuntu, Debian, Raspbian, Mint
    sudo apt-get install gdb-arm-none-eabi
    # Fedora
    su -c 'yum install arm-none-eabi-gdb' # https://apps.fedoraproject.org/packages/arm-none-eabi-gdb
    # Arch
    pacman -S arm-none-eabi-gdb # https://www.archlinux.org/packages/community/i686/arm-none-eabi-gdb/

Similar commands for the other tools needed. Read more here.
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm


1. Download the SDK and put it in the sdk subdirectory.
https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK-for-Mesh/Download#infotabs

2. Add this dir to examples
    cd sdk/examples
    ln -s ../../../nrf5/ detector
    Add detector dir to CMakeLists.txt
      add_subdirectory("light_switch")
      add_subdirectory("detector")
3. Build
   cd ../.. , this should put you in the top (ble_mesh_detector/nrf5/sdk)
   mkdir build; cd build; cmake ..; make

# Flashing 

I prefer using blag magic probe. I use this one.
http://github.com/esp32_blackmagic
https://github.com/blacksphere/blackmagic/wiki/Frequently-Asked-Questions

https://devzone.nordicsemi.com/f/nordic-q-a/38385/using-black-magic-probe-with-nrf52832


Connect SWD & SWCL & ground
    cd nrf5/sdk/build/examples/detector/server
    rm-none-eabi-gdb detector_server_nrf52832_xxAA_s132_6.1.0.elf
    # use the 192.168.1.136
    (gdb) target  extended-remote 192.168.1.136:2345
    (gdb) monitor swdp_scan
    (gdb) attach 1
    (gdb) load
    (gdb) b main
    (gdb) c



# Running in qemu

Does not yet work, but I am upating with nrf852_soc.
Get latest qemu or build from source with, 
../qemu/configure --target-list=arm-softmmu

    qemu-system-arm  -d guest_errors,unimp -M microbit -kernel detector_server_nrf52832_xxAA_s132_6.1.0.elf -cpu cortex-m4   -S -s -nographic

    arm-none-eabi-gdb sdk/build/examples/detector/server/detector_server_nrf52832_xxAA_s132_6.1.0.elf -ex ' target remote:1234'
    b 


arm-none-eabi-gdb 


# Other info

https://github.com/NordicPlayground/nRF51-ble-bcast-mesh

https://github.com/ARMmbed/mbed-os/tree/feature-nrf52-sdk15/features/FEATURE_BLE




Instructions for pio. Does not work until updated.
 Download nRFSDK https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/Download#infotabs

    mkdir -p server/lib/mesh
    mkdir -p  server/lib/nrf

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

7. Forget all the above steps and watch a video
https://www.youtube.com/watch?v=b0MxWaAjMco&index=4&list=PLx_tBuQ_KSqGHmzdEL2GWEOeix-S5rgTV