#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x3B
*******************************************************************************/
#define LIS302DL_WHO_AM_I_ADDR                  0x0F

/*******************************************************************************
*  CTRL_REG1 Register: Control Register 1
*  Read Write register
*  Default value: 0x07
*  7 DR: Data Rate selection.
*        0 - 100 Hz output data rate
*        1 - 400 Hz output data rate
*  6 PD: Power Down control.
*        0 - power down mode
*        1 - active mode
*  5 FS: Full Scale selection.
*        0 - Typical measurement range 2.3
*        1 - Typical measurement range 9.2
*  4:3 STP-STM Self Test Enable:
*              STP |  STM |   mode
*            ----------------------------
*               0  |  0   |   Normal mode
*               0  |  1   |   Self Test M
*               1  |  0   |   Self Test P
*  2 Zen: Z axis enable.
*         0 - Z axis disabled
*         1- Z axis enabled
*  1 Yen: Y axis enable.
*         0 - Y axis disabled
*         1- Y axis enabled
*  0 Xen: X axis enable.
*         0 - X axis disabled
*         1- X axis enabled
********************************************************************************/
#define LIS302DL_CTRL_REG1_ADDR                 0x20

#define LIS302DL_DATARATE_100                             0x00
#define LIS302DL_DATARATE_400                             0x80
  
#define LIS302DL_LOWPOWERMODE_POWERDOWN                   0x00
#define LIS302DL_LOWPOWERMODE_ACTIVE                      0x40
  
#define LIS302DL_FULLSCALE_2_3                            0x00
#define LIS302DL_FULLSCALE_9_2                            0x20

#define LIS302DL_SELFTEST_NORMAL                          0x00
#define LIS302DL_SELFTEST_P                               0x10
#define LIS302DL_SELFTEST_M                               0x08

#define LIS302DL_X_ENABLE                                 0x01
#define LIS302DL_Y_ENABLE                                 0x02
#define LIS302DL_Z_ENABLE                                 0x04
#define LIS302DL_XYZ_ENABLE                               0x07

#define LIS302DL_SENSITIVITY_2_3G                         18  /* 18 mg/digit*/
#define LIS302DL_SENSITIVITY_9_2G                         72  /* 72 mg/digit*/

/*******************************************************************************
*  CTRL_REG2 Regsiter: Control Register 2
*  Read Write register
*  Default value: 0x00
*  7 SIM: SPI Serial Interface Mode Selection. 
*         0 - 4 wire interface
*         1 - 3 wire interface
*  6 BOOT: Reboot memory content
*          0 - normal mode 
*          1 - reboot memory content
*  5 Reserved
*  4 FDS: Filtered data selection.
*         0 - internal filter bypassed
*         1 - data from internal filter sent to output register
*  3 HP FF_WU2: High pass filter enabled for FreeFall/WakeUp#2.
*               0 - filter bypassed
*               1 - filter enabled
*  2 HP FF_WU1: High pass filter enabled for FreeFall/WakeUp#1.
*               0 - filter bypassed
*               1 - filter enabled
*  1:0 HP coeff2-HP coeff1 High pass filter cut-off frequency (ft) configuration.
*                 ft= ODR[hz]/6*HP coeff
*            HP coeff2 | HP coeff1 |   HP coeff
*            -------------------------------------------
*                 0     |     0     |   8
*                 0     |     1     |   16
*                 1     |     0     |   32
*                 1     |     1     |   64
*            HP coeff |  ft[hz]   |  ft[hz]   |
*                     |ODR 100Hz | ODR 400Hz  |
*            --------------------------------------------
*              00     |    2      |     8     |
*              01     |    1      |     4     |
*              10     |    0.5    |     2     |
*              11     |    0.25   |     1     |
*******************************************************************************/
#define LIS302DL_CTRL_REG2_ADDR              0x21

#define LIS302DL_SERIALINTERFACE_4WIRE                    0x00
#define LIS302DL_SERIALINTERFACE_3WIRE                    0x80

#define LIS302DL_BOOT_NORMALMODE                          0x00
#define LIS302DL_BOOT_REBOOTMEMORY                        0x40

#define LIS302DL_FILTEREDDATASELECTION_BYPASSED           0x00
#define LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER     0x20

#define LIS302DL_HIGHPASSFILTERINTERRUPT_OFF              0x00
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1                0x04
#define LIS302DL_HIGHPASSFILTERINTERRUPT_2                0x08
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1_2              0x0C

