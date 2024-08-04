
stack_pointer
reset_handler
nmi_handler
hard_fault_handler
mem_manage_handler
bus_fault_handler
usage_fault_handler
default_handler // reserved 0x1C to 0x2B
svcall_handler
debug_monitor_handler
default_handler //reserved 0x34
pendsv_handler
systick_isr_handler
// mcu-specific handlers
wwdg_isr_handler // window watchdog interrupt
pvd_isr_handler
tamper_isr_handler
rtc_isr_handler
flash_isr_handler
rcc_isr_handler
exti0_isr_handler
exti1_isr_handler
exti2_isr_handler
exti3_isr_handler
exti4_isr_handler
dma1_channel1_isr_handler
dma1_channel2_isr_handler
dma1_channel3_isr_handler
dma1_channel4_isr_handler
dma1_channel5_isr_handler
dma1_channel6_isr_handler
dma1_channel7_isr_handler
adc1_adc2_isr_handler
usb_hp_or_can_tx_isr_handler
usb_lq_or_can_rx0_isr_handler
can_rx1_isr_handler
can_sce_isr_handler
exti_9_5_isr_handler
tim1_break_isr_handler
tim1_update_isr_handler
tim1_trigger_communication_isr_handler
tim1_capture_compare_isr_handler
tim2_isr_handler
tim3_isr_handler
tim4_isr_handler
i2c1_event_isr_handler
i2c1_error_isr_handler
i2c2_event_isr_handler
i2c2_error_isr_handler
spi1_isr_handler
spi2_isr_handler
uart1_isr_handler
uart2_isr_handler
uart3_isr_handler
exti15_10_isr_handler
rtc_alarm_isr_handler
usb_wakeup_isr_handler
tim8_break_isr_handler
tim8_update_isr_handler
tim8_trigger_communication_isr_handler
tim8_capture_compare_isr_handler
adc3_isr_handler
fsmc_isr_handler
sdio_isr_handler
tim5_isr_handler
spi3_isr_handler
uart4_isr_handler
uart5_isr_handler
tim6_isr_handler
tim7_isr_handler
dma2_channel1_isr_handler
dma2_channel2_isr_handler
dma2_channel3_isr_handler
dma2_channel4_5_isr_handler



static const uint32_t vector_table[] __attribute__((section(".isr_vector"))) =
{
    stack_pointer
    reset_handler
    nmi_handler
    hard_fault_handler
    mem_manage_handler
    bus_fault_handler
    usage_fault_handler
    default_handler // reserved 0x1C to 0x2B
    svcall_handler
    debug_monitor_handler
    default_handler //reserved 0x34
    pendsv_handler
    systick_isr_handler
    // mcu-specific handlers
    wwdg_isr_handler // window watchdog interrupt
    pvd_isr_handler
    tamper_isr_handler
    rtc_isr_handler
    flash_isr_handler
    rcc_isr_handler
    exti0_isr_handler
    exti1_isr_handler
    exti2_isr_handler
    exti3_isr_handler
    exti4_isr_handler
    dma1_channel1_isr_handler
    dma1_channel2_isr_handler
    dma1_channel3_isr_handler
    dma1_channel4_isr_handler
    dma1_channel5_isr_handler
    dma1_channel6_isr_handler
    dma1_channel7_isr_handler
    adc1_adc2_isr_handler
    usb_hp_or_can_tx_isr_handler
    usb_lq_or_can_rx0_isr_handler
    can_rx1_isr_handler
    can_sce_isr_handler
    exti_9_5_isr_handler
    tim1_break_isr_handler
    tim1_update_isr_handler
    tim1_trigger_communication_isr_handler
    tim1_capture_compare_isr_handler
    tim2_isr_handler
    tim3_isr_handler
    tim4_isr_handler
    i2c1_event_isr_handler
    i2c1_error_isr_handler
    i2c2_event_isr_handler
    i2c2_error_isr_handler
    spi1_isr_handler
    spi2_isr_handler
    uart1_isr_handler
    uart2_isr_handler
    uart3_isr_handler
    exti15_10_isr_handler
    rtc_alarm_isr_handler
    usb_wakeup_isr_handler
    tim8_break_isr_handler
    tim8_update_isr_handler
    tim8_trigger_communication_isr_handler
    tim8_capture_compare_isr_handler
    adc3_isr_handler
    fsmc_isr_handler
    sdio_isr_handler
    tim5_isr_handler
    spi3_isr_handler
    uart4_isr_handler
    uart5_isr_handler
    tim6_isr_handler
    tim7_isr_handler
    dma2_channel1_isr_handler
    dma2_channel2_isr_handler
    dma2_channel3_isr_handler
    dma2_channel4_5_isr_handler 
}










