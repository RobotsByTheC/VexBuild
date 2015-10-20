#ifndef USERAPI_H_
#define USERAPI_H_

#include <stdio.h>

void IO_Initialization(void);
void SetDigitalOutput(unsigned char port, unsigned char value);
unsigned char GetDigitalInput(unsigned char port);
void SetDirection(unsigned char port, unsigned char direction);
#ifdef _FRC_BOARD
void DefineControllerIO(unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4,
                        unsigned char p5, unsigned char p6, unsigned char p7, unsigned char p8,
                        unsigned char p9, unsigned char p10, unsigned char p11, unsigned char p12,
                        unsigned char p13, unsigned char p14, unsigned char p15, unsigned char p16,
                        unsigned char p17, unsigned char p18);
#endif // _FRC_BOARD
#ifdef _VEX_BOARD
void DefineControllerIO(unsigned char numberOfAnalogChannels,
                        unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4,
                        unsigned char p5, unsigned char p6, unsigned char p7, unsigned char p8,
                        unsigned char p9, unsigned char p10, unsigned char p11, unsigned char p12,
                        unsigned char p13, unsigned char p14, unsigned char p15, unsigned char p16);
void SetupWhoControlsPwms(int pwmSpec1,int pwmSpec2,int pwmSpec3,int pwmSpec4);
#endif // _VEX_BOARD
unsigned int GetAnalogInput(unsigned char port);

void SetPWM(unsigned char port, int speed);

#ifdef _VEX_BOARD
unsigned char GetPWM(unsigned char port);
#endif

void PrintToScreen(rom const char *fmt, ...);

unsigned char IsAutonomous(void);
unsigned char IsEnabled(void);
#ifdef _VEX_BOARD
void SetCompetitionMode(unsigned char mode, unsigned char operatorTime);
#endif // _VEX_BOARD
#ifdef _FRC_BOARD
void SetCompetitionMode(unsigned char mode);
#endif // _FRC_BOARD

void StartEncoder(unsigned char channel);
void StopEncoder(unsigned char channel);
long GetEncoder(unsigned char channel);
void PresetEncoder(unsigned char channel, long presetValue);

void StartQuadEncoder(unsigned char channelA, unsigned char channelB, unsigned char invert);
void StopQuadEncoder(unsigned char channelA, unsigned char channelB);
long GetQuadEncoder(unsigned char channelA, unsigned char channelB);
void PresetQuadEncoder(unsigned char channelA, unsigned char channelB, long presetValue);

void StartGTSensor(unsigned char port, unsigned char invert);
void StopGTSensor(unsigned char port);
long GetGTSensor(unsigned char port);
void PresetGTSensor(unsigned char port, long presetValue);

void StartInterruptWatcher(unsigned char port, unsigned char direction);
void StopInterruptWatcher(unsigned char port);
unsigned char GetInterruptWatcher(unsigned char port);

unsigned int GetUltrasonic(unsigned char echo, unsigned char ping);
void StartUltrasonic(unsigned char echo, unsigned char ping);
void StopUltrasonic(unsigned char echo, unsigned char ping);

void InitGyro(unsigned char port);
void StartGyro(unsigned char port);
void StopGyro(unsigned char port);
int GetGyroAngle(unsigned char port);
void SetGyroType(unsigned char port, unsigned type);
void SetGyroDeadband(unsigned char port, char deadband);

void InitAccelerometer(unsigned char port);
void StartAccelerometer(unsigned char port);
int GetAcceleration(unsigned char port);
int GetVelocity(unsigned char port);
int GetDistance(unsigned char port);
void StopAccelerometer(unsigned char port);

void Wait(unsigned long ms);
void StartTimer(unsigned char timerNumber);
void StopTimer(unsigned char timerNumber);
void PresetTimer(unsigned char timerNumber, unsigned long value);
unsigned long GetTimer(unsigned char timerNumber);

unsigned GetSecondClock(void);
unsigned long GetMsClock(void);
unsigned GetGameTime(void);

unsigned char GetOIDInput(unsigned char port, unsigned char channel);
unsigned char GetOIAInput(unsigned char port, unsigned char channel);
void OIToDOutput(unsigned char port, unsigned char function, unsigned char dport);
void OIToPWM(unsigned char port, unsigned char function, unsigned char pwm, unsigned char invert);
unsigned char GetRxInput(unsigned char port, unsigned char channel);

#ifdef _FRC_BOARD
void SetOILED(unsigned char led, unsigned char value);
void SetUserDisplay(unsigned char value);
void SetRelay(unsigned char port, char forward, char reverse);
void OIToRelay(unsigned char port, unsigned char function,
                unsigned char relayNumber, unsigned char direction);
void SetUserByte(unsigned char index, unsigned char value);
#endif // _FRC_BOARD

#ifdef _FRC_BOARD
void Arcade2(unsigned char movePort, unsigned char moveChannel,
                unsigned char rotatePort, unsigned char rotateChannel,
                unsigned char leftPWM, unsigned char rightPWM,
                unsigned char leftInvert, unsigned char rightInvert);
