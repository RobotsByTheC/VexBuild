
/****************************************************************************
 * Class:           brick list
 * Structure type:  rct_brick_list_t
 * 
 * This file contains functions dealing with the brick list.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <sysexits.h>
#include "roboctl.h"

/**
 *  \addtogroup roboctl
 *
 *  @{
 */
 
int     rct_find_bricks(rct_brick_list_t * bricks, char *name,
			unsigned int flags)

{
    rct_set_count(bricks,0);
    if (flags & RCT_PROBE_DEV_NXT)
	/* 2nd arg is a bluetooth device name. Pass NULL for default. */
	rct_find_nxt_bricks(bricks,name);
    return bricks->count;
}


int     rct_find_nxt_bricks(rct_brick_list_t * bricks,char *name)

{
    debug_printf("Finding bricks...\n");
    if ( rct_find_nxt_usb(bricks) == 0 )
	puts("No bricks found on USB interface.");
#if defined(__FreeBSD__) || defined(__linux__)
    if ( bricks->count == 0 )
	rct_find_nxt_bluetooth(bricks,name);
#endif
    return bricks->count;
}


int     rct_find_nxt_usb(rct_brick_list_t * bricks)

{
    struct usb_bus *busses, *bus;
    struct usb_device *dev;
    int     count = 0,
	    bus_num = 0,
	    dev_num = 0;

    debug_printf("Initializing USB...\n");
    usb_init();
    debug_printf("Finding busses...\n");
    usb_find_busses();
    debug_printf("Finding devices...\n");
    usb_find_devices();

    debug_printf("Getting busses...\n");
    busses = usb_get_busses();

    for (bus = busses; bus != NULL; bus = bus->next, ++bus_num)
    {
	for (dev = bus->devices; dev != NULL; dev = dev->next, ++dev_num)
	{
	    if (dev->descriptor.idVendor == RCT_VENDOR_LEGO &&
		dev->descriptor.idProduct == RCT_PRODUCT_NXT)
	    {
		debug_printf("Found NXT on USB bus %d, dev %d.\n",
		       bus_num, dev_num);
		if ( usb_device_info(dev) == 0 )
		{
		    rct_init_brick_struct(&bricks->bricks[count],RCT_NXT);
		    NXT_SET_USB_DEV(&(bricks->bricks[count].nxt),dev);
		    ++count;
		}
	    }
	}
    }

    rct_increase_count(bricks,count);
    return count;
}


int     rct_find_nxt_bluetooth(rct_brick_list_t *bricks,char *name)

{
#if defined(__FreeBSD__) || defined(__Linux__)
    int     count = 0;
    struct hostent *host;
    char    *bt_env;
    
    rct_init_brick_struct(&bricks->bricks[count],RCT_NXT);

    if ( name == NULL )
    {
	/* Check environment */
	if ( (bt_env = getenv("ROBOCTL_BTNAME")) != NULL )
	    name = bt_env;
	else
	    name = "NXT";
    }
    
    /* Check bluetooth hosts */
    host = bt_gethostbyname(name);
    if ( host != NULL )
    {
	printf("Trying %s...\n",host->h_name);
	nxt_copy_hostent_bt_addr(&(bricks->bricks[count].nxt),
	    host->h_addr);
	++count;
	rct_increase_count(bricks, count); 
    }
    else
	fprintf(stderr,"%s(): %s was not found in the bluetooth hosts database.\n",
		__func__, name);
    return bricks->count;
#else
    return 0;
#endif
}


/** 
    \brief Get brick N from an rct_brick_list_t structure.
    \param list - a structure containing an array of bricks.
    \param n - the index of the desired brick.
    \author Jason W. Bacon
    
    Return a pointer to brick N in the array of bricks.
 */
 
rct_brick_t *rct_get_brick_from_list(rct_brick_list_t *list,int n)

{
    return &list->bricks[n];
}

/** 
    \brief Return the number of bricks in an rct_brick_list_t structure
 */

int     rct_brick_count(rct_brick_list_t *bricks)

{
    return bricks->count;
}

/**
    \brief Set the number of bricks in an rct_brick_list_t structure 
 */

rct_status_t    rct_set_count(rct_brick_list_t *bricks,int n)

{
    if ( n <= RCT_MAX_BRICKS )
    {
	bricks->count = n;
	return RCT_OK;
    }
    else
    {
	fprintf(stderr,"rct_set_count(): %d is beyond the maximum of %d\n",
		n,RCT_MAX_BRICKS);
	return RCT_INVALID_DATA;
    }
}

/**
    \brief Set the number of bricks in an rct_brick_list_t structur 
 */
 
rct_status_t    rct_increase_count(rct_brick_list_t *bricks,int n)

{
    if ( bricks->count + n <= RCT_MAX_BRICKS )
    {
	bricks->count += n;
	return RCT_OK;
    }
    else
    {
	fprintf(stderr,"rct_increment_count(): %d is already the maximum number of bricks.\n",
		bricks->count);
	return RCT_INVALID_DATA;
    }
}

/** @} */

