from setuptools import find_packages, setup

package_name = 'py_example'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='andres',
    maintainer_email='andres.gonzalez5767@alumnos.udg.mx',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'test_node = py_example.test_node:main',
            'custom_publisher_py = py_example.custom_publisher:main',
            'custom_subscriber_py = py_example.custom_subscriber:main',
        ],
    },
)
