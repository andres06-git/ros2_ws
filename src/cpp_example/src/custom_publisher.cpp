#include <chrono>
#include <functional>
#include <memory>
#include <string>

// Libreria C++ para ROS 2
#include <rclcpp/rclcpp.hpp>
// Inclusion del tipo de mensaje personalizado generado en robx7_interfaces
#include "robx7_interfaces/msg/custom_string.hpp"

// Namespace para utilizar literales de tiempo como 1 segundo
using namespace std::chrono_literals;

// Definicion de la clase PublisherCpp que hereda de rclcpp::Node
class PublisherCpp : public rclcpp::Node
{

  private:
    // PUntero al publicador de mensaje personalizado 
    rclcpp::Publisher<robx7_interfaces::msg::CustomString>::SharedPtr pub_;
    // Puntero al timer para la ejecucion periodica
    rclcpp::TimerBase::SharedPtr timer_;
    // Contador para llevar la secuencia de mensajes enviados
    int contador_ = 0;
  
  // FUncion callback ejecutada periodicamente por el timer 
  void publicar_mensaje()
  {
      // Mensaje del tipo CustomString
      auto message = robx7_interfaces::msg::CustomString();
      contador_++;
      message.data = "Mensaje C++: " + std::to_string(contador_);
      // Impresion del mensaje publicado en la consola
      RCLCPP_INFO(this->get_logger(), "Publicado: '%s'", message.data.c_str());
      // Publicacion del mensaje en el topico
      pub_->publish(message);
  }
  public:
    // Constructor de la clase, nombra al nodo como "noticias_carlos2"
    PublisherCpp() : Node("noticias_carlos2")
    {
      // Creacion del publisher en el topico "TraficoZMG"
      pub_= create_publisher<robx7_interfaces::msg::CustomString>("TraficoZMG",10);
      RCLCPP_INFO(get_logger(), "Noticias carlos ha iniciado.");

      // Inicializacion del timer para llamar a publicar_mensaje cada 1 segundo
      timer_ = create_wall_timer(
     1s,
     std::bind(&PublisherCpp::publicar_mensaje, this)
     );
    }
    
};

int main(int argc, char *argv[])
{
  // Inicializacion del sistema de comunicacion de ROS 2
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PublisherCpp>();
  // Mantiene el nodo activo escuchando y ejecutando callbacks (spin)
  rclcpp::spin(node);
  // Cierre limpio de ROS 2 al terminar la ejecucion
  rclcpp::shutdown();
  return 0;
}



