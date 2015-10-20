#include <stdio.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFNumber.h>
#include <IOBluetooth/IOBluetoothUserLib.h>

#define NXT_RFCOMM_CHANNEL_ID   1

void    listener(IOBluetoothRFCOMMChannelRef rfcommChannel, void *data, UInt16 length, void *refCon);

int     main(int argc,char *argv[])

{
    BluetoothDeviceAddress      btAddr = {{0x00,0x16,0x53,0x01,0x11,0xB9}};
    BluetoothDeviceName         btName;
    IOBluetoothRFCOMMChannelRef rfcommChannel;
    IOBluetoothDeviceRef        btDevice;
    CFStringRef                 name;
    char                        raw_name[100],
				cmd[100],
				buff[100];
    int                         new_data[100],
				handle,
				bytes;
    
    /* Create a BluetoothDeviceAddress for use in setting up a connection */
    btDevice = IOBluetoothDeviceCreateWithAddress(&btAddr);
    printf("btDevice = %p\n",btDevice);
    
    /* Quick test of address */
    if ( IOBluetoothDeviceRemoteNameRequest(btDevice,NULL,NULL,btName) !=
	    kIOReturnSuccess )
    {
	fprintf(stderr,"IOBluetoothDeviceRemoteNameRequest() failed.\n");
	return 1;
    }
    else
    {
	name = IOBluetoothDeviceGetName(btDevice);
	printf("CFString name = %p\n",name);
	if ( CFStringGetCString(name,raw_name,100,kCFStringEncodingISOLatin1) )
	    printf("Device name = %s\n",raw_name);
	else
	    fprintf(stderr,"CFStringGetCString() failed.\n");
    }
    
    /* Open an RFCOMM channel to the NXT.  The underlying baseband
       connection is automatically opened if needed.  The process
       isn't finished until a listener callback function is registered,
       since synchronous reads are not supported. */
    if ( IOBluetoothDeviceOpenRFCOMMChannel(btDevice,NXT_RFCOMM_CHANNEL_ID,
	&rfcommChannel) != kIOReturnSuccess )
    {
	fprintf(stderr,"IOBluetoothDeviceOpenRFCOMMChannel() failed.\n");
	return 1;
    }
    else
    {
	printf("Successfully opened RFCOMM channel!\n");
    }
    
    /* Check Max Transmission unit, just for kicks. */
    handle = IOBluetoothDeviceGetConnectionHandle(btDevice);
    printf("Handle = %d\n",handle);
    printf("MTU = %d\n",IOBluetoothRFCOMMChannelGetMTU(rfcommChannel));

    /* Send a get-battery-level command and check response */
    if ( IOBluetoothRFCOMMChannelRegisterIncomingDataListener(
	    rfcommChannel,listener,new_data) == 0 )
    {
	/* Get battery level */
	cmd[0] = 0x00;
	cmd[1] = 0x0B;
	*new_data = 0;   /* Make use of the refCon arg by setting a flag. */
	if ( IOBluetoothRFCOMMChannelWrite(rfcommChannel,cmd,2,0) == 0 )
	{
	    printf("Successfully sent command!\n");

	    /* Now read back response.  This will require using the
	       callback function, since there is no synchronous read
	       function. */
	    
	    // Use CFMessagePort?
	    //CFRunLoopAddSource(CFRunLoopGetCurrent(),RFCOMMchannel,kCFRunLoopCommonModes);
	    //See gnokii, osxbluetooth.c
	    //CFRunLoopRun();
	    //bytes = read(handle,buff,100);
	    //printf("%d bytes read back.\n",bytes);
	}
	else
	{
	    fprintf(stderr,"IOBluetoothRFCOMMChannelWrite() failed.\n");
	}
    }
    else
    {
	fprintf(stderr,"IOBluetoothRFCOMMChannelRegisterIncomingDataListener() failed.\n");
    }

    IOBluetoothRFCOMMChannelRegisterIncomingDataListener(
		    rfcommChannel, NULL, NULL);
    if ( IOBluetoothRFCOMMChannelCloseChannel(rfcommChannel) != 0 )
	fprintf(stderr,"IOBluetoothRFCOMMChannelCloseChannel() failed.\n");
    IOBluetoothDeviceCloseConnection(btDevice);
    IOBluetoothObjectRelease(rfcommChannel);
    IOBluetoothObjectRelease(btDevice);
    return 0;
}


void    listener(IOBluetoothRFCOMMChannelRef rfcommChannel, void *data,
		UInt16 length, void *refCon)

{
    int     c,
	    *new_data = refCon;
    
    puts("Got data!");
    for (c=0; c<length; ++c)
	printf("%02X ",((char *)data)[c]);
    putchar('\n');
    *new_data = 1;
    CFRunLoopStop(CFRunLoopGetCurrent());
}

