/**
 * @file stm32f103c8_startup.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */



/*------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------- INCLUDES ---------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

/* STDLib. */
#include <stddef.h> /* offsetof() */
#include <stdint.h>
#include <string.h> /* memset */

/* ECU. */
#include <ecu/asserter.h>



/*------------------------------------------------------------------------------------------------------*/
/*------------------------------ GLOBAL VARIABLES EXPORTED FROM LINKER SCRIPT --------------------------*/
/*------------------------------------------------------------------------------------------------------*/

/**
 * @brief This is the ending address of RAM. I.e. RAM starting address + RAM total length.
 * This is because stack grows from top-down on ARM Cortex M. This value is assigned to
 * the Stack Pointer when Cortex M3 first boots up, so it is used to initialize the
 * Stack on bootup.
 */
extern uint32_t stack_start_;


/**
 * @brief Starting address of FLASH .data section we load from.
 */
extern uint32_t data_start_flash_;


/**
 * @brief Starting address of RAM .data section we copy into.
 */
extern uint32_t data_start_ram_;


/**
 * @brief Ending address of RAM .data section we copy into.
 */
extern uint32_t data_end_ram_;


/**
 * @brief Starting address of .bss section in RAM.
 */
extern uint32_t bss_start_;


/**
 * @brief Ending address of .bss section in RAM.
 */
extern uint32_t bss_end_;



/*------------------------------------------------------------------------------------------------------*/
/*---------------------------------------- STATIC FUNCTION DECLARATIONS --------------------------------*/
/*------------------------- THESE ARE SYSTEM FAULTS THAT SHOULD BE DEFINED IN THIS FILE ----------------*/
/*------------------------------------------------------------------------------------------------------*/

/**
 * @brief Fires if a spurrious ISR was triggered, either due to corruption
 * or an interrupt the user did not register firing. In either case this
 * is a fatal error and this function should treat it as such.
 */
static void unregistered_isr_handler(void);


/**
 * @brief Catch all for system faults.
 */
static void hard_fault_isr_handler(void);


/**
 * @brief Valid address accessed but it was to protected memory (either
 * governed by MPU or system).
 */
static void memmanage_isr_handler(void);


/**
 * @brief Invalid address access attempted.
 */
static void bus_fault_isr_handler(void);


/**
 * @brief Undefined instruction attempted.
 */
static void usage_fault_isr_handler(void);



/*------------------------------------------------------------------------------------------------------*/
/*---------------------------------------- PUBLIC FUNCTION DECLARATIONS --------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

/**
 * @brief Responsible for all required initializations before main(). Must also
 * branch to main(). When Cortex M3 machines first boot up this function will
 * be called. Declared as global function in order for linker script to see
 * the symbol.
 */
extern void reset_isr_handler(void);


/**
 * @brief STDLib initialization function that must be called prior to using
 * STDLib.
 */
extern void __libc_init_array(void);


/**
 * @brief Application's main.
 */
extern int main(void);



/*------------------------------------------------------------------------------------------------------*/
/*---------------------------------------- PUBLIC FUNCTION DECLARATIONS --------------------------------*/
/*-------------------------------- DEFINITIONS CAN BE OVERRIDDEN BY APPLICATION ------------------------*/
/*------------------------------------------------------------------------------------------------------*/

