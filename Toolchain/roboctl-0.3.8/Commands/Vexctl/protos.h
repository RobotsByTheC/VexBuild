/* vexctl.c */
int main(int argc, char *argv[]);
int vexctl(rct_cmd_t cmd, arg_t *arg_data, unsigned int flags);
void vexctl_usage(char *progname);
int parse_args(int argc, char *argv[], rct_cmd_t *cmd, arg_t *arg_data, unsigned int *flags);
int parse_global_flags(int argc, char *argv[], rct_cmd_t *cmd, arg_t *arg_data, unsigned int *flags, int *argp);
void monitor_controller(rct_pic_t *pic);

