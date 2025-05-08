from setuptools import find_packages
from setuptools import setup

setup(
    name='grpr2f85_ifaces',
    version='0.0.0',
    packages=find_packages(
        include=('grpr2f85_ifaces', 'grpr2f85_ifaces.*')),
)
