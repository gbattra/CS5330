#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program experiments with different variations of the network architecture for a model
trained on the MNIST Fashion dataset. As a baseline, the original model used for the MNIST Numbers
dataset is used.
"""

from tensorflow import keras
from tensorflow.keras import layers
from mnist_train import model


N_conv_layers = [2, 3, 4]
N_conv_filters = [32, 64, 128]
N_dense_layers = [2, 3, 4]
N_dense_units = [50, 100, 200, 400]
conv_filter_size = [3, 5]


def generate_config(c, f, s, d, u):
    """
    Generates a configuration object given the indices for each config
    param.
    :param c: index for number of conv layers
    :param f: index for number of conv filters
    :param s: index for conv filter size
    :param d: index for number of dense layers
    :param u: index for number of dense units
    :return: a config dictionary
    """
    config = {
        'N_conv_layers': N_conv_layers[c],
        'N_conv_filters': N_conv_filters[f],
        'N_dense_layers': N_dense_layers[d],
        'N_dense_units': N_dense_units[u],
        'conv_filter_size': conv_filter_size[s],
    }
    return config


def build_configurations():
    """
    Builds a list of each possible configuration combination.
    :return: the list of configurations
    """
    configs = []
    for c in N_conv_layers:
        for f in N_conv_filters:
            for s in conv_filter_size:
                for d in N_dense_layers:
                    for u in N_dense_units:
                        configs.append(generate_config(c, f, s, d, u))
    return configs


def generate_dense_layers(d, u, l):
    """
    Adds the dense layers to the model layers.
    :param d: the number of dense layers to add
    :param u: the number of units for the dense layers
    :param l: the model layers
    :return: None
    """
    for _ in d:
        l.append(layers.Dense(u, activation="relu"))
        l.append(layers.Dropout(0.5))


def generate_conv_layers(c, f, s, l):
    """
    Adds the conv layers to the model layers.
    :param c: the number of conv layers
    :param f: the number of filters for each layer
    :param s: the size of each filter
    :param l: the
    :return None
    """
    for _ in c:
        l.append(layers.Conv2D(f, kernel_size=(s, s), activation="relu"))
        l.append(layers.MaxPooling2D(pool_size=0.5))
        l.append(layers.Dropout(0.5))


def build_model(input_shape, n_classes, config):
    """
    Builds the model given a configuration.
    :param input_shape: the input shape for the model
    :param n_classes: the output shape for the model
    :param config: the configuration to use when building the model
    :return: the Keras model
    """
    l = [keras.Input(shape=input_shape)]
    generate_conv_layers(
        config['N_conv_layers'],
        config['N_conv_filters'],
        config['conv_filter_size'],
        l)
    l.append(layers.Flatten()),
    generate_dense_layers(
        config['N_dense_layers'],
        config['N_dense_units'],
        l),
    l.append(layers.Dropout(0.5)),
    l.append(layers.Dense(n_classes, activation="softmax"))

    return keras.Sequential(l)


def main():
    m_baseline = model((28, 28, 1), 10)


if __name__ == '__main__':
    main()
