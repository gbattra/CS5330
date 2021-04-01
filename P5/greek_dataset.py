#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program reads the images from the /data/greek directory and saves their values and classifications
in two CSV files to the same directory.
"""

import glob
import numpy as np
import cv2
import os


def import_images(folder):
    """
    Reads the images into a labeled list.
    :return: the images and their corresponding labels
    """
    imgs = []
    categories = []
    lbls = []

    filenames = glob.glob(folder.rstrip('/') + "/*.png")
    filenames.sort()
    data = [cv2.imread(img) for img in filenames]

    for i, img in enumerate(data):
        img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
        img = cv2.resize(img, (28, 28))
        img = (255 - img) / 255
        img = img.flatten()
        label, categories = get_label(filenames[i], categories)
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


def main():
    """
    Imports the image data, generates labels, and saves the data to two CSV files.
    :return: None
    """
    images, labels = import_images("data/greek/images")
    np.savetxt("data/greek/pixels.csv", images, header="Pixels")
    np.savetxt("data/greek/labels.csv", labels, header="Labels")
    cv2.waitKey()


if __name__ == '__main__':
    main()