#define LIS302DL_HIGHPASSFILTER_LEVEL_0                   0x00
#define LIS302DL_HIGHPASSFILTER_LEVEL_1                   0x01
#define LIS302DL_HIGHPASSFILTER_LEVEL_2                   0x02
#define LIS302DL_HIGHPASSFILTER_LEVEL_3                   0x03

/*******************************************************************************
*  CTRL_REG3 Register: Interrupt Control Register
*  Read Write register
*  Default value: 0x00
*  7 IHL active: Interrupt active high/low.
*                0 - active high
*                1 - active low
*  6 PP_OD: push-pull/open-drain.
*           0 - push-pull
*           1 - open-drain
*  5:3 I2_CFG2 - I2_CFG0 Data signal on INT2 pad control bits
*  2:0 I1_CFG2 - I1_CFG0 Data signal on INT1 pad control bits
*        I1(2)_CFG2  |  I1(2)_CFG1  |  I1(2)_CFG0  | INT1(2) Pad
*        ----------------------------------------------------------
*              0     |      0       |       0      | GND
*              0     |      0       |       1      | FreeFall/WakeUp#1
*              0     |      1       |       0      | FreeFall/WakeUp#2
*              0     |      1       |       1      | FreeFall/WakeUp#1 or FreeFall/WakeUp#2
*              1     |      0       |       0      | Data ready
*              1     |      1       |       1      | Click interrupt
*******************************************************************************/
#define LIS302DL_CTRL_REG3_ADDR              0x22

/*******************************************************************************
*  HP_FILTER_RESET Register: Dummy register. Reading at this address zeroes 
*  instantaneously the content of the internal high pass filter. If the high pass
*  filter is enabled all three axes are instantaneously set to 0g.
*  This allows to overcome the settling time of the high pass filter.
*  Read only register
*  Default value: Dummy
*******************************************************************************/
#define LIS302DL_HP_FILTER_RESET_REG_ADDR     0x23

/*******************************************************************************
*  STATUS_REG Register: Status Register
*  Default value: 0x00
*  7 ZYXOR: X, Y and Z axis data overrun.
*           0: no overrun has occurred
*           1: new data has overwritten the previous one before it was read
*  6 ZOR: Z axis data overrun.
*         0: no overrun has occurred 
*         1: new data for Z-axis has overwritten the previous one before it was read
*  5 yOR: y axis data overrun.
*         0: no overrun has occurred
*         1: new data for y-axis has overwritten the previous one before it was read
*  4 XOR: X axis data overrun.
*         0: no overrun has occurred
*         1: new data for X-axis has overwritten the previous one before it was read
*  3 ZYXDA: X, Y and Z axis new data available
*           0: a new set of data is not yet available
*           1: a new set of data is available
*  2 ZDA: Z axis new data available.
*         0: a new set of data is not yet available
*         1: a new data for Z axis is available
*  1 YDA: Y axis new data available
*         0: a new set of data is not yet available
*         1: a new data for Y axis is available
*  0 XDA: X axis new data available
*         0: a new set of data is not yet available
*         1: a new data for X axis is available
*******************************************************************************/
#define LIS302DL_STATUS_REG_ADDR             0x27

/*******************************************************************************
*  OUT_X Register: X-axis output Data
*  Read only register
*  Default value: output
*  7:0 XD7-XD0: X-axis output Data
*******************************************************************************/
#define LIS302DL_OUT_X_ADDR                  0x29

/*******************************************************************************
*  OUT_Y Register: Y-axis output Data
*  Read only register
*  Default value: output
*  7:0 YD7-YD0: Y-axis output Data
*******************************************************************************/
#define LIS302DL_OUT_Y_ADDR                  0x2B

/*******************************************************************************
*  OUT_Z Register: Z-axis output Data
*  Read only register
*  Default value: output
*  7:0 ZD7-ZD0: Z-axis output Data
*******************************************************************************/
#define LIS302DL_OUT_Z_ADDR                  0x2D

/*******************************************************************************
*  FF_WW_CFG_1 Register: Configuration register for Interrupt 1 source.
*  Read write register
*  Default value: 0x00
*  7 AOI: AND/OR combination of Interrupt events. 
*         0: OR combination of interrupt events
*         1: AND combination of interrupt events 
*  6 LIR: Latch/not latch interrupt request
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 ZHIE: Enable interrupt generation on Z high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  4 ZLIE: Enable interrupt generation on Z low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  3 YHIE: Enable interrupt generation on Y high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  2 YLIE: Enable interrupt generation on Y low event. 
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  1 XHIE: Enable interrupt generation on X high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  0 XLIE: Enable interrupt generation on X low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*******************************************************************************/
#define LIS302DL_FF_WU_CFG1_REG_ADDR         0x30

