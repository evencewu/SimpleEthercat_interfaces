#include "ecat_motor_dlc/ecat_DM4310.h"

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}
int float_to_uint(float x, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

// DM4310_demo
void DM_can_set(uint8_t canid, ecat::Ecat_Outputs_Pack *pack, int mode, uint8_t id,float _pos, float _vel, float _KP, float _KD, float _torq)
{
    if (_KP > KP_MAX)_KP = KP_MAX;
    else if (_KP < KP_MIN)_KP = KP_MIN;
    if (_KD > KD_MAX)_KD = KD_MAX;
    else if (_KD < KD_MIN)_KD = KD_MIN;
    if (_pos > POS_MAX)_pos = POS_MAX;
    else if (_pos < POS_MIN) _pos = POS_MIN;
    if (_vel > SPD_MAX)_vel = SPD_MAX;
    else if (_vel < SPD_MIN)_vel = SPD_MIN;
    if (_torq > T_MAX)_torq = T_MAX;
    else if (_torq < T_MIN)_torq = T_MIN;

    uint16_t pos_tmp, vel_tmp, kp_tmp, kd_tmp, tor_tmp;
    pos_tmp = float_to_uint(_pos, DM_P_MIN, DM_P_MAX, 16);
    vel_tmp = float_to_uint(_vel, DM_V_MIN, DM_V_MAX, 12);
    kp_tmp = float_to_uint(_KP, DM_KP_MIN, DM_KP_MAX, 12);
    kd_tmp = float_to_uint(_KD, DM_KD_MIN, DM_KD_MAX, 12);
    tor_tmp = float_to_uint(_torq, DM_T_MIN, DM_T_MAX, 12);

    uint8_t *pbuf, *vbuf;
    pbuf = (uint8_t *)&_pos;
    vbuf = (uint8_t *)&_vel;

    // SPEED CTRL
    if (mode == 0)
    {
        pack->can[canid-1].StdId = 0x0200 + id;
        pack->can[canid-1].ExtId = 0x00;
        pack->can[canid-1].IDE = 0x00;
        pack->can[canid-1].RTR = 0x00;
        pack->can[canid-1].DLC = 0x04;

        pack->can[canid-1].Data[0] = *vbuf;
        pack->can[canid-1].Data[1] = *(vbuf + 1);
        pack->can[canid-1].Data[2] = *(vbuf + 2);
        pack->can[canid-1].Data[3] = *(vbuf + 3);
    }
    // POS CTRL
    else if (mode == 1)
    {
        pack->can[canid-1].StdId = 0x0100 + id;
        pack->can[canid-1].ExtId = 0x00;
        pack->can[canid-1].IDE = 0x00;
        pack->can[canid-1].RTR = 0x00;
        pack->can[canid-1].DLC = 0x08;

        pack->can[canid-1].Data[0] = *pbuf;
        pack->can[canid-1].Data[1] = *(pbuf + 1);
        pack->can[canid-1].Data[2] = *(pbuf + 2);
        pack->can[canid-1].Data[3] = *(pbuf + 3);
        pack->can[canid-1].Data[4] = *vbuf;
        pack->can[canid-1].Data[5] = *(vbuf + 1);
        pack->can[canid-1].Data[6] = *(vbuf + 2);
        pack->can[canid-1].Data[7] = *(vbuf + 3);
    }
    // MIT CTRL
    else if (mode == 2)
    {
        pack->can[canid-1].StdId = 0x0000 + id;
        pack->can[canid-1].ExtId = 0x00;
        pack->can[canid-1].IDE = 0x00;
        pack->can[canid-1].RTR = 0x00;
        pack->can[canid-1].DLC = 0x08;

        pack->can[canid-1].Data[0] = (pos_tmp >> 8);
        pack->can[canid-1].Data[1] = pos_tmp;
        pack->can[canid-1].Data[2] = (vel_tmp >> 4);
        pack->can[canid-1].Data[3] = ((vel_tmp & 0xF) << 4) | (kp_tmp >> 8);
        pack->can[canid-1].Data[4] = kp_tmp;
        pack->can[canid-1].Data[5] = (kd_tmp >> 4);
        pack->can[canid-1].Data[6] = ((kd_tmp & 0xF) << 4) | (tor_tmp >> 8);
        pack->can[canid-1].Data[7] = tor_tmp;
    }
    // ENABLE
    else if (mode == 3)
    {
        pack->can[canid-1].StdId = 0x00000001;
        pack->can[canid-1].ExtId = 0x00;
        pack->can[canid-1].IDE = 0x00;
        pack->can[canid-1].RTR = 0x00;
        pack->can[canid-1].DLC = 0x08;

        pack->can[canid-1].Data[0] = 0xFF;
        pack->can[canid-1].Data[1] = 0xFF;
        pack->can[canid-1].Data[2] = 0xFF;
        pack->can[canid-1].Data[3] = 0xFF;
        pack->can[canid-1].Data[4] = 0xFF;
        pack->can[canid-1].Data[5] = 0xFF;
        pack->can[canid-1].Data[6] = 0xFF;
        pack->can[canid-1].Data[7] = 0xFC;
    }
    // DISABLE
    else if (mode == 4)
    {
        pack->can[canid-1].StdId = 0x00000001;
        pack->can[canid-1].ExtId = 0x00;
        pack->can[canid-1].IDE = 0x00;
        pack->can[canid-1].RTR = 0x00;
        pack->can[canid-1].DLC = 0x08;

        pack->can[canid-1].Data[0] = 0xFF;
        pack->can[canid-1].Data[1] = 0xFF;
        pack->can[canid-1].Data[2] = 0xFF;
        pack->can[canid-1].Data[3] = 0xFF;
        pack->can[canid-1].Data[4] = 0xFF;
        pack->can[canid-1].Data[5] = 0xFF;
        pack->can[canid-1].Data[6] = 0xFF;
        pack->can[canid-1].Data[7] = 0xFD;
    }
    // CLEAR_ERROR
    else if (mode == 5)
    {
        pack->can[canid-1].StdId = 0x00000001;
        pack->can[canid-1].ExtId = 0x00;
        pack->can[canid-1].IDE = 0x00;
        pack->can[canid-1].RTR = 0x00;
        pack->can[canid-1].DLC = 0x08;

        pack->can[canid-1].Data[0] = 0xFF;
        pack->can[canid-1].Data[1] = 0xFF;
        pack->can[canid-1].Data[2] = 0xFF;
        pack->can[canid-1].Data[3] = 0xFF;
        pack->can[canid-1].Data[4] = 0xFF;
        pack->can[canid-1].Data[5] = 0xFF;
        pack->can[canid-1].Data[6] = 0xFF;
        pack->can[canid-1].Data[7] = 0xFB;
    }
}