extern void nmi_isr_handler(void)                           __attribute__((weak, alias ("unregistered_isr_handler")));
extern void svcall_isr_handler(void)                        __attribute__((weak, alias ("unregistered_isr_handler")));
extern void debug_monitor_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void pendsv_isr_handler(void)                        __attribute__((weak, alias ("unregistered_isr_handler")));
extern void systick_isr_handler(void)                       __attribute__((weak, alias ("unregistered_isr_handler")));
extern void watchdog_isr_handler(void)                      __attribute__((weak, alias ("unregistered_isr_handler")));
extern void pvd_isr_handler(void)                           __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tamper_isr_handler(void)                        __attribute__((weak, alias ("unregistered_isr_handler")));
extern void rtc_isr_handler(void)                           __attribute__((weak, alias ("unregistered_isr_handler")));
extern void flash_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void rcc_isr_handler(void)                           __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti0_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti1_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti2_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti3_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti4_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel1_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel2_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel3_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel4_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel5_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel6_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma1_channel7_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void adc1_adc2_isr_handler(void)                     __attribute__((weak, alias ("unregistered_isr_handler")));
extern void usb_hp_or_can_tx_isr_handler(void)              __attribute__((weak, alias ("unregistered_isr_handler")));
extern void usb_lq_or_can_rx0_isr_handler(void)             __attribute__((weak, alias ("unregistered_isr_handler")));
extern void can_rx1_isr_handler(void)                       __attribute__((weak, alias ("unregistered_isr_handler")));
extern void can_sce_isr_handler(void)                       __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti_9_5_isr_handler(void)                      __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim1_break_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim1_update_isr_handler(void)                   __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim1_trigger_communication_isr_handler(void)    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim1_capture_compare_isr_handler(void)          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim2_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim3_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim4_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void i2c1_event_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void i2c1_error_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void i2c2_event_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void i2c2_error_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void spi1_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void spi2_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void uart1_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void uart2_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void uart3_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void exti15_10_isr_handler(void)                     __attribute__((weak, alias ("unregistered_isr_handler")));
extern void rtc_alarm_isr_handler(void)                     __attribute__((weak, alias ("unregistered_isr_handler")));
extern void usb_wakeup_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim8_break_isr_handler(void)                    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim8_update_isr_handler(void)                   __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim8_trigger_communication_isr_handler(void)    __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim8_capture_compare_isr_handler(void)          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void adc3_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void fsmc_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void sdio_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim5_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void spi3_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void uart4_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void uart5_isr_handler(void)                         __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim6_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void tim7_isr_handler(void)                          __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma2_channel1_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma2_channel2_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma2_channel3_isr_handler(void)                 __attribute__((weak, alias ("unregistered_isr_handler")));
extern void dma2_channel4_5_isr_handler(void)               __attribute__((weak, alias ("unregistered_isr_handler")));



/*------------------------------------------------------------------------------------------------------*/
/*------------------------------------------- FILE SCOPE VARIABLES -------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

/**
 * @brief Vector Table for STM32F103C8 microcontroller. This is placed in
 * input section .isr_vector so it can be loaded into address 0x08000000
 * in FLASH. This table holds addresses to ISR handlers that must be in
 * this order. When an exception occurs the NVIC goes into this table and
 * passes the address of the corresponding ISR handler directly to the CPU. 
 * Note that these functions don't have to have the interrupt attribute 
 * since NVIC automatically saves and restores context onto the stack on 
 * exception entry.
 */
