/**
 * @file
 * @brief TODO
 * 
 * @author Ian Ress
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 */



/*-------------------------------------------------------------------------------------*/
/*------------------------------------- INCLUDES --------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/* Translation unit. */
#include "app/led_fsm.h"

/* STDLib. */
#include <stdbool.h>
#include <stddef.h> /* offsetof */

/* External libraries. ECU. */
#include "ecu/asserter.h"
#include "ecu/fsm.h"

/* Board support package. Asserts. */
#include "bsp/bsp.h"



/*-------------------------------------------------------------------------------------*/
/*---------------------------------- STATIC ASSERTS -----------------------------------*/
/*-------------------------------------------------------------------------------------*/

/* Assert that struct ecu_fsm and struct ecu_event are first members of these
structs. This memory alignment is required in order to use C-style inheritance. */
ECU_STATIC_ASSERT( (offsetof(struct led_fsm, base_fsm) == 0) );
ECU_STATIC_ASSERT( (offsetof(struct led_fsm_event, base_event) == 0) );


/*-------------------------------------------------------------------------------------*/
/*------------------------- STATIC FUNCTION DECLARATIONS - CHECKS ---------------------*/
/*-------------------------------------------------------------------------------------*/

static bool is_constructed(struct led_fsm *me);



/*-------------------------------------------------------------------------------------*/
/*----------------------- STATIC FUNCTION DECLARATIONS - OFF STATE --------------------*/
/*-------------------------------------------------------------------------------------*/

static enum ecu_fsm_status off_state_on_entry(struct led_fsm *me);
static void off_state_on_exit(struct led_fsm *me);
static enum ecu_fsm_status off_state_handler(struct led_fsm *me, 
                                             const struct led_fsm_event *evt);



/*-------------------------------------------------------------------------------------*/
/*----------------------- STATIC FUNCTION DECLARATIONS - ON STATE ---------------------*/
/*-------------------------------------------------------------------------------------*/

static enum ecu_fsm_status on_state_on_entry(struct led_fsm *me);
static void on_state_on_exit(struct led_fsm *me);
static enum ecu_fsm_status on_state_handler(struct led_fsm *me, 
                                            const struct led_fsm_event *evt);



/*-------------------------------------------------------------------------------------*/
/*------------------- STATIC FUNCTION DECLARATIONS - HELD DOWN STATE ------------------*/
/*-------------------------------------------------------------------------------------*/

static enum ecu_fsm_status held_down_state_on_entry(struct led_fsm *me);
static void held_down_state_on_exit(struct led_fsm *me);
static enum ecu_fsm_status held_down_state_handler(struct led_fsm *me, 
                                                   const struct led_fsm_event *evt);



/*-------------------------------------------------------------------------------------*/
/*--------------------------------- FILE SCOPE VARIABLES ------------------------------*/
/*-------------------------------------------------------------------------------------*/

static const struct ecu_fsm_state off_state =
{
    .handler    = (ecu_fsm_state_handler)&off_state_handler,
    .on_entry   = (ecu_fsm_on_entry_handler)&off_state_on_entry,
    .on_exit    = (ecu_fsm_on_exit_handler)&off_state_on_exit
};


static const struct ecu_fsm_state on_state =
{
    .handler    = (ecu_fsm_state_handler)&on_state_handler,
    .on_entry   = (ecu_fsm_on_entry_handler)&on_state_on_entry,
    .on_exit    = (ecu_fsm_on_exit_handler)&on_state_on_exit
};


static const struct ecu_fsm_state held_down_state =
{
    .handler    = (ecu_fsm_state_handler)&held_down_state_handler,
    .on_entry   = (ecu_fsm_on_entry_handler)&held_down_state_on_entry,
    .on_exit    = (ecu_fsm_on_exit_handler)&held_down_state_on_exit
};



/*-------------------------------------------------------------------------------------*/
/*------------------------- STATIC FUNCTION DEFINITIONS - CHECKS ----------------------*/
/*-------------------------------------------------------------------------------------*/

static bool is_constructed(struct led_fsm *me)
{
    bool status = false;
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );

    if ((me->hold_time_ms > 0) && \
        (me->toggle_time_ms > 0) && \
        (me->api.i_led_toggle) && \
        (me->api.i_led_on) && \
        (me->api.i_led_off) && \
        (me->api.i_timer_arm) && \
        (me->api.i_timer_disarm))
    {
        status = true;
    }

    return status;
}



/*-------------------------------------------------------------------------------------*/
/*----------------------- STATIC FUNCTION DEFINITIONS - OFF STATE ---------------------*/
/*-------------------------------------------------------------------------------------*/

static enum ecu_fsm_status off_state_on_entry(struct led_fsm *me)
{
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );
    (*me->api.i_led_off)();
    (*me->api.i_timer_disarm)(me);
}


static void off_state_on_exit(struct led_fsm *me)
{
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );

    /* Suppress unused parameter warning if asserts disabled. */
#if defined ECU_DISABLE_RUNTIME_ASSERTS
    (void)me;
#endif
}


