#ifndef __ECAT_GM6020__
#define __ECAT_GM6020__

#include "ecat_can_base/ecat_base.h"
#include "ecat_can_base/ecat_typedef.h"

#include <inttypes.h>

//--------//
// GM6020 //
//--------//

#define GM6020_TAG1 0x1FF
#define GM6020_TAG2 0x2FF

// GM6020_demo

namespace ecat
{
    class GM6020dlc
    {
    public:
        GM6020dlc(uint8_t can_id, uint32_t motor_id);
        void GM6020_can_set(Ecat_Outputs_Pack *pack, uint16_t I_);
        void GM6020_can_analyze(Ecat_Inputs_Pack *pack);

        /* motor rx data*/
        int16_t pos = 0;
        int16_t vec = 0;
        int16_t toq = 0;
        int8_t temp = 0;

    private:
        /* can&motor port*/
        uint8_t can_id;
        uint32_t motor_id;

        /* motor tx data*/
        int16_t I = 0; // Electric current
    };
}

#endif