#include "ecat_motor_dlc/ecat_GM6020.h"

namespace ecat
{
    GM6020dlc::GM6020dlc(uint8_t can_id, uint32_t motor_id)
    {
        this->can_id = can_id;
        this->motor_id = motor_id;
    }

    void GM6020dlc::GM6020_can_set(Ecat_Outputs_Pack *pack,uint16_t I_)
    {
        pack->can[can_id].DLC = 0x08;
        if (motor_id <= 4)
        {
            pack->can[can_id].StdId = GM6020_TAG1;
            pack->can[can_id].Data[motor_id * 2 - 2] = I_ >> 8 & 0xFF;
            pack->can[can_id].Data[motor_id * 2 - 1] = I_ & 0xFF;
        }
        else
        {
            pack->can[can_id].StdId = GM6020_TAG2;
            pack->can[can_id].Data[motor_id * 2 - 10] = I_ >> 8 & 0xFF;
            pack->can[can_id].Data[motor_id * 2 - 9] = I_ & 0xFF;
        }
    }

    void GM6020dlc::GM6020_can_analyze(Ecat_Inputs_Pack *pack)
    {
        uint32_t tag = pack->can[can_id].StdId;

        if(tag == 0x204 + motor_id)
        {
            this->pos = (uint16_t)pack->can[can_id].Data[0] << 8 | (uint16_t)pack->can[can_id].Data[1];
            this->vec = (uint16_t)pack->can[can_id].Data[2] << 8 | (uint16_t)pack->can[can_id].Data[3];
            this->toq = (uint16_t)pack->can[can_id].Data[4] << 8 | (uint16_t)pack->can[can_id].Data[5];
            this->temp = (uint8_t)pack->can[can_id].Data[6];
        }
    }
}