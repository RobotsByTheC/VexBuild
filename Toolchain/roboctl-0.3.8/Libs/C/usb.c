#include <stdio.h>
#include <usb.h>

int     usb_device_info(struct usb_device *dev)

{
    int     a,i,c;
    
    /*printf("USB info:\n\t%d config(s).\n", dev->descriptor.bNumConfigurations);*/
    /* Loop through all of the configurations */
    for (c = 0; c < dev->descriptor.bNumConfigurations; c++)
    {
	if ( dev->config == NULL )
	{
	    fputs("---------------------------------------------------------------------------\n",stderr);
	    fputs("Could not read USB configurations.  This is probably a permissions problem.\n",stderr);
#ifdef __FreeBSD__
	    fputs("Check /dev/usb* and /dev/ugen*.\n",stderr);
#endif
	    fputs("Run 'man roboctl' for more information.\n",stderr);
	    fputs("---------------------------------------------------------------------------\n",stderr);
	    return 1;
	}
	/*printf("\t%d interface(s) for config %d.\n",
	       dev->config[c].bNumInterfaces, c);
	fflush(stdout);*/
	/* Loop through all of the interfaces */
	for (i = 0; i < dev->config[c].bNumInterfaces; i++)
	{
	    /*printf("\t%d altsetting(s) for interface %d.\n",
		   dev->config[c].interface[i].num_altsetting, i);*/
	    /* Loop through all of the alternate settings */
	    for (a = 0; a < dev->config[c].interface[i].num_altsetting; a++)
	    {
		/* Check if this interface is a printer */
		/*printf("\tInterface Class = %d\n",
		       dev->config[c].interface[i].altsetting[a].bInterfaceClass);*/
	    }
	}
    }
    return 0;
}

