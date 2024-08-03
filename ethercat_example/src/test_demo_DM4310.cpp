#include "ecat_can_base/ecat_base.hpp"
#include "ecat_can_base/ecat_typedef.hpp"
#include "ecat_motor_dlc/ecat_DM4310.hpp"
#include "ecat_motor_dlc/ecat_GM6020.hpp"

#include <array>
#include <signal.h>
#include <unistd.h>

bool app_stopped = false;

void sigint_handler(int sig);
void safe_stop();

ecat::EcatBase Ethercat(1);

std::array<ecat::DM4310dlc, 2> MotorDM = {
    ecat::DM4310dlc(CAN1, 5),
    ecat::DM4310dlc(CAN1, 6)};

int main()
{
    signal(SIGINT, sigint_handler);

    char phy[] = "enp5s0";
    Ethercat.EcatStart(phy);

    printf("start\n");

    for (int i = 0; i < 10000000; i++)
    {

        MotorDM[0].DM_can_set(&Ethercat.packet_tx[0], 3, 0, 0, 0, 0, 0);

        Ethercat.EcatSyncMsg();
        printf("----------------\n");
        printf("0x%hX\n", (__uint16_t)Ethercat.packet_rx[0].can[0].Data[0]);

        MotorDM[0].DM_can_set(&Ethercat.packet_tx[0], 3, 0, 0, 0, 0, 0);

        Ethercat.EcatSyncMsg();
        printf("----------------\n");
        printf("0x%hX\n", (__uint16_t)Ethercat.packet_rx[0].can[0].Data[0]);

        if (app_stopped)
        {
            break;
        }
    }

    safe_stop();
    Ethercat.EcatStop();

    printf("stop");

    return 0;
}

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        app_stopped = true;
    }
}

void safe_stop()
{
    printf("stop motor!");
}