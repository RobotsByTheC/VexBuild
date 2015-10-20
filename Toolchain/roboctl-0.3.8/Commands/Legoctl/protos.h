/* legoctl.c */
int main(int argc, char *argv[]);
int legoctl(rct_cmd_t cmd, arg_t *arg_data, unsigned int flags);
int multi_brick_cmd(rct_brick_list_t *bricks, rct_cmd_t cmd, unsigned int flags);
int file_cmd(rct_brick_list_t *bricks, char *filename, rct_cmd_t cmd, unsigned int flags);
int play_tone(rct_brick_list_t *bricks, int herz, int milliseconds);
void legoctl_usage(char *progname);
int parse_args(int argc, char *argv[], rct_cmd_t *cmd, arg_t *arg_data, unsigned int *flags);
