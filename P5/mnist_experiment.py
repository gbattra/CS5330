#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This program experiments with different variations of the network architecture for a model
trained on the MNIST Fashion dataset. As a baseline, the original model used for the MNIST Numbers
dataset is used.
"""

from mnist_train import model


def build_model():

    return


def main():
    m_baseline = model((28, 28, 1), 10)


if __name__ == '__main__':
    main()
