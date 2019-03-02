
// https://electronut.in/nrf52-baremetal/

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0179b/ar01s02s07.html

// http://www.bravegnu.org/gnu-eprog/hello-arm.html

// https://gcc.gnu.org/onlinedocs/gcc-6.1.0/gcc/ARM-Function-Attributes.html

// To create mbr.bin
//  arm-none-eabi-gcc -c mbr.c 
// arm-none-eabi-ld -T mbr.ld -o mbr.elf mbr.o
// arm-none-eabi-objcopy -O binary mbr.elf mbr.bin 

#if 0

00000000: 0050 0020 
 1        4b00 0008 
 2       3d00 0008 
 3       3d00 0008  
00000010: 3d00 0008 
 5        3d00 0008 
 6       0x3d000008 
 7       3d00 0008  
00000020: 3d00 0008 
 9        3d00 0008 
 10        3d00 0008 
 11        5900 0008 
00000030: 3d00 0008 
 13        3d00 0008 
14		  3d00 0008 

15		 80b500af  
00000040: c046bd46 
          80bc01bc 
		  004780b5 
		  00afc046  .F.F.....G.....F
00000050: bd46 80bc 01bc 0047 80b5 00af c046 bd46  .F.....G.....F.F
00000060: 80bc 01bc 0047 80b5 84b0 00af 7860 7b68  .....G......x`{h
00000070: 1833 1b68 023b 1b78 fb60 c046 c046 bd46  .3.h.;.x.`.F.F.F
00000080: 04b0 80bc 01bc 0047                      .......G
#endif
// For reference
//  arm-none-eabi-ld -Ttext=0x0 -o mbr.elf mbr.o
// arm-none-eabi-nm mbr.o
// arm-none-eabi-objdump -d mbr.o

// SVCALL(SD_SOFTDEVICE_ENABLE, uint32_t, sd_softdevice_enable(nrf_clock_lf_cfg_t const * p_clock_lf_cfg, nrf_fault_handler_t fault_handler))

#pragma GCC target ("thumb")

#define NRF_SUCCESS   0

#define STACK_TOP (void *)0x20005000

#define SVC_00 0
#define SVC_01 1

