# Qemu information

This directory contains code for patching qemu to be able to load and run some of the mesh code in order to 
understand the startup and initialization code.

http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk52.v0.9.0%2Fbledfu_memory.html


# Based on the nRF51 

Thanks to this project.
https://douzepouze.github.io/gsoc18-qemu/


# Results

## Sensor

~/nrf_qemu/arm-softmmu/qemu-system-arm -d unimp,guest_errors -M nrf52_dk -kernel detector_sensor_nrf52832_xxAA_s132_6.1.0.elf  -s -S

arm-none-eabi-gdb  detector_sensor_nrf52832_xxAA_s132_6.1.0.elf -ex ' target remote:1234'

(gdb) b app_timer_init()

(gdb) b ble_stack_init()

See how this is handled.
Status = SEGGER_RTT_WriteNoLock(BufferIndex, pBuffer, NumBytes); 

```
#0  SEGGER_RTT_Write (BufferIndex=BufferIndex@entry=0, pBuffer=pBuffer@entry=0x2000fc14, NumBytes=31)
    at /home/olof/ble/ble_mesh_detector/nrf5/sdk/external/rtt/src/SEGGER_RTT.c:678
#1  0x00040140 in SEGGER_RTT_vprintf (BufferIndex=BufferIndex@entry=0, sFormat=0x420a1 "", sFormat@entry=0x0, pParamList=0x2000fc84,
    pParamList@entry=0x2000fc6c) at /home/olof/ble/ble_mesh_detector/nrf5/sdk/external/rtt/src/SEGGER_RTT_printf.c:456
#2  0x00040158 in SEGGER_RTT_printf (BufferIndex=BufferIndex@entry=0, sFormat=0x4208c "<t: %10u>, %s, %4d, ")
    at /home/olof/ble/ble_mesh_detector/nrf5/sdk/external/rtt/src/SEGGER_RTT_printf.c:498
#3  0x0002defa in log_callback_rtt (dbg_level=<optimized out>, p_filename=<optimized out>, line=<optimized out>, timestamp=<optimized out>,
    format=0x419e4 "----- BLE Mesh Light Switch Client Demo -----\n", arguments=...) at /home/olof/ble/ble_mesh_detector/nrf5/sdk/mesh/core/src/log.c:63
#4  0x0002df3c in log_vprintf (dbg_level=dbg_level@entry=4, p_filename=<optimized out>, line=<optimized out>, timestamp=<optimized out>,
```


## Server

