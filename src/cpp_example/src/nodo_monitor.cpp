#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include "robx7_interfaces/msg/hardware_status.hpp"
#include "robx7_interfaces/srv/check_system.hpp"

class NodoMonitor : public rclcpp::Node
{
public:
  NodoMonitor() : Node("monitor_node"), ultima_temperatura_(0), motores_listos_(false)
  {
    // Declarar y leer el parametro estatico al inicializar
    this->declare_parameter<int64_t>("umbral_temperatura", 50);
    umbral_temperatura_ = this->get_parameter("umbral_temperatura").as_int();

    RCLCPP_INFO(this->get_logger(), "Monitor iniciado. Umbral de seguridad: %ld", umbral_temperatura_);

    // Suscriptor al topico del emulador
    sub_ = this->create_subscription<robx7_interfaces::msg::HardwareStatus>(
      "hardware_status", 10,
      std::bind(&NodoMonitor::cb_hardware_status, this, std::placeholders::_1));

    // Servidor del servicio CheckSystem
    srv_ = this->create_service<robx7_interfaces::srv::CheckSystem>(
      "check_system",
      std::bind(&NodoMonitor::cb_check_system, this, std::placeholders::_1, std::placeholders::_2));
  }

private:
  void cb_hardware_status(const robx7_interfaces::msg::HardwareStatus::SharedPtr msg)
  {
    ultima_temperatura_ = msg->temperatura;
    motores_listos_ = msg->motores_listos;

    // Evalua si supera el umbral para imprimir la advertencia
    if (ultima_temperatura_ > umbral_temperatura_) {
      RCLCPP_WARN(this->get_logger(), "ADVERTENCIA: La temperatura (%ld) supera el umbral (%ld)!", 
                  ultima_temperatura_, umbral_temperatura_);
    }
  }

  void cb_check_system(const std::shared_ptr<robx7_interfaces::srv::CheckSystem::Request> request,
                       std::shared_ptr<robx7_interfaces::srv::CheckSystem::Response> response)
  {
    (void)request; 
    if (motores_listos_ && ultima_temperatura_ <= umbral_temperatura_) {
      response->sistema_seguro = true;
      response->mensaje_autorizacion = "Autorizacion de movimiento concedida";
    } else {
      response->sistema_seguro = false;
      response->mensaje_autorizacion = "Bloqueo por temperatura alta o motores inactivos";
    }

    RCLCPP_INFO(this->get_logger(), "Peticion recibida. Respondiendo: '%s'", 
                response->mensaje_autorizacion.c_str());
  }

  int64_t umbral_temperatura_;
  int64_t ultima_temperatura_;
  bool motores_listos_;

  rclcpp::Subscription<robx7_interfaces::msg::HardwareStatus>::SharedPtr sub_;
  rclcpp::Service<robx7_interfaces::srv::CheckSystem>::SharedPtr srv_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NodoMonitor>());
  rclcpp::shutdown();
  return 0;
}