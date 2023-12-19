#ifndef __EcatBase__
#define __EcatBase__

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "soem_ros2/soem.h"
#include "ecat_can_base/ecat_typedef.h"

#define EC_VER1

namespace ecat
{
    class EcatBase
    {
    public:
        EcatBase();
        ~EcatBase();

        void EcatStart(char *ifname);
        void EcatSyncMsg();
        void EcatStop();

        void SetUserStop(void (*save_stop)());

        Ecat_Outputs_Pack packet_tx;
        Ecat_Inputs_Pack packet_rx;
    private:
        void (*UserStop)();

        int pdo_output_byte = 30;
        int pdo_input_byte = 34;

        char IOmap[4096];
        volatile int wkc;
        int expectedWKC;
        int EC_TIMEOUTMON=500;
    };
}

// #include "ecat_can_base/ecat_typedef.h"

#endif