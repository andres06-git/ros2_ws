#include <chrono>
#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include "robx7_interfaces/msg/hardware_status.hpp"

using namespace std::chrono_literals;

class EmuladorHardware : public rclcpp::Node
{
public:
  EmuladorHardware() : Node("emulador_hardware"), temperatura_actual_(40)
  {
    // Publica en el topico "estado_hardware"
pub_ = this->create_publisher<robx7_interfaces::msg::HardwareStatus>("hardware_status", 10);    
    // Temporizador a 1 Hz
    timer_ = this->create_wall_timer(
      1s, std::bind(&EmuladorHardware::publicar_estado, this));
      
    RCLCPP_INFO(this->get_logger(), "Nodo Emulador de Hardware iniciado. Simulando telemetría...");
  }

private:
  void publicar_estado()
  {
    auto msg = robx7_interfaces::msg::HardwareStatus();
    
    // Subir temperatura gradualmente de 40 a 85
    temperatura_actual_ += 5;
    if (temperatura_actual_ > 85) {
        temperatura_actual_ = 40; // Reiniciar ciclo
    }

    // Asignacion de valores al mensaje
    msg.temperatura = temperatura_actual_;
    msg.motores_listos = (temperatura_actual_ < 80); // Falla si pasa de 80
    
    if (temperatura_actual_ <= 50) {
        msg.debug_msg = "Operacion normal";
    } else if (temperatura_actual_ <= 75) {
        msg.debug_msg = "Calentando";
    } else {
        msg.debug_msg = "Fallo de sensor / Sobrecalentamiento";
    }

    // Impresion en consola y publicacion
    RCLCPP_INFO(this->get_logger(), "Publicando -> Temp: %ld, Motores listos: %s, Msg: %s", 
                msg.temperatura, msg.motores_listos ? "Si" : "No", msg.debug_msg.c_str());
    pub_->publish(msg);
  }

  rclcpp::Publisher<robx7_interfaces::msg::HardwareStatus>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  int64_t temperatura_actual_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<EmuladorHardware>());
  rclcpp::shutdown();
  return 0;
}