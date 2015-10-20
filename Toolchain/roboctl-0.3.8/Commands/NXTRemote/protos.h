/* arcade_drive.c */
void swap(int *a, int *b);
void arcade_drive(int x, int y, int max_pos, int max_power, float warp_factor, int *left_speed_ptr, int *right_speed_ptr);
/* nxtremote.c */
int main(int argc, char *argv[]);
int joy_scaled_x(int x, int max_x);
int joy_scaled_y(int y, int max_y);
void control_motion(rct_brick_t *brick, int x, int y, settings_t *settings);
void control_implement_with_button(rct_brick_t *brick, int b1, int b2, settings_t *settings);
void control_implement_with_joystick(rct_brick_t *brick, int z, int max_z, settings_t *settings);
int process_args(int argc, char **argv, char **bt_name, settings_t *settings);
void usage(char *argv[]);
