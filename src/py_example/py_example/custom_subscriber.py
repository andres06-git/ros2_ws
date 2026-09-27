import rclpy
from rclpy.node import Node

# Inclusion del mensaje personalizado desde las interfaces de la actividad
from robx7_interfaces.msg import CustomString

# Definicion del nodo suscriptor en Python
class SubscriberPython(Node):

    def __init__(self):
        # Inicializa el nodo con el nombre "usuario_python_2026"
        super().__init__('usuario_python_2026')
        # Suscripcion al topico "TraficoZMG" vinculada a la funcion listener_callback
        self.subscription = self.create_subscription(
            CustomString,
            'TraficoZMG',
            self.listener_callback,
            10)
        # Evita advertencia de variable no utilizada
        self.subscription

    # Funcion que procesa los mensajes entrantes del topico
    def listener_callback(self, msg):
        self.get_logger().info('Recibido: "%s"' % msg.data)


def main(args=None):
    # Inicializa la comunicacion ROS 2 en Python
    rclpy.init(args=args)

    node = SubscriberPython()
    # Bucle para recibir mensajes de forma continua
    rclpy.spin(node)
    # Destruye el nodo al finalizar
    node.destroy_node()
    # Cierra la libreria rclpy
    rclpy.shutdown()


if __name__ == '__main__':
    main()