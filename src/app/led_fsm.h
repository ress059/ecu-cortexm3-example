/**
 * @file
 * @brief TODO
 * 
 * @author Ian Ress
 * @version 0.1
 * @date 2024-08-17
 * @copyright Copyright (c) 2024
 */


#ifndef LED_FSM_H_
#define LED_FSM_H_



/*-------------------------------------------------------------------------------------*/
/*------------------------------------- INCLUDES --------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/* STDLib. */
#include <stdint.h>

/* ECU. */
#include "ecu/fsm.h"



/*-------------------------------------------------------------------------------------*/
/*------------------------------- LED FSM DATA STRUCTURES -----------------------------*/
/*-------------------------------------------------------------------------------------*/

enum led_fsm_event_signals
{
    LED_FSM_SWITCH_PRESSED_EVT = ECU_USER_EVENT_ID_BEGIN,
    LED_FSM_SWITCH_RELEASED_EVT,
    LED_FSM_TIMEOUT_EVT
};


// This is NOT the state the fsm is in. This indicates to the driver
// whether the LED should be turned on or off.
enum led_fsm_led_state
{
    LED_FSM_LED_STATE_ON,
    LED_FSM_LED_STATE_OFF
};


struct led_fsm
{
    struct ecu_fsm base_fsm; /* MUST be first. */
    uint32_t hold_time_ms;
    uint32_t toggle_time_ms;
    enum led_fsm_led_state led_state;
    
    struct 
    {
        void *i_obj;
        void (*i_led_set)(void *i_obj, enum led_fsm_led_state state);
        void (*i_timer_arm)(void *i_obj, uint32_t ms);
        void (*i_timer_disarm)(void *i_obj);
    } api;
};


struct led_fsm_event
{
    struct ecu_event base_event; /* MUST be first. */
};



/*-------------------------------------------------------------------------------------*/
/*---------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
/*-------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern void led_fsm_ctor(struct led_fsm *me,
                         uint32_t hold_time_ms_0,
                         uint32_t toggle_time_ms_0,
                         void *i_obj_0,
                         void (*i_led_set_0)(void *i_obj, enum led_fsm_led_state state),
                         void (*i_timer_arm_0)(void *i_obj, uint32_t ms),
                         void (*i_timer_disarm_0)(void *i_obj));

#ifdef __cplusplus
}
#endif

#endif /* LED_FSM_H_ */
