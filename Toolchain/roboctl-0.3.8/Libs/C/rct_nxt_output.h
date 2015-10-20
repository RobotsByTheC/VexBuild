/* SET_OUTPUT_STATE */
typedef enum
{
    NXT_MODE_MOTORON = 0x01,
    NXT_MODE_BRAKE = 0x02,
    NXT_MODE_REGULATED = 0x04
}   nxt_output_mode_t;

typedef enum
{
    NXT_REGULATION_MODE_IDLE = 0x00,
    NXT_REGULATION_MODE_MOTOR_SPEED = 0x01,
    NXT_REGULATION_MODE_MOTOR_SYNC = 0X02
}   nxt_output_regulation_mode_t;

typedef enum
{
    NXT_RUN_STATE_IDLE = 0x00,
    NXT_RUN_STATE_RAMPUP = 0x10,
    NXT_RUN_STATE_RUNNING = 0x20,
    NXT_RUN_STATEE_RAMPDOWN = 0x40
}   nxt_output_runstate_t;

typedef struct
{
    nxt_output_mode_t               mode;
    nxt_output_regulation_mode_t    regulation_mode;
    nxt_output_runstate_t           run_state;
    int                             power;
    int                             turn_ratio;
    unsigned long                   tacho_limit;
}   nxt_output_state_t;

#define NXT_OUTPUT_INIT { NXT_MODE_BRAKE, NXT_REGULATION_MODE_IDLE, \
			    NXT_RUN_STATE_IDLE, 0, 0, 0 }

