#include "ecat_can_base/ecat_base.h"
#include "ecat_motor_dlc/ecat_DM4310.h"
#include "ecat_motor_dlc/ecat_GM6020.h"

#include <array>
#include <signal.h>
#include <unistd.h>

bool app_stopped = false;

void sigint_handler(int sig);
void save_stop();

ecat::EcatBase Ethercat;

std::array<ecat::GM6020dlc, 2> Motor6020 = {
    ecat::GM6020dlc(CAN2, 2),
    ecat::GM6020dlc(CAN1, 7)};

int main()
{
    signal(SIGINT, sigint_handler);

    char phy[] = "enp5s0";
    Ethercat.EcatStart(phy);
    Ethercat.SetUserStop(save_stop);

    printf("start\n");

    float kp = 5;
    float kd = 10;
    float I_1 = 0;

    for (int i = 0; i < 10000000; i++)
    {
        // printf("loop\n");
        Ethercat.packet_tx.LED = 0x04;

        Motor6020[0].GM6020_can_set(&Ethercat.packet_tx, I_1);
        Motor6020[1].GM6020_can_set(&Ethercat.packet_tx, 0);
        Ethercat.EcatSyncMsg();
        Motor6020[0].GM6020_can_analyze(&Ethercat.packet_rx);
        Motor6020[1].GM6020_can_analyze(&Ethercat.packet_rx);

        I_1 = (Motor6020[0].pos - Motor6020[1].pos) * kp - (Motor6020[0].pos - Motor6020[1].pos) * kd;

        printf("%f\n", I_1);
        printf("%d\n", Motor6020[0].pos);
        printf("%d\n", Motor6020[1].pos);
        printf("--------------------------------\n");

        if (app_stopped)
        {
            break;
        }
    }

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

void save_stop()
{
    Motor6020[0].GM6020_can_set(&Ethercat.packet_tx, 0);
    Motor6020[1].GM6020_can_set(&Ethercat.packet_tx, 0);
    // Repeat 3 times to prevent packet loss
    Ethercat.EcatSyncMsg();
    Ethercat.EcatSyncMsg();
    Ethercat.EcatSyncMsg();
    printf("stop motor!");
}