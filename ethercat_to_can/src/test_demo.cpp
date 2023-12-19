#include "ecat_can_base/EcatBase.h"
#include "ecat_can_base/ecat_typedef.h"
#include "ecat_motor_dlc/ecat_DM4310.h"
#include "ecat_motor_dlc/ecat_GM6020.h"

#include <signal.h>
#include <unistd.h>


bool app_stopped = false;
Ecat_Outputs_Pack packet_tx;
Ecat_Inputs_Pack packet_rx;

void Stop_all(Ecat_Outputs_Pack *packet_tx, Ecat_Inputs_Pack *packet_rx);
void sigint_handler(int sig);

int main()
{
    signal(SIGINT, sigint_handler);

    char phy[] = "enp5s0";
    EcatStart(phy);

    memset(&packet_tx, 0, sizeof(Ecat_Outputs_Pack));
    EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);

    //-------------//
    // DM4310 demo //
    //-------------//

    // DM_can_set(CAN2, &packet, CLEAR_ERROR, DM4310_ID_1, 0, 10.0, 0, 0, 0);
    // EcatSyncMsg((uint8_t *)&packet);

    // DM_can_set(CAN2, &packet_tx, ENABLE, DM4310_ID_1, 0, 0, 0, 0, 0);
    // EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);

    //-----END-----//
    printf("start\n");

    packet_tx.LED = 0x00;

    for (int i = 0; i < 10000; i++)
    {

        //-------------//
        // DM4310 demo //
        //-------------//

        // if (tag == 0)
        //{
        // DM_can_set(CAN2, &packet_tx, MIT_CTRL, DM4310_ID_1, -12, 0, 1, 0.5, 0);
        // EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);
        // tag = 1;
        //}
        // else
        //{
        //    DM_can_set(CAN2, &packet_tx, MIT_CTRL, DM4310_ID_1, 9.0, 0, 1, 0.5, 0);
        //    EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);
        //    tag = 0;
        //}

        // EcatSyncMsg((uint8_t *)&packet);

        //-------------//
        // GM6020 demo //
        //-------------//

        GM6020_can_set(CAN1, &packet_tx, GM6020_TAG1, 0, 2000, 0, 2000);
        packet_tx.LED = 0x0F;

        EcatSyncMsg((uint8_t *)&packet_tx, (uint8_t *)&packet_rx);

        // printf("%x\n", packet_rx.can[0].StdId);
        // printf("%x\n", packet_rx.can[0].StdId);

        //-----END-----//
        //printf("%x\n", packet_rx.can[0].StdId);
        printf("%x\n", packet_rx.can[0].Data[1]);

        if (app_stopped)
        {
            break;
        }
    }

    //-------------//
    // DM4310 demo //
    //-------------//

    //
    printf("stop");
    Stop_all(&packet_tx, &packet_rx);
    EcatStop();

    return 0;
}

// stop
void Stop_all(Ecat_Outputs_Pack *packet_tx, Ecat_Inputs_Pack *packet_rx)
{
    // memset(&packet, 0, sizeof(Ecat_Outputs_Pack));

    // stop led
    packet_tx->LED = 0x00;

    // stop DM4310
    DM_can_set(CAN1, packet_tx, DISABLE, DM4310_ID_1, 0, 0, 0, 0, 0);
    DM_can_set(CAN2, packet_tx, DISABLE, DM4310_ID_1, 0, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)packet_tx, (uint8_t *)packet_rx);

    // stop GM6020
    GM6020_can_set(CAN1, packet_tx, GM6020_TAG1, 0, 0, 0, 0);
    GM6020_can_set(CAN2, packet_tx, GM6020_TAG1, 0, 0, 0, 0);                                                                                                                                                                                          
    EcatSyncMsg((uint8_t *)packet_tx, (uint8_t *)packet_rx);

    GM6020_can_set(CAN1, packet_tx, GM6020_TAG2, 0, 0, 0, 0);
    GM6020_can_set(CAN2, packet_tx, GM6020_TAG2, 0, 0, 0, 0);
    EcatSyncMsg((uint8_t *)packet_tx, (uint8_t *)packet_rx);
}

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        app_stopped = true;
    }
}