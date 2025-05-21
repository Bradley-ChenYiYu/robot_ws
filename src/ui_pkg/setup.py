from setuptools import find_packages, setup

package_name = 'ui_pkg'

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
    maintainer='jason9308',
    maintainer_email='jason9308@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'map_viewer_node = ui_pkg.map_viewer:main',
            'chat_viewer_node = ui_pkg.chat_viewer:main',
            'main_ui = ui_pkg.main_ui:main',
        ],
    },
)
