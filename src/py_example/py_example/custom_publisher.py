import rclpy
from rclpy.node import Node

# Inclusion del mensaje personalizado desde las interfaces de la actividad
from robx7_interfaces.msg import CustomString

# Definicion del nodo publicador en Python
class PublisherPython(Node):

    def __init__(self):
        # Inicializa el nodo con el nombre "noticias_python"
        super().__init__('noticias_python')
        # Publicador en el topico "TraficoZMG", tipo CustomString y cola de 10
        self.publisher_ = self.create_publisher(CustomString, 'TraficoZMG', 10)
        # Configuracion del timer para ejecutar el callback cada 1 segundo
        timer_period = 1.0  # segundos
        self.timer = self.create_timer(timer_period, self.timer_callback)
        # Contador incremental para el contenido del mensaje
        self.i = 1
        self.get_logger().info('Noticias Python ha iniciado.')

    # Funcion que se ejecuta periodicamente por el timer
    def timer_callback(self):
        msg = CustomString()
        msg.data = 'Mensaje Python: %d' % self.i
        # Envia el mensaje al topico
        self.publisher_.publish(msg)
        self.get_logger().info('Publicado: "%s"' % msg.data)
        self.i += 1

# Funcion principal
def main(args=None):
    # Inicializa la comunicacion ROS 2 en Python
    rclpy.init(args=args)
    node = PublisherPython()
    # Mantiene el nodo activo escuchando eventos y timers
    rclpy.spin(node)
    # Destruye el nodo al salir
    node.destroy_node()
    # Cierra la libreria rclpy
    rclpy.shutdown()


if __name__ == '__main__':
    main()