#if 0
EXTI0_IRQHandler PROC
ldr r0,[r11,#0x14+PORTD] ; R0 < @GPIOD_ODR
eor r0,r0, #BluLed ; R0 = Toggle Blue Led
str r0,[r11,#0x14+PORTD] ; R0 > @GPIOD_ODR
ldr r0, =0xE000E280 ;=NVIC_ICPR0 ; clear pending interrupt from EXTI0_IRQn channel
ldr r1, =0x40
str r1, [r0]
BX LR
ENDP
#endif

void dummy_fn(void) {

};

//__attribute__ ((interrupt ("IRQ")))  
void SVCHandler(void);


void Reset_Handler(void) {
    
}

__attribute__((section(".isr_vector")))
void (*vectors[])(void) = {
        STACK_TOP,
        Reset_Handler,  // reset will be replaced i qemu
        dummy_fn,
        dummy_fn,
        dummy_fn,
        dummy_fn,
        dummy_fn,
        dummy_fn,
        dummy_fn,
        dummy_fn,
        dummy_fn,
        SVCHandler,
        dummy_fn,
        dummy_fn,
        dummy_fn,
};

 void SVCHandler_simple(void)
 {    
	 asm("push	{r0, lr}");
	 //asm("LDR r0,MSP");
	 asm("mov r0,#0x0");
	 asm("pop	{r0}");
 }

 //__attribute__((interrupt ("IRQ")))  
 void SVCHandler(void) 
{   
//   asm("IMPORT SVCHandler_main");
//    asm("TST lr, #4");    
//    asm("MRSEQ r0, MSP");    
//    MRSNE r0, PSP    
//    B SVCHandler_main
 }
 
 void SVCHandler_main(unsigned int * svc_args)
 {    
     unsigned int svc_number;    
 /*    * Stack contains:    * r0, r1, r2, r3, r12, r14, the return address and xPSR    
       * First argument (r0) is svc_args[0]    */    
    svc_number = ((char *)svc_args[6])[-2];    
    switch(svc_number)    
    {        
        case SVC_00:            /* Handle SVC 00 */            
          break;        
        case SVC_01:            /* Handle SVC 01 */            
           break;        
        default:             /* Unknown SVC */            
          break;    
    }
}

/*
#define SVC_00 0x00#define SVC_01 0x01
void __svc(SVC_00) svc_zero(const char *string);
void __svc(SVC_01) 
svc_one(const char *string);
int call_system_func(void)
{    svc_zero("String to pass to SVC handler zero");    
     svc_one("String to pass to a different OS function");}
*/
#if 0
static void jump_to_app() {
    // Adjust the current ISR vector address.
    *(uint32_t*)0x20000000 = SD_CODE_BASE

    // Jump to the ISR vector of the SoftDevice.
    uint32_t *sd_isr = (uint32_t*) SD_CODE_BASE;
    uint32_t new_sp = sd_isr[0]; // load end of stack (_estack)
    uint32_t new_pc = sd_isr[1]; // load Reset_Handler
    __asm__ __volatile__(
            "mov sp, %[new_sp]\n" // set stack pointer to initial stack pointer
            "mov pc, %[new_pc]\n" // jump to SoftDevice Reset_Vector
            :
            : [new_sp]"r" (new_sp),
              [new_pc]"r" (new_pc));
}


	/* Configure initial stack pointer using linker-generated symbols. */
	.pvStack = (void*) (&__stack_top),
	.pfnReset_Handler = (void*) Reset_Handler,
	.pfnNMI_Handler = (void*) NMI_Handler,
	.pfnHardFault_Handler = (void*) HardFault_Handler,
	.pfnMemManage_Handler = (void*) MemManage_Handler,
	.pfnBusFault_Handler = (void*) BusFault_Handler,
	.pfnUsageFault_Handler = (void*) UsageFault_Handler,
	.pfnReserved1_Handler = (void*) (0UL), // Reserved
	.pfnReserved2_Handler = (void*) (0UL), // Reserved
	.pfnReserved3_Handler = (void*) (0UL), // Reserved
	.pfnReserved4_Handler = (void*) (0UL), // Reserved
	.pfnSVC_Handler = (void*) SVC_Handler,
	.pfnDebugMon_Handler = (void*) DebugMon_Handler,
	.pfnReserved5_Handler = (void*) (0UL), // Reserved
	.pfnPendSV_Handler = (void*) PendSV_Handler,
	.pfnSysTick_Handler = (void*) SysTick_Handler,

	/* External Interrupts */
	.pfnPowerClk_Handler = (void*) PowerClk_Handler, // 0 - Power/Clock Controller
	.pfnRadio_Handler = (void*) Radio_Handler, // 1 - Radio Controller
	.pfnUART0_Handler = (void*) UART0_Handler, // 2 - UART0
	.pfnSPI0_TWI0_Handler = (void*) SPI0_TWI0_Handler, // 3 - SPI0/TWI0 Controller
	.pfnSPI1_TWI1_Handler = (void*) SPI1_TWI1_Handler, // 4 - SPI1/TWI1 Controller
	.pfnNFC_Handler = (void*) NFC_Handler, // 5 - Near-Field Communication Controller
	.pfnGPIOTE_Handler = (void*) GPIOTE_Handler, // 6 - General Purpose Input Output Task and Events
	.pfnSAADC_Handler = (void*) SAADC_Handler, // 7 - Successive Approximation Analog-to-Digital Converter
	.pfnTimer0_Handler = (void*) Timer0_Handler, // 8 - Timer 0
	.pfnTimer1_Handler = (void*) Timer1_Handler, // 9 - Timer 1
	.pfnTimer2_Handler = (void*) Timer2_Handler, // 10 - Timer 2
	.pfnRTC0_Handler = (void*) RTC0_Handler, // 11 - Real Time Clock 0
	.pfnTemp_Handler = (void*) Temp_Handler, // 12 - Temperature Sensor
	.pfnRNG_Handler = (void*) RNG_Handler, // 13 - Random Number Generator
	.pfnECB_Handler = (void*) ECB_Handler, // 14 - AES Electronic Code Book
	.pfnCCM_AAR_Handler = (void*) CCM_AAR_Handler, // 15 - AES CCM
	.pfnWDT_Handler = (void*) WDT_Handler, // 16 - Watchdog Timer
	.pfnRTC1_Handler = (void*) RTC1_Handler, // 17 - Real Time Clock 1
	.pfnQDEC_Handler = (void*) QDEC_Handler, // 18 - Quadrature Decoder
	.pfnCOMP_LPCOMP_Handler = (void*) COMP_LPCOMP_Handler, // 19 - Comparator/Low Power Comparator
	.pfnSWI0_EGU0_Handler = (void*) SWI0_EGU0_Handler, // 20 - SWI0/EGU0
	.pfnSWI1_EGU1_Handler = (void*) SWI1_EGU1_Handler, // 21 - SWI1/EGU1
	.pfnSWI2_EGU2_Handler = (void*) SWI2_EGU2_Handler, // 22 - SWI2/EGU2
	.pfnSWI3_EGU3_Handler = (void*) SWI3_EGU3_Handler, // 23 - SWI3/EGU3
	.pfnSWI4_EGU4_Handler = (void*) SWI4_EGU4_Handler, // 24 - SWI4/EGU4
	.pfnSWI5_EGU5_Handler = (void*) SWI5_EGU5_Handler, // 25 - SWI5/EGU5
	.pfnTimer3_Handler = (void*) Timer3_Handler, // 26 - Timer 3
	.pfnTimer4_Handler = (void*) Timer4_Handler, // 27 - Timer 4
	.pfnPWM0_Handler = (void*) PWM0_Handler, // 28 - Pulse-Width Modulation Controller 0
	.pfnPDM_Handler = (void*) PDM_Handler, // 29 - Pulse-Density Modulation Controller
	.pfnReserved6_Handler = (void*) (0UL), // 30 - Reserved
	.pfnReserved7_Handler = (void*) (0UL), // 31 - Reserved
	.pfnMWU_Handler = (void*) MWU_Handler, // 32 - Memory Watch Unit
	.pfnPWM1_Handler = (void*) PWM1_Handler, // 33 - Pulse-Width Modulation Controller 1
	.pfnPWM2_Handler = (void*) PWM2_Handler, // 34 - Pulse-Width Modulation Controller 2
	.pfnSPI2_Handler = (void*) SPI2_Handler, // 35 - Serial Peripheral Interface 2
	.pfnRTC2_Handler = (void*) RTC2_Handler, // 36 - Real Time Clock 2
	.pfnI2S_Handler = (void*) I2S_Handler, // 37 - Inter-IC Sound Controller
	.pfnFPU_Handler = (void*) FPU_Handler // 38 - Floating-Point Unit
#endif