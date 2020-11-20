from distutils.core import Extension, setup
from Cython.Build import cythonize
from skbuild import setup

# setup(packages=['wrapper', 'primes'])
# ext = Extension(name="cython_messaging", sources=["cython_messaging.pyx"])
# ext = Extension(name="primes", sources=["primes.pyx"], language="c++")
setup(packages=['wrapper', 'primes'])
# setup(ext_modules=cythonize(ext))
