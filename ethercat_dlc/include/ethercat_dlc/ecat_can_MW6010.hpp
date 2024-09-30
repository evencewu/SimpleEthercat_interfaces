#pragma once

#include "ethercat_interfaces/EcatBase.hpp"
#include "ethercat_interfaces/TypeDef.hpp"

#include <inttypes.h>

namespace ecat
{
    class MW6010dlc
    {
    public:
        MW6010dlc(uint8_t _can_id, uint32_t _motor_id);

        void MW6010_can_set(ecat::Ecat_Outputs_Pack *pack, int mode,float _pos, float _vel, float _KP, float _KD, float _torq);
        void MW6010_can_analyze(Ecat_Inputs_Pack *pack);

        float pos;
        float vec;
        float toq;
        float temp_mos;
        float temp;

        static const int ENABLE = 0;
        static const int MIT_CTRL = 1;
        static const int SELECT_MODE = 2;
        static const int DISABLE = 3;
        
    private:

        /* can&motor port*/
        uint8_t can_id;
        uint32_t motor_id;
    };
}