static enum ecu_fsm_status off_state_handler(struct led_fsm *me, 
                                             const struct led_fsm_event *evt)
{
    enum ecu_fsm_status status = ECU_FSM_EVENT_HANDLED;
    ECU_RUNTIME_ASSERT( (me && evt), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );

    switch (((const struct ecu_event *)evt)->id)
    {
        case LED_FSM_SWITCH_PRESSED_EVT:
        {
            status = ecu_fsm_transition_to_state((struct ecu_fsm *)me, &on_state);
            break;
        }

        default:
        {
            status = ECU_FSM_EVENT_IGNORED;
            break;
        }
    }

    return status;
}



/*-------------------------------------------------------------------------------------*/
/*------------------------ STATIC FUNCTION DEFINITIONS - ON STATE ---------------------*/
/*-------------------------------------------------------------------------------------*/

static enum ecu_fsm_status on_state_on_entry(struct led_fsm *me)
{
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );
    (*me->api.i_led_on)();
    (*me->api.i_timer_arm)(me, me->hold_time_ms);
}


static void on_state_on_exit(struct led_fsm *me)
{
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );

    /* Suppress unused parameter warning if asserts disabled. */
#if defined ECU_DISABLE_RUNTIME_ASSERTS
    (void)me;
#endif
}


static enum ecu_fsm_status on_state_handler(struct led_fsm *me, 
                                            const struct led_fsm_event *evt)
{
    enum ecu_fsm_status status = ECU_FSM_EVENT_HANDLED;
    ECU_RUNTIME_ASSERT( (me && evt), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );

    switch (((const struct ecu_event *)evt)->id)
    {
        case LED_FSM_SWITCH_RELEASED_EVT:
        {
            status = ecu_fsm_transition_to_state((struct ecu_fsm *)me, &off_state);
            break;
        }

        case LED_FSM_TIMEOUT_EVT:
        {
            status = ecu_fsm_transition_to_state((struct ecu_fsm *)me, &held_down_state);
            break;
        }

        default:
        {
            status = ECU_FSM_EVENT_IGNORED;
            break;
        }
    }

    return status;
}



/*-------------------------------------------------------------------------------------*/
/*-------------------- STATIC FUNCTION DEFINITIONS - HELD DOWN STATE ------------------*/
/*-------------------------------------------------------------------------------------*/

static enum ecu_fsm_status held_down_state_on_entry(struct led_fsm *me)
{
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );
    (*me->api.i_timer_arm)(me, me->toggle_time_ms);
}


static void held_down_state_on_exit(struct led_fsm *me)
{
    ECU_RUNTIME_ASSERT( (me), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );

    /* Suppress unused parameter warning if asserts disabled. */
#if defined ECU_DISABLE_RUNTIME_ASSERTS
    (void)me;
#endif
}


static enum ecu_fsm_status held_down_state_handler(struct led_fsm *me, 
                                                   const struct led_fsm_event *evt)
{
    enum ecu_fsm_status status = ECU_FSM_EVENT_HANDLED;
    ECU_RUNTIME_ASSERT( (me && evt), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (is_constructed(me)), BSP_ASSERT_FUNCTOR );

    switch (((const struct ecu_event *)evt)->id)
    {
        case LED_FSM_SWITCH_RELEASED_EVT:
        {
            status = ecu_fsm_transition_to_state((struct ecu_fsm *)me, &off_state);
            break;
        }

        case LED_FSM_TIMEOUT_EVT:
        {
            (*me->api.i_led_toggle)();
            (*me->api.i_timer_arm)(me, me->toggle_time_ms);
            break;
        }

        default:
        {
            status = ECU_FSM_EVENT_IGNORED;
            break;
        }
    }

    return status;
}



/*-------------------------------------------------------------------------------------*/
/*---------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
/*-------------------------------------------------------------------------------------*/

void led_fsm_ctor(struct led_fsm *me,
                  uint32_t hold_time_ms_0,
                  uint32_t toggle_time_ms_0,
                  void(*i_led_toggle_0)(void),
                  void(*i_led_on_0)(void),
                  void(*i_led_off_0)(void),
                  void (*i_timer_arm_0)(struct led_fsm *me, uint32_t ms),
                  void (*i_timer_disarm_0)(struct led_fsm *me))
{
    ECU_RUNTIME_ASSERT( ((hold_time_ms_0 > 0) && (toggle_time_ms_0 > 0)), BSP_ASSERT_FUNCTOR );
    ECU_RUNTIME_ASSERT( (me && i_led_toggle_0 && i_led_on_0 && i_led_off_0 && i_timer_arm_0 && i_timer_disarm_0),
                        BSP_ASSERT_FUNCTOR );

    ecu_fsm_ctor((struct ecu_fsm *)me, &off_state);
    me->hold_time_ms        = hold_time_ms_0;
    me->toggle_time_ms      = toggle_time_ms_0;
    me->api.i_led_toggle    = i_led_toggle_0;
    me->api.i_led_on        = i_led_on_0;
    me->api.i_led_off       = i_led_off_0;
    me->api.i_timer_arm     = i_timer_arm_0;
    me->api.i_timer_disarm  = i_timer_disarm_0;
}