Emulation was not perfect and to get to the interesting parts I patched system_nrf52,c
nRF5_SDK_15.2.0_9412b96/modules/nrfx/mdk/system_nrf52.c

    void SystemInit(void)
    {
       return;
    ...

/home/olof/nrf_qemu/arm-softmmu/qemu-system-arm -d unimp,guest_errors -M nrf52_dk -kernel detector_server_nrf52832_xxAA_s132_6.1.0.elf  -s -S

arm-none-eabi-gdb sdk/build/examples/detector/server/detector_server_nrf52832_xxAA_s132_6.1.0.elf -ex ' target remote:1234'


Starts here.

nRF5_SDK_15.2.0_9412b96/modules/nrfx/mdk/gcc_startup_nrf52.S
I this fails...
app_timer_create()
NRF52_soc.io: unimplemented device read (size 4, offset 0x11504)
NRF52_soc.io: unimplemented device read (size 4, offset 0x11504)
rf5/sdk/examples/common/src/simple_hal.c:175


## dfu
 /home/olof/nrf_qemu/arm-softmmu/qemu-system-arm -d unimp,guest_errors -M nrf52_dk -kernel dfu_nrf52832_xxAA_s132_6.1.0.elf -s -S

 arm-none-eabi-gdb dfu/dfu_nrf52832_xxAA_s132_6.1.0.elf -ex ' target remote:1234'


Stops here
    void ble_stack_init(void)                                                                                                           │
   │115     {                                                                                                                                   │
B+>│116         uint32_t err_code = nrf_sdh_enable_request();   
SysTick: Bad write offset 0x28
SysTick: Bad write offset 0x2c
NVIC: Bad write offset 0x0
NVIC: Bad write offset 0x4
NVIC: Bad write offset 0x8
NVIC: Bad write offset 0xc
SysTick: Bad write offset 0xc

app_util_critical_region_enter

 SVCALL(SD_SOFTDEVICE_ENABLE, uint32_t, sd_softdevice_enable(nrf_clock_lf_cfg_t const * p_clock_lf_cfg, nrf_fault_handler_t fault_han│

  0x270cc <sd_softdevice_enable>           svc    16                       
  0x270ce <sd_softdevice_enable+2>        bx     lr                       
  0x270d0 <nrf_sdh_enable_request>        push   {r4, r5, r6, lr}                     
  0x270d2 <nrf_sdh_enable_request+2>      ldr    r5, [pc, #240]  ; (0x271c4 <nrf_sdh_enable_request+244>)                

   /* Take the next available page address */                                                                           │
   │214         bank_addr  = (uint32_t) (rom_end & FLASH_PAGE_MASK) + FLASH_PAGE_SIZE;                                               │
   │215         __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "rom_base   %X\n", rom_base);                                                     │
   │216         __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "rom_end    %X\n", rom_end);                                                      │
   │217         __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "rom_length %X\n", rom_length);                                                   │
   │218         __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "bank_addr   %X\n", bank_addr);   

   (gdb) p/x rom_base 
   $6 = 0x26241
   (gdb) p/x rom_end
   $7 = 0x49a30
  (gdb) p/x rom_length 
   $8 = 0x237ef
  (gdb) p/x bank_addr
  $9 = 0x4a000




# Hello nRF52 

http://www.bravegnu.org/gnu-eprog/hello-arm.html

I am trying to get things to run

It seems like we start here,
/home/olof/ble/ble_mesh_detector/nrf5/nRF5_SDK_15.2.0_9412b96/modules/nrfx/mdk/gcc_startup_nrf52.S

0x00000000 in ?? ()
(gdb) monitor info mtree
address-space: memory
  0000000000000000-ffffffffffffffff (prio -1, i/o): system
    0000000040000000-000000005fffffff (prio -1000, i/o): NRF52_soc.io
    0000000040008000-0000000040008fff (prio 0, i/o): nrf51_soc.timer
    0000000040009000-0000000040009fff (prio 0, i/o): nrf51_soc.timer
    000000004000a000-000000004000afff (prio 0, i/o): nrf51_soc.timer
    00000000f0000000-00000000ffffffff (prio -1000, i/o): NRF52_soc.private

address-space: I/O
  0000000000000000-000000000000ffff (prio 0, i/o): io

address-space: cpu-memory-0
  0000000000000000-ffffffffffffffff (prio 0, i/o): armv7m-container
    0000000000000000-ffffffffffffffff (prio -1, i/o): nrf52-container
      0000000000000000-00000000001fffff (prio 0, romd): nrf51_soc.flash
      0000000000000000-ffffffffffffffff (prio -1, i/o): system
        0000000040000000-000000005fffffff (prio -1000, i/o): NRF52_soc.io
        0000000040008000-0000000040008fff (prio 0, i/o): nrf51_soc.timer
        0000000040009000-0000000040009fff (prio 0, i/o): nrf51_soc.timer
        000000004000a000-000000004000afff (prio 0, i/o): nrf51_soc.timer
        00000000f0000000-00000000ffffffff (prio -1000, i/o): NRF52_soc.private
      0000000010000000-00000000100000ff (prio 0, i/o): nrf51_soc.ficr
      0000000010001000-00000000100010ff (prio 0, i/o): nrf51_soc.uicr
      0000000020000000-000000002000ffff (prio 0, ram): nrf52.sram
      0000000040000000-0000000040000fff (prio -1, i/o): NRF52_soc.clock
      0000000040002000-0000000040002fff (prio 0, i/o): nrf51_soc.uart
      0000000040003000-0000000040003fff (prio -1, i/o): microbit.twi
      000000004000d000-000000004000dfff (prio 0, i/o): nrf51_soc.rng
      000000004001e000-000000004001efff (prio 0, i/o): nrf51_soc.nvmc
      0000000050000000-0000000050000fff (prio 0, i/o): nrf51_soc.gpio
    00000000e000e000-00000000e000efff (prio 0, i/o): nvic
      00000000e000e000-00000000e000efff (prio 0, i/o): nvic_sysregs
      00000000e000e010-00000000e000e0ef (prio 1, i/o): nvic_systick

(gdb) p/x 273529
$1 = 0x42c79



