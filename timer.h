#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include <pico/time.h>

#define MAX_TIMERS 5

// Timer action types
typedef enum {
    EVENT_NONE,
    EVENT_INTERVAL,       // Recurring based on interval_ms
    EVENT_ALARM_DAILY,    // Recurring based on HH:MM:SS
    EVENT_ALARM_ONESHOT   // One-shot based on M/D/YY HH:MM:SS
} TimerActionType;

// Timer target type (GOSUB or CALL)
typedef enum {
    TARGET_GOSUB,
    TARGET_CALL
} TargetType;

// Timer Event Configuration Structure
typedef struct {
    int interval_ms;
    TimerActionType action_type;
    TargetType target_type;      // GOSUB or CALL
    int target_line;             // For GOSUB
    char target_name[32];        // For CALL
    bool enabled;
    absolute_time_t next_fire_time;
} TimerEvent;

// Initialize timer system (call once at startup)
void timer_system_init(void);

// Set up a timer/alarm event
void timer_set_interval(int index, int interval_ms, TargetType target_type, 
                        int target_line, const char *target_name);
void timer_set_alarm_daily(int index, int hour, int minute, int second,
                           TargetType target_type, int target_line, const char *target_name);
void timer_set_alarm_oneshot(int index, int month, int day, int year,
                             int hour, int minute, int second,
                             TargetType target_type, int target_line, const char *target_name);

// Disable a timer
void timer_disable(int index);

// Check if any timers need to fire and return the next one
TimerEvent *timer_check_and_get_next(void);

// Mark timer as fired (reschedule or disable based on type)
void timer_mark_fired(int index);

// Get/set handler state (for interrupt suppression)
bool timer_is_in_handler(void);
void timer_set_in_handler(bool in_handler);

// Clear the "just fired" flag to allow timer re-checking
void timer_clear_last_fired(void);

// Persist timers across NEW command (called before clearing program)
void timer_system_save_state(void);
void timer_system_restore_state(void);

// Get access to timer array (for debugging/listing)
TimerEvent *timer_get_array(void);

// Check if any timer is ready to fire (for cooperative multitasking)
bool timer_is_ready(void);

#endif // TIMER_H