#define LIS302DL_INTERRUPTREQUEST_NOTLATCHED              0x00
#define LIS302DL_INTERRUPTREQUEST_LATCHED                 0x40

#define LIS302DL_CLICKINTERRUPT_XYZ_DISABLE               0x00
#define LIS302DL_CLICKINTERRUPT_X_ENABLE                  0x01
#define LIS302DL_CLICKINTERRUPT_Y_ENABLE                  0x04
#define LIS302DL_CLICKINTERRUPT_Z_ENABLE                  0x10
#define LIS302DL_CLICKINTERRUPT_XYZ_ENABLE                0x15

#define LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE         0x00
#define LIS302DL_DOUBLECLICKINTERRUPT_X_ENABLE            0x02
#define LIS302DL_DOUBLECLICKINTERRUPT_Y_ENABLE            0x08
#define LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE            0x20
#define LIS302DL_DOUBLECLICKINTERRUPT_XYZ_ENABLE          0x2A

/*******************************************************************************
*  FF_WU_SRC_1 Register: Interrupt 1 source register.
*  Reading at this address clears FF_WU_SRC_1 register and the FF, WU 1 interrupt
*  and allow the refreshment of data in the FF_WU_SRC_1 register if the latched option
*  was chosen.
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 ZH: Z high. 
*        0: no interrupt
*        1: ZH event has occurred 
*  4 ZL: Z low.
*        0: no interrupt
*        1: ZL event has occurred
*  3 YH: Y high.
*        0: no interrupt
*        1: YH event has occurred 
*  2 YL: Y low.
*        0: no interrupt
*        1: YL event has occurred
*  1 YH: X high.
*        0: no interrupt
*        1: XH event has occurred 
*  0 YL: X low.
*        0: no interrupt
*        1: XL event has occurred
*******************************************************************************/
#define LIS302DL_FF_WU_SRC1_REG_ADDR           0x31

/*******************************************************************************
*  FF_WU_THS_1 Register: Threshold register
*  Read Write register
*  Default value: 0x00
*  7 DCRM: Reset mode selection.
*          0 - counter resetted
*          1 - counter decremented
*  6 THS6-THS0: Free-fall/wake-up threshold value.
*******************************************************************************/
#define LIS302DL_FF_WU_THS1_REG_ADDR          0x32

/*******************************************************************************
*  FF_WU_DURATION_1 Register: duration Register
*  Read Write register
*  Default value: 0x00
*  7:0 D7-D0 Duration value. (Duration steps and maximum values depend on the ODR chosen)
 ******************************************************************************/
#define LIS302DL_FF_WU_DURATION1_REG_ADDR     0x33

/*******************************************************************************
*  FF_WW_CFG_2 Register: Configuration register for Interrupt 2 source.
*  Read write register
*  Default value: 0x00
*  7 AOI: AND/OR combination of Interrupt events. 
*         0: OR combination of interrupt events
*         1: AND combination of interrupt events 
*  6 LIR: Latch/not latch interrupt request
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 ZHIE: Enable interrupt generation on Z high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  4 ZLIE: Enable interrupt generation on Z low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  3 YHIE: Enable interrupt generation on Y high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  2 YLIE: Enable interrupt generation on Y low event. 
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  1 XHIE: Enable interrupt generation on X high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  0 XLIE: Enable interrupt generation on X low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*******************************************************************************/
#define LIS302DL_FF_WU_CFG2_REG_ADDR         0x34

/*******************************************************************************
*  FF_WU_SRC_2 Register: Interrupt 2 source register.
*  Reading at this address clears FF_WU_SRC_2 register and the FF, WU 2 interrupt
*  and allow the refreshment of data in the FF_WU_SRC_2 register if the latched option
*  was chosen.
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 ZH: Z high. 
*        0: no interrupt
*        1: ZH event has occurred 
*  4 ZL: Z low.
*        0: no interrupt
*        1: ZL event has occurred
*  3 YH: Y high.
*        0: no interrupt
*        1: YH event has occurred 
*  2 YL: Y low.
*        0: no interrupt
*        1: YL event has occurred
*  1 YH: X high.
*        0: no interrupt
*        1: XH event has occurred 
*  0 YL: X low.
*        0: no interrupt
*        1: XL event has occurred
*******************************************************************************/
#define LIS302DL_FF_WU_SRC2_REG_ADDR           0x35

