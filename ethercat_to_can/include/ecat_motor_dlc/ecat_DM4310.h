#ifndef __ECAT_DM4310__
#define __ECAT_DM4310__

#include "ecat_can_base/ecat_base.h"
#include "ecat_can_base/ecat_typedef.h"

#include <inttypes.h>

//--------//
// DM4310 //
//--------//
#define DM4310_ID_1 1
#define DM4310_ID_2 2
#define DM4310_ID_3 3
#define DM4310_ID_4 4
#define DM4310_ID_5 5
#define DM4310_ID_6 6
#define DM4310_ID_7 7
#define DM4310_ID_8 8

#define SPEED_CTRL 0
#define POS_CTRL 1
#define MIT_CTRL 2
#define ENABLE 3
#define DISABLE 4
#define CLEAR_ERROR 5

//===================

#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 50.0f
#define POS_MIN -12.5f
#define POS_MAX 12.5f
#define SPD_MIN -18.0f
#define SPD_MAX 18.0f
#define T_MIN -30.0f
#define T_MAX 30.0f
#define I_MIN -30.0f
#define I_MAX 30.0f

#define DM_P_MIN  -12.5f // Radians
#define DM_P_MAX  12.5f
#define DM_V_MIN  -45.0f // Rad/s
#define DM_V_MAX  45.0f
#define DM_KP_MIN 0.0f  // N-m/rad
#define DM_KP_MAX 500.0f
#define DM_KD_MIN 0.0f // N-m/rad/s
#define DM_KD_MAX 5.0f
#define DM_T_MIN  -18.0f
#define DM_T_MAX  18.0f

//===================

float uint_to_float(int x_int, float x_min, float x_max, int bits);
int float_to_uint(float x, float x_min, float x_max, int bits);
void DM_can_set(uint8_t canid, ecat::Ecat_Outputs_Pack *pack, int mode, uint8_t id,float _pos, float _vel, float _KP, float _KD, float _torq);


#endif