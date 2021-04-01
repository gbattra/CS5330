#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program reads the images from the /data/greek directory and saves their values and classifications
in two CSV files to the same directory.
"""

import numpy as np
import cv2
import os


def import_images():
    """
    Reads the images into a labeled list.
    :return: the images and their corresponding labels
    """
    folder = "data/greek/images"
    imgs = []
    categories = []
    lbls = []
    for filename in os.listdir(folder):
        img = cv2.imread(os.path.join(folder, filename))
        if img is not None:
            img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
            img = cv2.resize(img, (28, 28))
            img = (255 - img) / 255
            img = img.flatten()
            label, categories = get_label(filename, categories)
            imgs.append(img)
            lbls.append([label])

    return np.array(imgs), np.array(lbls)


def get_label(filename, categories):
    """
    Determines the file label given the filename and a list of known categories.
    :param filename: the name of the file to label
    :param categories: a list of known categories. if filename not present, it is added to the list.
    :return: the label and updated categories list
    """
    category = filename.split("_")[0]
    if category not in categories:
        categories.append(category)

    label = categories.index(category)

    return label, categories


if __name__ == '__main__':
    images, labels = import_images()
    np.savetxt("data/greek/pixels.csv", images, header="Pixels")
    np.savetxt("data/greek/labels.csv", labels, header="Labels")
    cv2.waitKey()
