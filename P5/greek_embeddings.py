#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program reads the greek dataset from the CSVs and uses the pretrained MNIST model to generate
an embedding space for each Greek symbol. These embeddings are saved to a CSV file.
"""

import numpy as np
import cv2

from tensorflow import keras


def truncated_model():
    """
    Returns a truncated model for computing the Greek symbol embeddings.
    :return: the truncated model
    """
    m = keras.models.load_model("models/mnist")
    m_trunc = keras.Model(inputs=m.inputs, outputs=m.layers[5].output)
    m_trunc.summary()
    return m_trunc


def read_data():
    """
    Reads in the Greek image data from the generated CSVs.
    :return: the formatted data and labels
    """
    p = np.genfromtxt("data/greek/pixels.csv")
    l = np.genfromtxt("data/greek/labels.csv")
    x = p.reshape((27, 28, 28))
    return x, l


def main():
    """
    Computes the embeddings for each image in the Greek image dataset and saves them to
    a CSV.
    :return: None
    """
    model = truncated_model()
    imgs, labels = read_data()
    p = model(imgs).numpy()
    np.savetxt("data/greek/embeddings.csv", p, header="Embeddings")


if __name__ == '__main__':
    main()
