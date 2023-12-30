# Ecat_ros2_pack

基于的SOEM的简单ethercat_2_can主站，并且有常见伺服器的dlc

* ## 环境&依赖
    * Ubuntu版本: Ubuntu22.04 
    * ROS版本: ros2-humble
    * 依赖包: [soem_for_ros2](https://github.com/evencewu/soem_for_ros2)

* ## 编译安装
    ```bash
    cd ${your_ros_ws}/src

    git clone https://github.com/evencewu/soem_for_ros2.git
    git clone https://github.com/evencewu/Ecat_ros2_pack.git

    cd ${your_ros_ws}

    colcon build
    ```

* ## 如何使用
    ### 程序结构(以ros2常规结构为例)
    ### xxx_interfaces.h
    ```c++
    #include <xxxx>
    #include "ecat_can_base/ecat_base.hpp"
    #include "ecat_motor_dlc/ecat_DM4310.hpp"
    #include "your_pack/your_motor_dlc.hpp"
    
    namespace xxx_interfaces
    {
        class xxx_InterfacesNode : public rclcpp::Node
        {
        public:
            xxx_InterfacesNode();
            ~xxx_InterfacesNode() override;

        private:
            void init();
            void main_loop();
            void analyze_all()；

            void safe_stop();

            ecat::EcatBase Ethercat;

            //CAN1代表 节点上的can编号 1代表motorid
            //实例化每个电机包含电机各项参数
            ecat::DM4310dlc  MotorDM_4310(CAN1, 1);
        };
    }
    ```
    ### xxx_interfaces.c
    ```c++
    #include "your_pack/xxx_interfaces.hpp"

    namespace xxx_interfaces
    {
        xxxInterfacesNode::xxxInterfacesNode() : Node("xxx_interfaces_node")
        {
            init();

            while(1)
            {
                main_loop();
            }
        }

        xxxInterfacesNode::~xxxInterfacesNode()
        {
            //用户自定义的安全停止代码
            safe_stop();
            //ethercat停止通信
            Ethercat.EcatStop();
        }

        void xxxInterfacesNode::init()
        {
            char phy[] = "enp5s0";
            Ethercat.EcatStart(phy);

            //电机使能

            //通过MotorDM_4310更改 Ethercat.packet_tx[0] 0代表从站节点0
            MotorDM_4310.DM_can_set(&Ethercat.packet_tx[0], ENABLE, 0, 0, 0, 0, 0);
            //执行ethercat收发
            Ethercat.EcatSyncMsg();
            //解析数据
            analyze_all();
        }

        void xxxaInterfacesNode::analyze_all()
        {
            MotorDM_4310.DM_can_analyze(&Ethercat.packet_rx[0]);
        }

        /// @brief arx5a 主循环函数
        void xxxInterfacesNode::main_loop()
        {
            //主程序
        }

        void xxxInterfacesNode::safe_stop()
        {
            //发送电机停止指令以安全状态停止(为了防止丢包导致的疯车，建议多次发送)
        }
    }

    int main(int argc, char **argv)
    {
      rclcpp::init(argc, argv);
      rclcpp::spin(std::make_shared<arx5a_interfaces::Arx5aInterfacesNode>());
      rclcpp::shutdown();
      return 0;
    }
    ```

    







