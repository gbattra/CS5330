#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program reads the greek dataset embeddings from the generated CSV file and computes the SSD for
a sample from each category with all other samples.
"""

import numpy as np

from greek_embeddings import truncated_model, read_data
from greek_dataset import import_images


def sum_squared_distance(img, imgs, labels, m):
    """
    Computes the sum of squared distances between the target image and all other images. Returns a list
    of each SSD for each sample.
    :param img: the target image
    :param imgs: the image dataset
    :param labels: the corresponding labels for each image
    :param m: the model to compute the embedding
    :return: a list of the 27 SSD values for the target image
    """
    e = m(img).numpy()
    p = m(imgs).numpy()
    ssd = np.sqrt(np.sum((p - e) ** 2, axis=1))
    labeled_ssd = np.hstack((
        np.expand_dims(labels, axis=0).transpose(),
        np.expand_dims(ssd, axis=0).transpose()))

    return labeled_ssd


def evaluate_ssd(model, data, labels):
    """
    Computes the SSD for a sample from each category against all samples in the data set and prints
    the results.
    :param model: the model to compute the embeddings
    :param data: the dataset to run on
    :param labels: the labels for the data
    :return: None
    """
    i_alpha = np.where(labels == 0)[0][0]
    i_beta = np.where(labels == 1)[0][0]
    i_gamma = np.where(labels == 2)[0][0]

    alpha_ssd = sum_squared_distance(data[i_alpha:i_alpha + 1], data, labels, model)
    beta_ssd = sum_squared_distance(data[i_beta:i_beta + 1], data, labels, model)
    gamma_ssd = sum_squared_distance(data[i_gamma:i_gamma + 1], data, labels, model)

    print(f'Alpha SSDs (sample {i_alpha}):\n{alpha_ssd}\n')
    print(f'Beta SSDs (sample {i_beta}):\n{beta_ssd}\n')
    print(f'Gamma SSDs (sample {i_gamma}):\n{gamma_ssd}\n')


def main():
    """
    Computes the SSD between a sample from each category and all other samples in the dataset, printing
    the results to the console. The program then computes the SSDs for custom handwritten samples for each
    symbol in the dataset and compares the results against those from the original dataset.
    :return: None
    """
    m = truncated_model()
    x, y = read_data()
    evaluate_ssd(m, x, y)

    print("Custom Images:\n")
    custom_x, custom_y = import_images("data/greek/images/custom")
    custom_x = custom_x.reshape((6, 28, 28))
    evaluate_ssd(m, custom_x, np.squeeze(custom_y, axis=1))


if __name__ == '__main__':
    main()
