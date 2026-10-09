from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Inicia el Emulador de Hardware
        Node(
            package='cpp_example',
            executable='emulador_hardware',
            output='screen'
        ),
        # Inicia el Monitor y sobrescribe el parametro a 75
        Node(
            package='cpp_example',
            executable='nodo_monitor',
            output='screen',
            parameters=[
                {'umbral_temperatura': 75}
            ]
        ),
        # Inicia el Operador
        Node(
            package='cpp_example',
            executable='nodo_operador',
            output='screen'
        )
    ])
