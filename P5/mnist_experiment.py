#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program experiments with different variations of the network architecture for a model
trained on the MNIST Fashion dataset. As a baseline, the original model used for the MNIST Numbers
dataset is used.
"""

import numpy as np
import matplotlib.pyplot as plt

from tensorflow import keras
from tensorflow.keras import layers
from mnist_train import model


N_conv_layers = [2, 3, 4]
N_conv_filters = [32, 64, 128]
N_dense_layers = [2, 3, 4]
N_dense_units = [50, 100, 400]
conv_filter_size = [3, 5]


def generate_config(c, f, s, d, u):
    """
    Generates a configuration object given the indices for each config
    param.
    :param c: number of conv layers
    :param f: number of conv filters
    :param s: conv filter size
    :param d: number of dense layers
    :param u: number of dense units
    :return: a config dictionary
    """
    config = {
        'N_conv_layers': c,
        'N_conv_filters': f,
        'N_dense_layers': d,
        'N_dense_units': u,
        'conv_filter_size': s
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
    for _ in range(d):
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
    for _ in range(c):
        l.append(layers.Conv2D(f, kernel_size=(s, s), activation="relu"))

    l.append(layers.MaxPooling2D(pool_size=(2, 2)))
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

    m = keras.Sequential(l)
    m.summary()
    return m


def load_data():
    """
    Loads and formats the MNIST Fashion dataset for training.
    :return: the formatted train and test datasets
    """
    (x_train, y_train), (x_test, y_test) = keras.datasets.fashion_mnist.load_data()

    x_train = x_train.astype("float32") / 255
    x_test = x_test.astype("float32") / 255
    x_train = np.expand_dims(x_train, -1)
    x_test = np.expand_dims(x_test, -1)

    y_train = keras.utils.to_categorical(y_train, num_classes=10)
    y_test = keras.utils.to_categorical(y_test, num_classes=10)

    return (x_train, y_train), (x_test, y_test)


def evaluate_configurations(configurations, x_train, y_train, x_test, y_test):
    """
    Fits the model for each config to the training data and returns a list of the history
    for each.
    :param configurations: the configs to use when building the model
    :param x_train: training inputs
    :param y_train: training labels
    :param x_test: test inputs
    :param y_test: test labels
    :return: the histories list
    """
    histories = []
    for config in configurations:
        m = build_model((28, 28, 1), 10, config)
        m.compile(loss="categorical_crossentropy", optimizer="adam", metrics=["accuracy"])
        hist = m.fit(x_train, y_train, batch_size=128, epochs=10, validation_split=0.3)
        score = m.evaluate(x_test, y_test)
        histories.append({
            'config': config,
            'history': hist,
            'score': score
        })
    return histories


def plot_history(title, hist, config=None):
    """
    Plots the training history of the model.
    :param title: the title for the plot
    :param hist: the history of the training
    :param config: the config used for the model
    :return: None
    """
    txt = f"No. Conv Layers: {config['N_conv_layers']}\n" \
          f"No. Conv Filters: {config['N_conv_filters']}\n" \
          f"Conv Filter Size: {config['conv_filter_size']}\n" \
          f"No. Dense Layers: {config['N_dense_layers']}\n" \
          f"No. Dense Units: {config['N_dense_units']}\n" if config is not None else "Baseline History"
    plt.plot(hist.history["accuracy"])
    plt.plot(hist.history["val_accuracy"])
    plt.title(title)
    plt.ylabel("Accuracy")
    plt.xlabel("Epochs")
    plt.legend(["Train", "Test"], loc="upper left")
    plt.figtext(0.05, 0.05, txt, ha="left", fontsize=9, bbox={"facecolor": "orange", "alpha": 0.5, "pad": 5})
    plt.show()


def map_params_to_metrics(history, n_conv_l, n_fltr, fltr_size, n_dense_l, n_dense_u):
    """
    Maps the history accuracy to the config params dictionary.
    :param history: the history for the model
    :param n_conv_l: num conv layers
    :param n_fltr: num filters
    :param fltr_size: filter size
    :param n_dense_l: num dense layers
    :param n_dense_u: num dense units
    :return: None
    """
    score = history['score'][1]
    config = history['config']

    n_conv_l['value'].append(config['N_conv_layers'])
    n_fltr['value'].append(config['N_conv_filters'])
    fltr_size['value'].append(config['conv_filter_size'])
    n_dense_l['value'].append(config['N_dense_layers'])
    n_dense_u['value'].append(config['N_dense_units'])

    n_conv_l['acc'].append(score)
    n_fltr['acc'].append(score)
    fltr_size['acc'].append(score)
    n_dense_l['acc'].append(score)
    n_dense_u['acc'].append(score)


def plot_param_metric(param_metric, title, ax):
    """
    Plots the param metric in a scatter plot
    :param param_metric: the param metric to plot
    :param title: the title of the plot
    :param ax: the axis plot
    :return: None
    """
    r = np.random.random()
    b = np.random.random()
    g = np.random.random()
    color = (r, g, b)

    ax.set_title(title)
    ax.plot(param_metric['value'], param_metric['acc'], 'o', color=color)


def plot_param_metrics(histories):
    """
    Plots the accuracies of each param variation for each model trained.
    :param histories: the histories of metrics
    :return: None
    """
    n_conv_layers = {'value': [], 'acc': []}
    n_filters = {'value': [], 'acc': []}
    filter_size = {'value': [], 'acc': []}
    n_dense_layers = {'value': [], 'acc': []}
    n_dense_units = {'value': [], 'acc': []}
    [map_params_to_metrics(
        h,
        n_conv_layers,
        n_filters,
        filter_size,
        n_dense_layers,
        n_dense_units) for h in histories]

    fig, ax = plt.subplots(2, 3)
    fig.suptitle("Config Param Metrics")
    fig.delaxes(ax[1, 2])
    plot_param_metric(n_conv_layers, "No. conv layers", ax[0, 0])
    plot_param_metric(n_filters, "No. filters", ax[0, 1])
    plot_param_metric(filter_size, "Filter size", ax[0, 2])
    plot_param_metric(n_dense_layers, "No. dense layers", ax[1, 0])
    plot_param_metric(n_dense_units, "No. dense units", ax[1, 1])
    plt.show()


def plot_histories(histories):
    """
    Iterates through each history and plots the accuracy
    :param histories:
    :return: None
    """

    max_score = 0
    max_i = 0
    i = 0
    for history in histories:
        if history['score'][1] > max_score:
            max_score = history['score'][1]
            max_i = i
        i += 1
    best_model = histories[max_i]
    baseline = histories[0]

    plot_history("Baseline", baseline['history'])
    plot_history("Best Model", best_model['history'], best_model['config'])
    plot_param_metrics(histories)


def main():
    """
    Evaluates each model configuration against the baseline model (used for MNIST numbers dataset).
    :return: None
    """
    (x_train, y_train), (x_test, y_test) = load_data()

    m_baseline = model((28, 28, 1), 10)
    m_baseline.compile(loss="categorical_crossentropy", optimizer="adam", metrics=["accuracy"])
    baseline_history = m_baseline.fit(x_train, y_train, batch_size=128, epochs=10, validation_split=0.3)
    baseline_score = m_baseline.evaluate(x_test, y_test)

    histories = []
    histories.append({'config': None, 'history': baseline_history, 'score': baseline_score})

    configurations = build_configurations()
    histories = evaluate_configurations(configurations, x_train, y_train, x_test, y_test)

    plot_histories(histories)


if __name__ == '__main__':
    main()