static const uint32_t vector_table[] __attribute__((section(".isr_vector"))) =
{
    /*--------------------------------------------------------------*/
    /*------------------- FOR ALL CORTEX M3 CORES ------------------*/
    /*--------------------------------------------------------------*/

    (uint32_t)&stack_start_,                                /* 0x00. */
    (uint32_t)&reset_isr_handler,                           /* 0x04. */
    (uint32_t)&nmi_isr_handler,                             /* 0x08. */
    (uint32_t)&hard_fault_isr_handler,                      /* 0x0C. */
    (uint32_t)&memmanage_isr_handler,                       /* 0x10. */
    (uint32_t)&bus_fault_isr_handler,                       /* 0x14. */
    (uint32_t)&usage_fault_isr_handler,                     /* 0x18. */
    (uint32_t)&unregistered_isr_handler,                    /* Pad. 0x1C. */
    (uint32_t)&unregistered_isr_handler,                    /* Pad. 0x20. */
    (uint32_t)&unregistered_isr_handler,                    /* Pad. 0x24. */
    (uint32_t)&unregistered_isr_handler,                    /* Pad. 0x28. */
    (uint32_t)&svcall_isr_handler,                          /* 0x2C. */
    (uint32_t)&debug_monitor_isr_handler,                   /* 0x30. */
    (uint32_t)&unregistered_isr_handler,                    /* Pad. 0x34. */
    (uint32_t)&pendsv_isr_handler,                          /* 0x38. */
    (uint32_t)&systick_isr_handler,                         /* 0x3C. */


    /*--------------------------------------------------------------*/
    /*------------------- SPECIFIC TO STM32F103C8 ------------------*/
    /*--------------------------------------------------------------*/

    (uint32_t)&watchdog_isr_handler,                        /* 0x40. */
    (uint32_t)&pvd_isr_handler,                             /* 0x44. */
    (uint32_t)&tamper_isr_handler,                          /* 0x48. */
    (uint32_t)&rtc_isr_handler,                             /* 0x4C. */
    (uint32_t)&flash_isr_handler,                           /* 0x50. */
    (uint32_t)&rcc_isr_handler,                             /* 0x54. */
    (uint32_t)&exti0_isr_handler,                           /* 0x58. */
    (uint32_t)&exti1_isr_handler,                           /* 0x5C. */
    (uint32_t)&exti2_isr_handler,                           /* 0x60. */
    (uint32_t)&exti3_isr_handler,                           /* 0x64. */
    (uint32_t)&exti4_isr_handler,                           /* 0x68. */
    (uint32_t)&dma1_channel1_isr_handler,                   /* 0x6C. */
    (uint32_t)&dma1_channel2_isr_handler,                   /* 0x70. */
    (uint32_t)&dma1_channel3_isr_handler,                   /* 0x74. */
    (uint32_t)&dma1_channel4_isr_handler,                   /* 0x78. */
    (uint32_t)&dma1_channel5_isr_handler,                   /* 0x7C. */
    (uint32_t)&dma1_channel6_isr_handler,                   /* 0x80. */
    (uint32_t)&dma1_channel7_isr_handler,                   /* 0x84. */
    (uint32_t)&adc1_adc2_isr_handler,                       /* 0x88. */
    (uint32_t)&usb_hp_or_can_tx_isr_handler,                /* 0x8C. */
    (uint32_t)&usb_lq_or_can_rx0_isr_handler,               /* 0x90. */
    (uint32_t)&can_rx1_isr_handler,                         /* 0x94. */
    (uint32_t)&can_sce_isr_handler,                         /* 0x98. */
    (uint32_t)&exti_9_5_isr_handler,                        /* 0x9C. */
    (uint32_t)&tim1_break_isr_handler,                      /* 0xA0. */
    (uint32_t)&tim1_update_isr_handler,                     /* 0xA4. */
    (uint32_t)&tim1_trigger_communication_isr_handler,      /* 0xA8. */
    (uint32_t)&tim1_capture_compare_isr_handler,            /* 0xAC. */
    (uint32_t)&tim2_isr_handler,                            /* 0xB0. */
    (uint32_t)&tim3_isr_handler,                            /* 0xB4. */
    (uint32_t)&tim4_isr_handler,                            /* 0xB8. */
    (uint32_t)&i2c1_event_isr_handler,                      /* 0xBC. */
    (uint32_t)&i2c1_error_isr_handler,                      /* 0xC0. */
    (uint32_t)&i2c2_event_isr_handler,                      /* 0xC4. */
    (uint32_t)&i2c2_error_isr_handler,                      /* 0xC8. */
    (uint32_t)&spi1_isr_handler,                            /* 0xCC. */
    (uint32_t)&spi2_isr_handler,                            /* 0xD0. */
    (uint32_t)&uart1_isr_handler,                           /* 0xD4. */
    (uint32_t)&uart2_isr_handler,                           /* 0xD8. */
    (uint32_t)&uart3_isr_handler,                           /* 0xDC. */
    (uint32_t)&exti15_10_isr_handler,                       /* 0xE0. */
    (uint32_t)&rtc_alarm_isr_handler,                       /* 0xE4. */
    (uint32_t)&usb_wakeup_isr_handler,                      /* 0xE8. */
    (uint32_t)&tim8_break_isr_handler,                      /* 0xEC. */
    (uint32_t)&tim8_update_isr_handler,                     /* 0xF0. */
    (uint32_t)&tim8_trigger_communication_isr_handler,      /* 0xF4. */
    (uint32_t)&tim8_capture_compare_isr_handler,            /* 0xF8. */
    (uint32_t)&adc3_isr_handler,                            /* 0xFC. */
    (uint32_t)&fsmc_isr_handler,                            /* 0x100. */
    (uint32_t)&sdio_isr_handler,                            /* 0x104. */
    (uint32_t)&tim5_isr_handler,                            /* 0x108. */
    (uint32_t)&spi3_isr_handler,                            /* 0x10C. */
    (uint32_t)&uart4_isr_handler,                           /* 0x110. */
    (uint32_t)&uart5_isr_handler,                           /* 0x114. */
    (uint32_t)&tim6_isr_handler,                            /* 0x118. */
    (uint32_t)&tim7_isr_handler,                            /* 0x11C. */
    (uint32_t)&dma2_channel1_isr_handler,                   /* 0x120. */
    (uint32_t)&dma2_channel2_isr_handler,                   /* 0x124. */
    (uint32_t)&dma2_channel3_isr_handler,                   /* 0x128. */
    (uint32_t)&dma2_channel4_5_isr_handler                  /* 0x12C. */
};


