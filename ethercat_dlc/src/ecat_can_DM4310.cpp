#include "ethercat_dlc/ecat_can_DM4310.hpp"

namespace ecat
{
    DM4310dlc::DM4310dlc(uint8_t _can_id, uint32_t _motor_id)
    {
        this->can_id = _can_id;
        this->motor_id = _motor_id;
    }

    float DM4310dlc::uint_to_float(int x_int, float x_min, float x_max, int bits)
    {
        float span = x_max - x_min;
        float offset = x_min;
        return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
    }
    int DM4310dlc::float_to_uint(float x, float x_min, float x_max, int bits)
    {
        float span = x_max - x_min;
        float offset = x_min;
        return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
    }

    void DM4310dlc::DM_can_set(ecat::Ecat_Outputs_Pack *pack, int mode, float _pos, float _vel, float _KP, float _KD, float _torq)
    {
        if (_KP > KP_MAX)
            _KP = KP_MAX;
        else if (_KP < KP_MIN)
            _KP = KP_MIN;
        if (_KD > KD_MAX)
            _KD = KD_MAX;
        else if (_KD < KD_MIN)
            _KD = KD_MIN;
        if (_pos > POS_MAX)
            _pos = POS_MAX;
        else if (_pos < POS_MIN)
            _pos = POS_MIN;
        if (_vel > SPD_MAX)
            _vel = SPD_MAX;
        else if (_vel < SPD_MIN)
            _vel = SPD_MIN;
        if (_torq > T_MAX)
            _torq = T_MAX;
        else if (_torq < T_MIN)
            _torq = T_MIN;

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
            pack->can[this->can_id].StdId = 0x0200 + motor_id;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x04;

            pack->can[this->can_id].Data[0] = *vbuf;
            pack->can[this->can_id].Data[1] = *(vbuf + 1);
            pack->can[this->can_id].Data[2] = *(vbuf + 2);
            pack->can[this->can_id].Data[3] = *(vbuf + 3);
        }
        // POS CTRL
        else if (mode == 1)
        {
            pack->can[this->can_id].StdId = 0x0100 + motor_id;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = *pbuf;
            pack->can[this->can_id].Data[1] = *(pbuf + 1);
            pack->can[this->can_id].Data[2] = *(pbuf + 2);
            pack->can[this->can_id].Data[3] = *(pbuf + 3);
            pack->can[this->can_id].Data[4] = *vbuf;
            pack->can[this->can_id].Data[5] = *(vbuf + 1);
            pack->can[this->can_id].Data[6] = *(vbuf + 2);
            pack->can[this->can_id].Data[7] = *(vbuf + 3);
        }
        // MIT CTRL
        else if (mode == 2)
        {
            pack->can[this->can_id].StdId = 0x0000 + motor_id;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = (pos_tmp >> 8);
            pack->can[this->can_id].Data[1] = pos_tmp;
            pack->can[this->can_id].Data[2] = (vel_tmp >> 4);
            pack->can[this->can_id].Data[3] = ((vel_tmp & 0xF) << 4) | (kp_tmp >> 8);
            pack->can[this->can_id].Data[4] = kp_tmp;
            pack->can[this->can_id].Data[5] = (kd_tmp >> 4);
            pack->can[this->can_id].Data[6] = ((kd_tmp & 0xF) << 4) | (tor_tmp >> 8);
            pack->can[this->can_id].Data[7] = tor_tmp;
        }
        // ENABLE
        else if (mode == 3)
        {
            pack->can[this->can_id].StdId = motor_id;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = 0xFF;
            pack->can[this->can_id].Data[1] = 0xFF;
            pack->can[this->can_id].Data[2] = 0xFF;
            pack->can[this->can_id].Data[3] = 0xFF;
            pack->can[this->can_id].Data[4] = 0xFF;
            pack->can[this->can_id].Data[5] = 0xFF;
            pack->can[this->can_id].Data[6] = 0xFF;
            pack->can[this->can_id].Data[7] = 0xFC;
        }
        // DISABLE
        else if (mode == 4)
        {
            pack->can[this->can_id].StdId = motor_id;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = 0xFF;
            pack->can[this->can_id].Data[1] = 0xFF;
            pack->can[this->can_id].Data[2] = 0xFF;
            pack->can[this->can_id].Data[3] = 0xFF;
            pack->can[this->can_id].Data[4] = 0xFF;
            pack->can[this->can_id].Data[5] = 0xFF;
            pack->can[this->can_id].Data[6] = 0xFF;
            pack->can[this->can_id].Data[7] = 0xFD;
        }
        // CLEAR_ERROR
        else if (mode == 5)
        {
            pack->can[this->can_id].StdId = motor_id;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = 0xFF;
            pack->can[this->can_id].Data[1] = 0xFF;
            pack->can[this->can_id].Data[2] = 0xFF;
            pack->can[this->can_id].Data[3] = 0xFF;
            pack->can[this->can_id].Data[4] = 0xFF;
            pack->can[this->can_id].Data[5] = 0xFF;
            pack->can[this->can_id].Data[6] = 0xFF;
            pack->can[this->can_id].Data[7] = 0xFB;
        }
    }

    void DM4310dlc::DM_can_analyze(Ecat_Inputs_Pack *pack)
    {
        if (pack->can[this->can_id].StdId == 0)
        {
            if (pack->can[this->can_id].Data[0] == motor_id)
            {
                this->pos = (float)uint_to_float((uint16_t)pack->can[this->can_id].Data[1] << 8 | (uint16_t)pack->can[this->can_id].Data[2], POS_MIN, POS_MAX, 16);
                this->vec = (float)uint_to_float(((uint16_t)pack->can[this->can_id].Data[3] << 4) | ((uint16_t)pack->can[this->can_id].Data[4] >> 4), DM_V_MIN, DM_V_MAX, 12);
                this->toq = (float)uint_to_float((((uint16_t)pack->can[this->can_id].Data[4] & 0x0F) << 8) | ((uint16_t)pack->can[this->can_id].Data[5]), DM_T_MIN, DM_T_MAX, 12);
            }
        }
    }
}