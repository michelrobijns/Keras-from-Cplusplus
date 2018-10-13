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
    KerasModel(std::string module_name, std::string class_name,
        std::string function_name)
    {
        // Initialize the Python interpreter
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");

        // Load the Python module and get a pointer to the module
        PyObject *pName = PyUnicode_DecodeFSDefault(module_name.c_str());
        pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (!valid_module(pModule)) {
            throw runtime_error("Failed to load module");
        }

        // Get a pointer to the `Model' class
        pInstance = PyObject_GetAttrString(pModule, class_name.c_str());
        
        // Initialize instance of `Model'
        pInstance = PyObject_CallObject(pInstance, NULL);

        // Get a pointer to the `predict' member function of `Model'
        pFunc = PyObject_GetAttrString(pInstance, function_name.c_str());

        if (!valid_function(pFunc)) {
            throw runtime_error("Cannot find function");
        }
    }

    double predict(array<double, 6> input)
    {
        // Convert the input array to a Python tuple
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

        // Call the `predict' member function of `Model'
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
        Py_DECREF(pInstance);
        Py_DECREF(pModule);
        Py_FinalizeEx();
    }

    // Data members
    PyObject *pModule;
    PyObject *pInstance;
    PyObject *pFunc;
};


int main()
{
    KerasModel keras_model("model", "Model", "predict");

    array<double, 6> input = {0.1, 0.2, 0.3, 0.15, 0.25, 0.35};

    cout << keras_model.predict(input) << endl;

    return 0;
}
