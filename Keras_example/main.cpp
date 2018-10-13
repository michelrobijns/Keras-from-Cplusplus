#include <iostream>
#include <string>
#include <array>
#include <exception>
#include <Python.h>

using std::cout;
using std::endl;
using std::string;
using std::array;
using std::runtime_error;


int valid_module(PyObject *pModule)
{
    if (pModule != NULL) {
        return true;
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load module\n");

        return false;
    }
}


int valid_function(PyObject *pFunc)
{
    if (pFunc && PyCallable_Check(pFunc)) {
        return true;
    } else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }

        fprintf(stderr, "Cannot find function\n");

        return false;
    }
}


class KerasModel
{
public:
    KerasModel(std::string module, std::string function)
    {
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");

        // Load the Python module
        PyObject *pName = PyUnicode_DecodeFSDefault(module.c_str());
        pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (!valid_module(pModule)) {
            throw runtime_error("Failed to load module");
        }

        pFunc = PyObject_GetAttrString(pModule, function.c_str());

        if (!valid_function(pFunc)) {
            throw runtime_error("Cannot find function");
        }
    }

    double predict(array<double, 6> input)
    {
        // Convert the input to a Python tuple
        PyObject *pArgs = PyTuple_New(6);
        PyObject *pValue;

        for (int i = 0; i < 6; ++i) {
            pValue = PyFloat_FromDouble(input[i]);

            if (!pValue) {
                Py_DECREF(pArgs);
                throw runtime_error("Cannot convert to PyFloat");
            }

            PyTuple_SetItem(pArgs, i, pValue);
        }

        // Call the predict function in Python
        pValue = PyObject_CallObject(pFunc, pArgs);

        Py_DECREF(pArgs);

        if (pValue != NULL) {
            double result = PyFloat_AsDouble(pValue);
            
            Py_DECREF(pValue);

            return result;
        } else {
            Py_DECREF(pValue);
            
            throw runtime_error("Call failed");
        }
    }

    ~KerasModel()
    {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_FinalizeEx();
    }

    // Data members
    PyObject *pModule;
    PyObject *pFunc;
};


int main()
{
    KerasModel keras_model("model", "predict");

    array<double, 6> input = {0.1, 0.2, 0.3, 0.15, 0.25, 0.35};

    cout << keras_model.predict(input) << endl;

    return 0;
}
