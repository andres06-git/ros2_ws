#include <chrono>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "robx7_interfaces/srv/check_system.hpp"

using namespace std::chrono_literals;

class NodoOperador : public rclcpp::Node
{
public:
  NodoOperador() : Node("nodo_operador")
  {
    // Creacion del cliente para el servicio "check_system"
    cliente_ = this->create_client<robx7_interfaces::srv::CheckSystem>("check_system");
    
    // Temporizador que ejecutara la solicitud cada 3 segundos
    timer_ = this->create_wall_timer(
      3s, std::bind(&NodoOperador::solicitar_autorizacion, this));
      
    RCLCPP_INFO(this->get_logger(), "Operador iniciado. Solicitando autorizacion cada 3 segundos...");
  }

private:
  void solicitar_autorizacion()
  {
    // Verificar si el Nodo Monitor esta encendido y ofreciendo el servicio
    if (!cliente_->wait_for_service(1s)) {
      RCLCPP_WARN(this->get_logger(), "El servicio check_system no esta disponible. Esperando...");
      return;
    }

    auto request = std::make_shared<robx7_interfaces::srv::CheckSystem::Request>();

    // Enviar peticion asincrona para no bloquear el nodo
    cliente_->async_send_request(request,
      [this](rclcpp::Client<robx7_interfaces::srv::CheckSystem>::SharedFuture future) {
        auto response = future.get();
        // Evaluar la respuesta devuelta por el nodo monitor
        if (response->sistema_seguro) {
          RCLCPP_INFO(this->get_logger(), "[SEGURO] -> %s", response->mensaje_autorizacion.c_str());
        } else {
          RCLCPP_ERROR(this->get_logger(), "[PELIGRO] -> %s", response->mensaje_autorizacion.c_str());
        }
      }
    );
  }

  rclcpp::Client<robx7_interfaces::srv::CheckSystem>::SharedPtr cliente_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NodoOperador>());
  rclcpp::shutdown();
  return 0;
}