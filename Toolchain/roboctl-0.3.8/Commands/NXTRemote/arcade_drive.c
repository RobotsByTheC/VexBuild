#include <math.h>

#ifndef ABS
#define ABS(x)  ((x) >= 0 ? (x) : -(x))
#endif


void    swap(int *a,int *b)

{
    int     temp = *a;
    
    *a = *b;
    *b = temp;
}


/****************************************************************************
 * Description: 
 *  Convert joystick x and y coordinates (-max_joy to max_joy) to left and
 *  right motor speed (-max_power to max_power) using skid 
 *  (differential) steering.
 *  It's a little tricky to control both motors with one joystick.
 *  Below is a map of the joystick Cartesian plane, showing the desired
 *  left and right motor speeds (l,r) at each point (x,y).  This map was
 *  designed by first defining vehicle behavior at x,y = 0,0 and at the
 *  extremes, where x or y is -max_joy or max_joy.  Points between are
 *  linearly interpolated.
 *
 *  For example, assuming max_joy = 100:
 *      x,y = 0,0:      Vehicle is motionless.
 *      x,y = 0,100:    Full speed straight ahead.
 *      x,y = 100,0:    Full speed turning clockwise about center.
 *                      (Left motor full speed ahead, right full speed reverse)
 *      x,y = 100,100:  Full speed turning about right wheel/track.
 *                      (Left motor full speed, right motor stopped.)
 *      all x>0:        Vehicle is turning clockwise.
 *      all x<0:        Vehicle is turning counter-clockwise.
 *  
 *      0,100       50,100      100,100     100,50      100,0
 *
 *      -50,100     0,75        50,50       75,0        100,-50
 *
 *      -100,100    -50,50      0,0         50,-50      100,-100
 *
 *      -100,50     -75,0       -50,-50     0,-75       50,-100
 *
 *      -100,0      -100,-50    -100,-100   -50,-100    0,-100
 *  
 *  The formula for any single quandrant is simple.  A general formula for
 *  the entire plane would be ugly.  ( Observe how l varies across
 *  the top of the plane (where y=100), increasing from 0 to 100 over x from
 *  -100 to 0, and then becoming a constant 100 for x from 0 to 100. )
 *  
 *  The values are highly symmetric between various quandrants.  l and r
 *  are swapped going from any x to -x, and swapped and negated
 *  for any y and -y.  Hence, we simply compute (l,r) for the first quadrant
 *  by using abs(x) and abs(y), and then for:
 *  2nd quadrant: swap x and y
 *  3rd quadrant: negate x and y
 *  4th quadrant: swap and negate x and y
 *
 *  The warp_factor argument provides for non-linear mapping of
 *  joystick position to motor power.  Linear mappings (warp_factor=1.0)
 *  often don't work well.  For example, with Lego NXT systems, the vehicle
 *  may not move at all until motor power reaches 60 or higher.  This
 *  means that 60% of the joystick range does nothing, and the outer
 *  40% is mapped to available vehicle speeds.  This has the effect
 *  of making control very touchy, since small joystick movements
 *  result in noticeable power changes.  Mapping motor power
 *  to the Nth root of joystick position increases the slope of the
 *  power function close to zero, so that the threshold power is reached
 *  closer to joystick center position.  This spreads the usable motor
 *  power settings across a wider range of joystick positions, making
 *  the vehicle easier to control.
 *
 *  The higher the power, the higher the initial slope, and the more
 *  square the curve.  Using something around 4th root
 *  (warp_factor = 4.0f) has proven to work well for Lego NXT control.
 *  
 *
 *  Linear              Square root         4th root
 *
 *  p |       *         |       *           |       *
 *  o |     *           |   *               | *
 *  w |   *             | *                 |*
 *  e | *               |*                  |*
 *  r +---------        +----------         +----------
 *     joystick
 *
 ***************************************************************************/

void    arcade_drive(int x, int y, int max_joy, int max_power,
		    float warp_factor,
		    int *left_speed_ptr, int *right_speed_ptr)

{
    int     left_speed,
	    right_speed,
	    abs_x = ABS(x),
	    abs_y = ABS(y);
    float   warp_expo,
	    warp_denom;

    warp_expo = 1.0 / warp_factor;
    warp_denom = powf((float)max_power, warp_expo);

    /*
     * Compute left and right motor speeds using simple formula for
     * quadrant 1.
     */
    left_speed = abs_y + abs_x * ( max_joy - abs_y ) / max_joy;
    left_speed = left_speed * max_power / max_joy;
    right_speed = (abs_y - abs_x);
    right_speed = right_speed * max_power / max_joy;
    
    /* Warp by taking the nth root of the motor power, and normalize back
       to a range of 0 to 100 by dividing by 100^(1/n) */
    left_speed = max_power * powf((float)left_speed,warp_expo) / warp_denom;
    if ( right_speed >= 0 )
	right_speed = max_power * powf((float)right_speed,warp_expo) / warp_denom;
    else
	right_speed = -max_power * powf((float)-right_speed,warp_expo) / warp_denom;
    
    if ( y >= 0 )   /* Quadrants 1 and 2 */
    {
	/* 
	 *  Left speed and right speed are simply swapped when going from
	 *  x,y to -x,y.  ( Quandrant 1 to 2 )
	 */
	if ( x < 0 )    /* Swap for quadrant 2 */
	    swap(&left_speed,&right_speed);
    }
    else    /* Quadrants 3 and 4 */
    {
	/*
	 *  Left speed and right speed are swapped AND negated when going
	 *  from x,y to x,-y.  ( Quadrant 1 to 4 )
	 *
	 *  Going from x,y to -x,-y (quadrant 1 to 3), motor speeds are
	 *  only negated.
	 */
	if ( x >= 0 )   /* Swap for quadrant 4 only */
	    swap(&left_speed,&right_speed);

	/* Negate for quadrants 3 and 4 */
	left_speed = -left_speed;
	right_speed = -right_speed;
    }
    *left_speed_ptr = left_speed;
    *right_speed_ptr = right_speed;
}

