# Using Keras from C++

This repository shows how Keras models might be used to make predictions from within a C++ code base. My solution is rather simple: use the Python/C API to load a Python module and call the predict function inside that module. The Keras model is loaded once and stays in memory until the module pointer is dereferenced. Therefore, you can make as many predictions as you  like without the overhead of loading the Keras model into memory every single time you make a prediction.

## What's in this repository?

1. The folder `basic_example` shows how to run a simple Python function from within C++. It is a slightly modified version of the example in [the official Python documentation](https://docs.python.org/3.7/extending/embedding.html).
2. The folder `Keras_example` shows how to do inference with a simple Keras model from within C++. The previously trained Keras model is stored as `model.h5`. The files `mean.npy` and `std.npy` are also loaded for the purpose of normalizing inputs.

## Prerequisites

* Python with Keras, TensorFlow, and NumPy
* A C++ compiler (I use g++)

## Usage

You need to find the required compiler and linker flags for your system. To do so, open a terminal and run

```bash
$ pythonX.Y-config --cflags
```

and

```bash
$ pythonX.Y-config --ldflags
```

where X.Y is your version of Python. Copy and paste the output into the makefile and the code should compile.

To run the examples, simply call `run.sh`.
