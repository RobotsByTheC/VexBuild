#include "rct_nxt_output.h"

void    nxt_output_init(nxt_output_state_t *nxt_output)

{
    nxt_output->mode = NXT_MODE_BRAKE;
    nxt_output->regulation_mode = NXT_REGULATION_MODE_IDLE;
    nxt_output->run_state = NXT_RUN_STATE_IDLE;
    nxt_output->power = 0;
    nxt_output->turn_ratio = 0;
    nxt_output->tacho_limit = 0;
}

