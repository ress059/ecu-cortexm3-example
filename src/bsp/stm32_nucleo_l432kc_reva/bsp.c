/**
 * @file
 * @brief
 * 
 * @author Ian Ress 
 * @version 0.1
 * @date 2024-08-17
 * @copyright Copyright (c) 2024
 */



/*-------------------------------------------------------------------------------------*/
/*------------------------------------- INCLUDES --------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/* Translation unit. */
#include "bsp/bsp.h"

/* LED FSM. */
#include "app/led_fsm.h"

/* External libraries. ECU. */
#include "ecu/fsm.h"
#include "ecu/interface/itimer.h"
#include "ecu/timer.h"



/*-------------------------------------------------------------------------------------*/
/*----------------------------------- FILE SCOPE DEFINES ------------------------------*/
/*-------------------------------------------------------------------------------------*/

/**
 * @brief Return how many ticks correspond to one millisecond.
 * For this board the systick fires every 1ms so it is a 1:1 
 * ratio.
 */
#define MS_TO_TICKS(ms)                         (ms)
#define LED0_HOLD_TIME_MS                       (3000)
#define LED0_TOGGLE_TIME_MS                     (1000)
#define LED1_HOLD_TIME_MS                       (6000)
#define LED1_TOGGLE_TIME_MS                     (500)



/*-------------------------------------------------------------------------------------*/
/*----------------------------------- FILE SCOPE TYPES --------------------------------*/
/*-------------------------------------------------------------------------------------*/

struct led
{
    struct ecu_timer timer;
    struct led_fsm fsm;
    struct led_fsm_event evt;
};



/*-------------------------------------------------------------------------------------*/
/*------------------------------ STATIC FUNCTION DECLARATIONS -------------------------*/
/*-------------------------------------------------------------------------------------*/

static void led0_set(void *led, enum led_fsm_led_state state); // sets gpio connected to led0 for this board.
static void led1_set(void *led, enum led_fsm_led_state state); // sets gpio connected to led1 for this board.
static ecu_max_tick_size_t get_ticks(struct i_ecu_timer *me); // returns number of ticks from whatever time source is used for this board.
static void led_timer_arm(void *led, uint32_t ms);
static void led_timer_disarm(void *led);
static bool led_timeout_callback(void *led);



/*-------------------------------------------------------------------------------------*/
/*----------------------------------- GLOBAL VARIABLES --------------------------------*/
/*-------------------------------------------------------------------------------------*/

// TODO Set assert handler for this board.
struct ecu_assert_functor *const BSP_ASSERT_FUNCTOR = (struct ecu_assert_functor *)0;



/*-------------------------------------------------------------------------------------*/
/*--------------------------------- FILE SCOPE VARIABLES ------------------------------*/
/*-------------------------------------------------------------------------------------*/

static struct
{
    struct ecu_timer_collection timer_head;
} led_collection;


static struct led leds[2];



/*-------------------------------------------------------------------------------------*/
/*------------------------------ STATIC FUNCTION DEFINITIONS --------------------------*/
/*-------------------------------------------------------------------------------------*/

static void led0_set(void *led, enum led_fsm_led_state state)
{
    (void)led;
    // TODO set gpio connected to led0
}


static void led1_set(void *led, enum led_fsm_led_state state)
{
    (void)led;
    // TODO set gpio connected to led1
}


static ecu_max_tick_size_t get_ticks(struct i_ecu_timer *me)
{
    /* Wrapper function to accomodate any form the systick driver
    function may have. */
    (void)me;
    // TODO return systick_get_ticks();
    return 0;
}


// precondition is all timers have to be constructed.
static void led_timer_arm(void *led, uint32_t ms)
{
    struct led *me = (struct led *)0;
    ECU_RUNTIME_ASSERT( (led), BSP_ASSERT_FUNCTOR );

    me = (struct led *)led;
    ecu_timer_arm(&led_collection.timer_head, &me->timer, false, MS_TO_TICKS(ms));
}


static void led_timer_disarm(void *led)
{
    struct led *me = (struct led *)0;
    ECU_RUNTIME_ASSERT( (led), BSP_ASSERT_FUNCTOR );

    me = (struct led *)led;
    ecu_timer_disarm(&me->timer);
}


static bool led_timeout_callback(void *led)
{
    static const struct led_fsm_event timeout_evt =
    {
        .base_event.id = LED_FSM_TIMEOUT_EVT
    };

    struct led *me = (struct led *)0;
    ECU_RUNTIME_ASSERT( (led), BSP_ASSERT_FUNCTOR );
    me = (struct led *)led;

    ecu_fsm_dispatch((struct ecu_fsm *)(&me->fsm), (const struct ecu_event *)&timeout_evt);
    return true;
}



/*-------------------------------------------------------------------------------------*/
/*---------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
/*-------------------------------------------------------------------------------------*/

void led_fsms_init(void)
{
    static struct i_ecu_timer timer_api;
    i_ecu_timer_ctor(&timer_api, sizeof(uint32_t), &get_ticks);
    ecu_timer_collection_ctor(&led_collection.timer_head, &timer_api);

    /* Construct LED #0 with board-specific settings. */
    ecu_timer_ctor(&leds[0].timer, (void *)&leds[0], &led_timeout_callback);
    led_fsm_ctor(&leds[0].fsm, LED0_HOLD_TIME_MS, LED0_TOGGLE_TIME_MS, (void *)&leds[0], 
                 &led0_set, &led_timer_arm, &led_timer_disarm);

    /* Construct LED #1 with board-specific settings. */
    ecu_timer_ctor(&leds[1].timer, (void *)&leds[1], &led_timeout_callback);
    led_fsm_ctor(&leds[1].fsm, LED1_HOLD_TIME_MS, LED1_TOGGLE_TIME_MS, (void *)&leds[1], 
                 &led1_set, &led_timer_arm, &led_timer_disarm);
}


void led_fsms_run(void)
{
    /* Dispatch timeout events to relevant LED FSMs first. */
    ecu_timer_collection_tick(&led_collection.timer_head);

    // TODO: Get all switch inputs, debounce, dispatch SW_PRESSED and SW_RELEASED events.
}
