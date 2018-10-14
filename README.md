# Using Keras from C++

This repository shows how Keras models might be used to make predictions from within a C++ code base. The solution is straightforward: the Python/C API is used to initialize an instance of a class wrapped around the Keras model. The Keras model is loaded once and stays in memory as long as desired. Therefore, many predictions may be made without the overhead of reloading the Keras model.

## What's in this repository?

1. The folder `basic_example` contains an example of calling a Python function from within C++. It is a slightly modified version of the example in [the official Python documentation](https://docs.python.org/3.7/extending/embedding.html).
2. The folder `Keras_example` contains an example of how a Keras model can be used to make predictions from within C++.

## Prerequisites

* Python with Keras, TensorFlow, and NumPy
* A C++ compiler

## Usage

You need to find the required compiler and linker flags for your system. To do so, open a terminal and run

```bash
$ pythonX.Y-config --cflags
```

and

```bash
$ pythonX.Y-config --ldflags
```

where you replace X.Y by your version of Python. Copy and paste the output into the makefile and the code should compile.
