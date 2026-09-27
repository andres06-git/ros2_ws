#include <rclcpp/rclcpp.hpp>
#include "robx7_interfaces/msg/custom_string.hpp"

// Definicion de la clase SubscriberCpp que hereda de rclcpp::Node
class SubscriberCpp : public rclcpp::Node
{

  private:
  // Puntero inteligente a la suscripcion
    rclcpp::Subscription<robx7_interfaces::msg::CustomString>::SharedPtr sub_;
  
  public:
    // Constructor de la clase, nombra al nodo como "Usuario_2026"
    SubscriberCpp() : Node("Usuario_2026")
    {
      // Creacion de la suscripcion al topico "TraficoZMG" con cola QoS de 10
      // Utiliza una funcion como callback para procesar cada mensaje recibido
      sub_= create_subscription<robx7_interfaces::msg::CustomString>("TraficoZMG",10,
        [this](const robx7_interfaces::msg::CustomString::SharedPtr msg)
        {
          // Imprime el contenido del mensaje en la consola
          RCLCPP_INFO(get_logger(), "Recibido: '%s'", msg->data.c_str());
        }
      );
    }

};

int main(int argc, char *argv[])
{
  // Inicializacion de ROS 2
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SubscriberCpp>();
  // BUcle para recibir mensajes continuamente
  rclcpp::spin(node);
  // Liberacion de recursos de ROS 2
  rclcpp::shutdown();
  return 0;
}