ECU_STATIC_ASSERT( ((sizeof(vector_table) / sizeof(vector_table[0])) == 76U) );



/*------------------------------------------------------------------------------------------------------*/
/*----------------------------------------- STATIC FUNCTION DEFINITIONS --------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

static void hard_fault_isr_handler(void)
{
    #warning "todo reset stack pointer"
    ECU_RUNTIME_ASSERT( (false), ECU_DEFAULT_FUNCTOR );
}


static void memmanage_isr_handler(void)
{
    #warning "todo reset stack pointer"
    ECU_RUNTIME_ASSERT( (false), ECU_DEFAULT_FUNCTOR );
}


static void bus_fault_isr_handler(void)
{
    #warning "todo reset stack pointer"
    ECU_RUNTIME_ASSERT( (false), ECU_DEFAULT_FUNCTOR );
}


static void usage_fault_isr_handler(void)
{
    ECU_RUNTIME_ASSERT( (false), ECU_DEFAULT_FUNCTOR );
}


static void unregistered_isr_handler(void)
{
    ECU_RUNTIME_ASSERT( (false), ECU_DEFAULT_FUNCTOR );
}



/*------------------------------------------------------------------------------------------------------*/
/*----------------------------------------- PUBLIC FUNCTION DEFINITIONS --------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

void reset_isr_handler(void)
{
    /* Do NOT make any static variables here since .data has not been 
    copied over from FLASH to RAM yet. */
    uint32_t bss_size  = (uint32_t)&bss_end_ - (uint32_t)&bss_start_;
    uint32_t data_size = (uint32_t)&data_end_ram_ - (uint32_t)&data_start_ram_;

    /* Step 1: Initialize STDLib. This can be done before initializing
    .data and .bss since STDLib initialization does not need these. We
    also want to initialize STDLib first to use memcpy and memset. */
    __libc_init_array();

    /* Step 2: Copy .data from FLASH into RAM. */
    memcpy((void *)&data_start_ram_, (const void *)&data_start_flash_, data_size);

    /* Step 3: Zero out .bss section. */
    memset((void *)&bss_start_, 0, bss_size);

    /* Step 4: Initialize system clocks and any hardware you need. */
    #warning "todo:"

    /* Step 5: Branch to main. */
    #warning "todo: will I have stack saved that will be unusd since we branch to forever main??"
    main();
}
