#include "ethercat_interfaces/EcatBase.hpp"
#include "ethercat_interfaces/TypeDef.hpp"
#include "ethercat_dlc/ecat_can_DM4310.hpp"
#include "ethercat_dlc/ecat_can_GM6020.hpp"

#include <array>
#include <signal.h>
#include <unistd.h>

#include "rclcpp/rclcpp.hpp"

ecat::EcatBase Ethercat = ecat::EcatBase(1);

std::array<ecat::DM4310dlc, 2> MotorDM = {
    ecat::DM4310dlc(CAN1, 1),
    ecat::DM4310dlc(CAN1, 2)};

class FrequencyNode : public rclcpp::Node
{
public:
    FrequencyNode()
        : Node("frequency_node")
    {

        // 设置定时器频率为 10 Hz
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), // 1000 ms / 10 = 100 ms per callback
            [this]() -> void
            {
                this->timer_callback();
            });
    }

    ~FrequencyNode()
    {
        SafeStop();
        Ethercat.EcatStop();
    }

private:
    void timer_callback()
    {

        if (if_init == false)
        {
            Init();
            /*
            // MotorDM[1].DM_can_set(&Ethercat.packet_tx[0], MIT_CTRL, 0, 0, 0, 0, 0.5);
            Ethercat.packet_tx[0].LED = 0x06;
            MotorDM[0].DM_can_set(&Ethercat.packet_tx[0], ENABLE, 0, 0, 0, 0, 0);
            Ethercat.EcatSyncMsg();

            Ethercat.packet_tx[0].LED = 0x06;
            MotorDM[1].DM_can_set(&Ethercat.packet_tx[0], ENABLE, 0, 0, 0, 0, 0);
            Ethercat.EcatSyncMsg();
            */
            if_init = true;
        }
        else
        {
            
            Ethercat.packet_tx[0].LED = 0x06;
            MotorDM[0].DM_can_set(&Ethercat.packet_tx[0], ENABLE, 0, 0, 0, 0, 0);
            //MotorDM[1].DM_can_set(&Ethercat.packet_tx[0], MIT_CTRL, 0, 0, 0, 0, 0.5);
            Ethercat.EcatSyncMsg();

            std::this_thread::sleep_for(std::chrono::microseconds(500));

            Ethercat.packet_tx[0].LED = 0x06;
            MotorDM[1].DM_can_set(&Ethercat.packet_tx[0], ENABLE, 0, 0, 0, 0, 0);
            //MotorDM[0].DM_can_set(&Ethercat.packet_tx[0], MIT_CTRL, 0, 0, 0, 0, 0.5);
            Ethercat.EcatSyncMsg();
            
        }

        RCLCPP_INFO(this->get_logger(), "Timer callback executed.");
    }

    void Init()
    {
        char phy[] = "enp5s0";
        Ethercat.EcatStart(phy);
    }

    void SafeStop()
    {
        for (int i = 0; i < 1000; i++)
        {
            MotorDM[0].DM_can_set(&Ethercat.packet_tx[0], DISABLE, 0, 0, 0, 0, 0);
            Ethercat.packet_tx[0].LED = 0;
            Ethercat.EcatSyncMsg();
        }

        for (int i = 0; i < 1000; i++)
        {
            MotorDM[1].DM_can_set(&Ethercat.packet_tx[0], DISABLE, 0, 0, 0, 0, 0);
            Ethercat.packet_tx[0].LED = 0;
            Ethercat.EcatSyncMsg();
        }
    }

    bool if_init = false;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FrequencyNode>());
    rclcpp::shutdown();
    return 0;
}