#endif //_FRC_BOARD
#ifdef _VEX_BOARD
void Arcade2(unsigned char port,
                unsigned char moveChannel, unsigned char rotateChannel,
                unsigned char leftPWM, unsigned char rightPWM,
                unsigned char leftInvert, unsigned char rightInvert);
#endif // _VEXBOARD

#ifdef _FRC_BOARD
void Arcade4(unsigned char ucMovePort, unsigned char ucMoveChannel,
            unsigned char ucRotatePort, unsigned char ucRotateChannel,
            unsigned char ucLeftfrontPWM, unsigned char ucRightfrontPWM,
            unsigned char ucLeftrearPWM, unsigned char ucRightrearPWM,
            unsigned char ucLeftfrontInvert, unsigned char ucRightfrontInvert,
            unsigned char ucLeftrearInvert, unsigned char ucRightrearInvert);
#endif // _FRC_BOARD
#ifdef _VEX_BOARD
void Arcade4(unsigned char port,
            unsigned char ucRotateChannel, unsigned char ucMoveChannel,
            unsigned char ucLeftfrontPWM, unsigned char ucRightfrontPWM,
            unsigned char ucLeftrearPWM, unsigned char ucRightrearPWM,
            unsigned char ucLeftfrontInvert, unsigned char ucRightfrontInvert,
            unsigned char ucLeftrearInvert, unsigned char ucRightrearInvert);
#endif // _VEX_BOARD

#ifdef _FRC_BOARD
void Tank2(unsigned char leftPort, unsigned char leftChannel,
            unsigned char rightPort, unsigned char rightChannel,
            unsigned char leftPWM, unsigned char rightPWM,
            unsigned char leftInvert, unsigned char rightInvert);
#endif // _FRC_BOARD
#ifdef _VEX_BOARD
void Tank2(unsigned char port,
            unsigned char leftChannel, unsigned char rightChannel,
            unsigned char leftPWM, unsigned char rightPWM,
            unsigned char leftInvert, unsigned char rightInvert);
#endif // _VEX_BOARD

#ifdef _FRC_BOARD
void Tank4(unsigned char ucLeftPort, unsigned char ucLeftChannel,
            unsigned char ucRightPort, unsigned char ucRightChannel,
            unsigned char ucLeftfrontPWM, unsigned char ucRightfrontPWM,
            unsigned char ucLeftrearPWM, unsigned char ucRightrearPWM,
            unsigned char ucLeftfrontInvert, unsigned char ucRightfrontInvert,
            unsigned char ucLeftrearInvert, unsigned char ucRightrearInvert);
#endif // _FRC_BOARD
#ifdef _VEX_BOARD
void Tank4(unsigned char port,
            unsigned char ucLeftChannel, unsigned char ucRightChannel,
            unsigned char ucLeftfrontPWM, unsigned char ucRightfrontPWM,
            unsigned char ucLeftrearPWM, unsigned char ucRightrearPWM,
            unsigned char ucLeftfrontInvert, unsigned char ucRightfrontInvert,
            unsigned char ucLeftrearInvert, unsigned char ucRightrearInvert);
#endif // _VEX_BOARD

typedef struct {
    unsigned char mx;
    unsigned char my;
    unsigned char x1;
    unsigned char y1;
    unsigned char x2;
    unsigned char y2;
    unsigned char regionSize;
    unsigned char confidence;
    unsigned char pan;
    unsigned char tilt;
    unsigned char length;
    unsigned char sequence;
} TPacket;

typedef rom struct {
    unsigned char redMin, redMax, greenMin, greenMax, blueMin, blueMax;
    unsigned char yCrCb;
    unsigned char noiseFilter;
    unsigned char aecEnable;
    unsigned char aecLevel;
    unsigned char agcEnable;
    unsigned char agcLevel;
    unsigned char saturation;
    unsigned char blueGain;
    unsigned char redGain;
    unsigned char brightness;
    unsigned char panRangeFar;
    unsigned char panRangeNear;
    unsigned char panStep;
    unsigned char tiltRangeFar;
    unsigned char tiltRangeNear;
    unsigned char tiltStep;
} CameraInitializationData;

void InitCamera(unsigned char cameraInitIndex);
void CaptureTrackingData(
            unsigned char *centerX,
            unsigned char *centerY,
            unsigned char *x1,
            unsigned char *y1,
            unsigned char *x2,
            unsigned char *y2,
            unsigned char *regionSize,
            unsigned char *confidence,
            unsigned char *pan,
            unsigned char *tilt);
void StopCamera(void);
void StartCamera(void);
TPacket *CopyTrackingData(void);
void SetServoTracking(unsigned char panTracking, unsigned char tiltTracking);
void SetServoPosition(unsigned char servo, unsigned char position);
void InitializeCamera(CameraInitializationData *c);
unsigned char GetCameraStatus(void);
void SetCameraDebugMode(unsigned char mode);

