#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program reads the greek dataset from the CSVs and uses the pretrained MNIST model to generate
an embedding space for each Greek symbol. These embeddings are saved to a CSV file.
"""

import numpy as np

from tensorflow import keras


def truncated_model():
    m = keras.models.load_model("model")
    m_trunc = keras.Model(inputs=m.inputs, outputs=m.layers[5].output)
    m_trunc.summary()
    return m_trunc


def read_data():
    p = np.genfromtxt("data/greek/pixels.csv")
    l = np.genfromtxt("data/greek/labels.csv")
    x = p.reshape((27, 28, 28))
    return x, l


if __name__ == '__main__':
    model = truncated_model()
    imgs, labels = read_data()
    p = model(imgs).numpy()
    np.savetxt("data/greek/embeddings.csv", p, header="Embeddings")
