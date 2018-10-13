import numpy as np
from keras import models


class Model(object):

    def __init__(self):
        self.model = models.load_model('model/model.h5')
        self.mean  = np.load('model/mean.npy')
        self.std   = np.load('model/std.npy')


    def predict(self, a, b, c, d, e, f):
        # Pick inputs into list
        input = [a, b, c, d, e, f]

        # Turn list into 2D numpy array
        input = np.atleast_2d(input)

        # Normalize input
        input = (input - self.mean) / self.std

        # Return prediction
        prediction = self.model.predict(input)
        return prediction[0, 0]


m = Model()


def predict(a, b, c, d, e, f):
    return m.predict(a, b, c, d, e, f)


def main():
    print(m.predict(0.1, 0.2, 0.3, 0.15, 0.25, 0.35))


if __name__ == '__main__':
    main()
