#!/usr/bin/env python

# Greg Attra
# 03/28/2021

"""
This executable uses the pretrained model to classify new images from the /data dir.
"""

from tensorflow import keras


def import_data():
    """
    Reads the images from the /data dir into 28x28 grayscale arrays for the Keras model.
    :return: a tuple of labels and images in the proper format for the Keras model
    """

    return keras.preprocessing.image_dataset_from_directory(
        "data",
        labels="inferred",
        label_mode="categorical",
        color_mode="grayscale",
        batch_size=10,
        image_size=(28, 28))


def main():
    """
    Runs the images through the classifier and predicts the labels for each.
    :return 0 if success, -1 otherwise
    """
    data = import_data()
    i = iter(data)
    x, y = next(i)
    x /= 255

    m = keras.models.load_model("model")
    score = m.evaluate(x, y)

    print("Test loss: ", score[0])
    print("Test acc: ", score[1])

    return 0


if __name__ == "__main__":
    main()