void SetGDWaitTime(unsigned time);
void ResetGD(void);
void ClearGD(unsigned char ucRow1, unsigned char ucCol1, unsigned char ucRow2, unsigned char ucCol2, unsigned char ucFrame);
void PrintTextToGD(unsigned char ucRow, unsigned char ucCol, unsigned long ulColor, rom const char *szText, ...);
void PrintFrameToGD(unsigned char ucRow1, unsigned char ucCol1, unsigned char ucRow2, unsigned char ucCol2, unsigned long ulColor);

#ifdef _VEX_BOARD
void InitLCD(void);
unsigned char SetLCDText(unsigned char nLine, rom const char *szMsg, ...);
unsigned char SetLCDLight(unsigned char nLight);
void StartLCDButtonsWatcher(void);
void StopLCDButtonsWatcher(void);
void GetLCDButtonsWatcher(unsigned char *b1, unsigned char *b2, unsigned char *b3);
#endif // _VEX_BOARD

#ifdef _FRC_BOARD
void InitPressureSwitch(unsigned char pressureSwitchPort, unsigned char relayPort);
#endif //_FRC_BOARD


#ifdef _FRC_BOARD
#define PWM1_GREEN 0
#define PWM1_RED 1
#define PWM2_GREEN 2
#define PWM2_RED 3
#define RELAY1_GREEN 4
#define RELAY1_RED 5
#define RELAY2_GREEN 6
#define RELAY2_RED 7
#define SWITCH1_LED 8
#define SWITCH2_LED 9
#define SWITCH3_LED 10

#define PORT_1 1
#define PORT_2 2
#define PORT_3 3
#define PORT_4 4
#define X_AXIS 1
#define Y_AXIS 2
#define WHEEL_AXIS 3
#define AUX_AXIS 4
#define TRIGGER_SW 1
#define TOP_SW 2
#define AUX1_SW 3
#define AUX2_SW 4
#define ALT_TRIGGER 5
#define ALT_THUMB 6
#define ALT_AUX1_SW 7
#define ALT_AUX2_SW 8

#endif // _FRC_BOARD

#ifdef _VEX_BOARD

#define PORT_1 1
#define PORT_2 2
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3
#define CHANNEL_4 4
#define CHANNEL_5 5
#define CHANNEL_6 6
#define CHANNEL_5_TOP 1
#define CHANNEL_5_BOTTOM 2
#define CHANNEL_6_TOP 3
#define CHANNEL_6_BOTTOM 4

#endif // _VEX_BOARD

#define INPUT   1
#define OUTPUT  0

// which edge will cause the ISR to be  called (int 1 & 2 done in hardware, 3-6 in software)
#define RISING_EDGE 1
#define FALLING_EDGE 0
#define DISABLE_INTERRUPTS INTCONbits.GIEL = 0
#define ENABLE_INTERRUPTS INTCONbits.GIEL = 1

// these are gyro types that will be used for SetGyroType
#define ADXRS300	50
#define ADXRS150	125
#define ADXRS80		125
#define LY3100ALH	16

void Set_Number_of_Analog_Channels(unsigned char numberOfChannels);

void RegisterInterruptHandler(unsigned char port, unsigned char edge, void (*handler)(unsigned char port, unsigned char value));
void UnRegisterInterruptHandler(unsigned char port);
void SetInterruptEdge(unsigned char port, unsigned char edge);


#define BAUD_4800 0x0081
#define BAUD_9600 0x0040
#define BAUD_14400 0x01AD
#define BAUD_19200 0x0181
#define BAUD_28800 0x0156
#define BAUD_38400 0x0140
#define BAUD_57600 0x012A
#define BAUD_115200 0x0115

unsigned char ReadSerialPortOne(void);
void WriteSerialPortOne(unsigned char);
unsigned char ReadSerialPortTwo(void);
void WriteSerialPortTwo(unsigned char);
unsigned char GetSerialPort2ByteCount(void);
unsigned char GetSerialPort1ByteCount(void);
void OpenSerialPortOne(unsigned baudRate);
void OpenSerialPortTwo(unsigned baudRate);

void InitializeCompass(unsigned char port);
unsigned GetCompassHeading(void);

unsigned int Get_Analog_Value (unsigned char ADC_channel);

unsigned GetSecondClock(void);
unsigned long GetUsClock(void);
unsigned long GetMsClock(void);
unsigned GetGameTime(void);

typedef void (*TimerHandler)(void);
void RegisterSingleTimer(unsigned long time, void (*handler)(void));
void RegisterRepeatingTimer(unsigned long time, void (*handler)(void));
void CancelTimer(void (*handler)(void));

unsigned char GetPacketNumber(void);

#ifdef _FRC_BOARD
unsigned GetMainBattery(void);
unsigned GetBackupBattery(void);
#endif //_FRC_BOARD

#ifdef _VEX_BOARD
unsigned char ReceivingData(unsigned char port);
#endif // _VEX_BOARD



#endif // USERAPI_H_
