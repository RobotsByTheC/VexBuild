
#if defined(__FreeBSD__)
#include <bluetooth.h>
#endif

#if defined(__linux__)

/* FreeBSD and Linux bluetooth are *almost* identical.  #define the
 * FreeBSD constants here to keep nxt_open_bluetooth() free of
 * conditionals.
 */

#define BLUETOOTH_PROTO_RFCOMM  BTPROTO_RFCOMM
#define NG_HCI_BDADDR_ANY       BDADDR_ANY
#define sockaddr_rfcomm         sockaddr_rc
#define rfcomm_bdaddr           rc_bdaddr
#define rfcomm_family           rc_family
#define rfcomm_channel          rc_channel

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bacon.h>
#endif

