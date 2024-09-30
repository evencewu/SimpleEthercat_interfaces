#include "ethercat_dlc/ecat_can_MW6010.hpp"

namespace ecat
{
    MW6010dlc::MW6010dlc(uint8_t _can_id, uint32_t _motor_id)
    {

    }

    void MW6010dlc::MW6010_can_set(ecat::Ecat_Outputs_Pack *pack, int mode,float _pos, float _vel, float _KP, float _KD, float _torq)
    {
        uint16_t pos_tmp = (uint16_t)((_pos + 12.5)*65535 /25);
        uint16_t vel_tmp = (uint16_t)((_vel + 65) * 4095 / 130);
        uint16_t kp_tmp = (uint16_t)(_KP * 4095 / 500);
        uint16_t kd_tmp = (uint16_t)(_KD * 4095 / 5);
        uint16_t tor_tmp = (uint16_t)((_torq + 50) * 4095 / 100);

        // MIT CTRL
        if(mode == MIT_CTRL)
        {
            pack->can[this->can_id].StdId = ((uint8_t)motor_id<<5)+0x08;
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

        if(mode == ENABLE)
        {
            pack->can[this->can_id].StdId = ((uint8_t)motor_id<<5)+0x007;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = 0x00;
            pack->can[this->can_id].Data[1] = 0x00;
            pack->can[this->can_id].Data[2] = 0x00;
            pack->can[this->can_id].Data[3] = 0x00;
            pack->can[this->can_id].Data[4] = 0x00;
            pack->can[this->can_id].Data[5] = 0x00;
            pack->can[this->can_id].Data[6] = 0x00;
            pack->can[this->can_id].Data[7] = 0x08;
        }

        if(mode == SELECT_MODE)
        {
            pack->can[this->can_id].StdId = ((uint8_t)motor_id<<5)+0x00B;
            pack->can[this->can_id].ExtId = 0x00;
            pack->can[this->can_id].IDE = 0x00;
            pack->can[this->can_id].RTR = 0x00;
            pack->can[this->can_id].DLC = 0x08;

            pack->can[this->can_id].Data[0] = 0x00;
            pack->can[this->can_id].Data[1] = 0x00;
            pack->can[this->can_id].Data[2] = 0x00;
            pack->can[this->can_id].Data[3] = 0x03;
            pack->can[this->can_id].Data[4] = 0x00;
            pack->can[this->can_id].Data[5] = 0x00;
            pack->can[this->can_id].Data[6] = 0x00;
            pack->can[this->can_id].Data[7] = 0x09;
        }

        
    }

    void MW6010dlc::MW6010_can_analyze(Ecat_Inputs_Pack *pack)
    {
        if (pack->can[this->can_id].StdId == ((uint8_t)motor_id<<5)+0x08)
        {
            if (pack->can[this->can_id].Data[0] == motor_id)
            {
                uint16_t pos_int = (uint16_t)pack->can[this->can_id].Data[1] << 8 | (uint16_t)pack->can[this->can_id].Data[2];
                uint16_t vel_int = (uint16_t)pack->can[this->can_id].Data[3] << 4 | (uint16_t)pack->can[this->can_id].Data[4] >> 4;
                uint16_t t_int = (uint16_t)pack->can[this->can_id].Data[4] & 0x0F << 8 | ((uint16_t)pack->can[this->can_id].Data[5]);
                 
                this->pos = (float)pos_int * 25 / 65535 - 12.5;
                this->vec = (float)vel_int * 130 / 4095 - 65;
                this->toq = (float)t_int * 100 / 4095 - 50;
            }
        }
    }

}