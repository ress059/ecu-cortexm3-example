/**
 * @file
 * @brief TODO
 * 
 * @author Ian Ress
 * @version 0.1
 * @date 2024-08-17
 * @copyright Copyright (c) 2024
 */


#ifndef BSP_H_
#define BSP_H_

#include "ecu/asserter.h"


extern struct ecu_assert_functor *const BSP_ASSERT_FUNCTOR;


#ifdef __cplusplus
extern "C" {
#endif

extern void led_fsms_init(void);
extern void led_fsms_run(void);



#ifdef __cplusplus
}
#endif


#endif /* BSP_H_ */
