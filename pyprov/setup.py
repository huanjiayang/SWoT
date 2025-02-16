from setuptools import setup

setup(
    name='pyprov',
    version='1.0.10',
    author='Huanjia Yang',
    author_email='huanjiayang@hotmail.com',
    packages=['pyprov', 'pyprov.model','pyprov.model.examples','pyprov.model.test'],
    scripts=[],
    url='http://github.com/huanjiayang',
    license='LICENSE.txt',
    description='A Python implementation of W3C PROV data model.',
    long_description=open('README').read(),
    install_requires=[],
)