/*******************************************************************************
*  FF_WU_THS_2 Register: Threshold register
*  Read Write register
*  Default value: 0x00
*  7 DCRM: Reset mode selection.
*          0 - counter resetted
*          1 - counter decremented
*  6 THS6-THS0: Free-fall/wake-up threshold value.
*******************************************************************************/
#define LIS302DL_FF_WU_THS2_REG_ADDR          0x36

/*******************************************************************************
*  FF_WU_DURATION_2 Register: duration Register
*  Read Write register
*  Default value: 0x00
*  7:0 D7-D0 Duration value. (Duration steps and maximum values depend on the ODR chosen)
 ******************************************************************************/
#define LIS302DL_FF_WU_DURATION2_REG_ADDR     0x37

/******************************************************************************
*  CLICK_CFG Register: click Register
*  Read Write register
*  Default value: 0x00
*  7 Reserved
*  6 LIR: Latch Interrupt request.
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 Double_Z: Enable interrupt generation on double click event on Z axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  4 Single_Z: Enable interrupt generation on single click event on Z axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  3 Double_Y: Enable interrupt generation on double click event on Y axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  2 Single_Y: Enable interrupt generation on single click event on Y axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  1 Double_X: Enable interrupt generation on double click event on X axis.
*              0: disable interrupt request
*              1: enable interrupt request
*  0 Single_y: Enable interrupt generation on single click event on X axis.
*              0: disable interrupt request
*              1: enable interrupt request
 ******************************************************************************/
#define LIS302DL_CLICK_CFG_REG_ADDR     0x38

/******************************************************************************
*  CLICK_SRC Register: click status Register
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 Double_Z: Double click on Z axis event.
*        0: no interrupt
*        1: Double Z event has occurred 
*  4 Single_Z: Z low.
*        0: no interrupt
*        1: Single Z event has occurred 
*  3 Double_Y: Y high.
*        0: no interrupt
*        1: Double Y event has occurred 
*  2 Single_Y: Y low.
*        0: no interrupt
*        1: Single Y event has occurred 
*  1 Double_X: X high.
*        0: no interrupt
*        1: Double X event has occurred 
*  0 Single_X: X low.
*        0: no interrupt
*        1: Single X event has occurred 
*******************************************************************************/
#define LIS302DL_CLICK_SRC_REG_ADDR        0x39

/*******************************************************************************
*  CLICK_THSY_X Register: Click threshold Y and X register
*  Read Write register
*  Default value: 0x00
*  7:4 THSy3-THSy0: Click threshold on Y axis, step 0.5g
*  3:0 THSx3-THSx0: Click threshold on X axis, step 0.5g
*******************************************************************************/
#define LIS302DL_CLICK_THSY_X_REG_ADDR        0x3B

/*******************************************************************************
*  CLICK_THSZ Register: Click threshold Z register
*  Read Write register
*  Default value: 0x00
*  7:4 Reserved
*  3:0 THSz3-THSz0: Click threshold on Z axis, step 0.5g
*******************************************************************************/
#define LIS302DL_CLICK_THSZ_REG_ADDR         0x3C

/*******************************************************************************
*  CLICK_TimeLimit Register: Time Limit register
*  Read Write register
*  Default value: 0x00
*  7:0 Dur7-Dur0: Time Limit value, step 0.5g
*******************************************************************************/
#define LIS302DL_CLICK_TIMELIMIT_REG_ADDR        0x3D

/*******************************************************************************
*  CLICK_Latency Register: Latency register
*  Read Write register
*  Default value: 0x00
*  7:0 Lat7-Lat0: Latency value, step 1msec
*******************************************************************************/
#define LIS302DL_CLICK_LATENCY_REG_ADDR        0x3E

/*******************************************************************************
*  CLICK_Window Register: Window register
*  Read Write register
*  Default value: 0x00
*  7:0 Win7-Win0: Window value, step 1msec
*******************************************************************************/
#define LIS302DL_CLICK_WINDOW_REG_ADDR        0x3F

void InitializeAccelerometer();
bool PingAccelerometer();
void SetAccelerometerMainConfig(uint8_t config);
void SetAccelerometerFilterConfig(uint8_t config);
void SetAccelerometerInterruptConfig(uint8_t config);
void ResetAccelerometer();
void ResetAccelerometerFilter();
void ReadRawAccelerometerData(int8_t *values);

#endif

