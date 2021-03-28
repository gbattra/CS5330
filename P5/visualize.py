#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This executable examines a trained model and visualizes how it processes the data.
"""

import numpy as np
import matplotlib.pyplot as plt
import cv2

from tensorflow import keras
from matplotlib import colors


def plot_weights(weights):
    """
    Visualize the weight values in subplots.
    :param weights: the weights of the layer
    :return: None
    """

    fig, ax = plt.subplots(8, 4)
    for i in range(weights[0].shape[3]):
        weight = weights[0][:, :, 0, i]
        ax[i // 4, i % 4].imshow(weight)

    fig.suptitle("Layer 1 Weights")
    plt.show()


def apply_weights(weights, img):
    """
    Applies each 3x3 filter as a 2D filter over the image.
    :param weights: the weights for the layer (3, 3, 1, 32)
    :return: None
    """

    fig, ax = plt.subplots(8, 4)
    for i in range(weights[0].shape[3]):
        weight = weights[0][:, :, 0, i]
        dst = cv2.filter2D(img, -1, weight)
        ax[i // 4, i % 4].imshow(dst)

    fig.suptitle("Weights as Filters")
    plt.show()


def apply_truncated(m, img):
    """
    Applies the first layer of the model to the img and visualizes the results.
    :param m: the truncated model
    :param img: the image to process
    :return: None
    """

    p = m.predict(img)
    fig, ax = plt.subplots(8, 4)
    for i in range(p.shape[3]):
        output = p[0, :, :, i]
        ax[i // 4, i % 4].imshow(output)

    fig.suptitle("Truncated Outputs")
    plt.show()


def main():
    """
    Visualizes the weights and how they process the data.
    :return: 0 if success, -1 otherwise
    """

    m = keras.models.load_model("model")
    weights = np.array(m.layers[0].get_weights())
    plot_weights(weights)

    (x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data("mnist.npz")
    apply_weights(weights, x_train[0])

    m_truncated = keras.Model(inputs=m.inputs, outputs=m.layers[0].output)
    apply_truncated(m_truncated, x_train[:1])


if __name__ == "__main__":
    main()
