from setuptools import find_packages
from setuptools import setup

setup(
    name='lslidar_msgs',
    version='3.0.1',
    packages=find_packages(
        include=('lslidar_msgs', 'lslidar_msgs.*')),
)
