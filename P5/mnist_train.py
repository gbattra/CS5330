#!/usr/bin/env python

# Greg Attra
# 03/27/2021

"""
This executable builds and trains a CNN to classify digits from the MNIST dataset.
"""

import numpy as np
import matplotlib.pyplot as plt

from tensorflow import keras
from tensorflow.keras import layers

# a random seed to make results reproducible
seed = np.random.seed(42)


def model(input_shape, n_classes):
    """
    Builds the model to train.
    :param input_shape: the shape of the input images (28, 28, 1)
    :param n_classes: the number of classes in the dataset (10)
    :return: a Keras sequential CNN for classifying the MNIST dataset
    """

    m = keras.Sequential([
        keras.Input(shape=input_shape),
        layers.Conv2D(32, kernel_size=(3, 3), activation="relu"),
        layers.Conv2D(32, kernel_size=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Dropout(0.5),
        layers.Flatten(),
        layers.Dense(128, activation="relu"),
        layers.Dropout(0.5),
        layers.Dense(n_classes, activation="softmax")
    ])

    m.summary()
    return m


def load_data():
    """
    Loads the MNIST data and preps it for training
    :return: the training and test datasets
    """

    (x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data("mnist.npz")

    x_train = x_train.astype("float32") / 255
    x_test = x_test.astype("float32") / 255
    x_train = np.expand_dims(x_train, -1)
    x_test = np.expand_dims(x_test, -1)

    y_train = keras.utils.to_categorical(y_train, num_classes=10)
    y_test = keras.utils.to_categorical(y_test, num_classes=10)

    fig, ax = plt.subplots(2, 1)
    fig.suptitle("First Two Samples")
    ax[0].imshow(x_train[0, :, :, 0])
    ax[1].imshow(x_train[1, :, :, 0])
    plt.show()

    return (x_train, y_train), (x_test, y_test)


def plot_history(hist):
    """
    Plots the training history of the model.
    :param hist: the history of the training
    :return: None
    """

    plt.plot(hist.history["accuracy"])
    plt.plot(hist.history["val_accuracy"])
    plt.title("Model Accuracy")
    plt.ylabel("Accuracy")
    plt.xlabel("Epochs")
    plt.legend(["Train", "Test"], loc="upper left")
    plt.show()


def main():
    """
    Entrypoint to the program.
    :return: 0 if success, 1 if error
    """

    (x_train, y_train), (x_test, y_test) = load_data()
    m = model((28, 28, 1), 10)
    m.compile(loss="categorical_crossentropy", optimizer="adam", metrics=["accuracy"])
    hist = m.fit(x_train, y_train, batch_size=128, epochs=10, validation_split=0.3)
    score = m.evaluate(x_test, y_test)
    plot_history(hist)

    print("Test loss: ", score[0])
    print("Test acc: ", score[1])

    m.save("models/mnist")

    return 0


if __name__ == "__main__":
    main()
