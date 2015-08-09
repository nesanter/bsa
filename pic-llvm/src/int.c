#include "runtime.h"
#include "driver/console.h"
#include "driver/basic_io.h"
#include "driver/timer.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"
#include "ulib/pins.h"
#include "exception.h"
#include "task.h"

#define DRV_SUCCESS (1)
#define DRV_FAILURE (0)

#define DRV_TRUE (1)
#define DRV_FALSE (0)

extern struct task_info *current_task;

extern handler_t volatile __vector_table[43];

/*
void handler_change_notify() {
    
//    unblock_tasks(BLOCK_REASON_CHANGE_NOTIFY, 
}

void handler_change_notify_b() {

}

void handler_uart1_rx() {
    unblock_tasks(BLOCK_REASON_UART1_RX, 0);
}

void handler_timerb() {
//    unblock_tasks(BLOCK_REASON_TIMER_B
}
*/

