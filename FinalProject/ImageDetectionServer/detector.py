# Greg Attra
# 04/12/2021

"""
This object interfaces with the model, passing in the image data and returning the predicted labels
and bounding box dimensions.
"""

from google.cloud import vision


def detect(content):
    """
    Passes the base64 encoded image through the Google Vision API and returns the detected
    object labels and bounding box dimensions.
    :param content: the base64 encoded image
    :return: the detected labels and bounding box dimensions
    """
    client = vision.ImageAnnotatorClient()
    image = vision.Image(content=content)
    objects = client.object_localization(image=image).localized_object_annotations

    print('Number of objects found: {}'.format(len(objects)))
    for object_ in objects:
        print('\n{} (confidence: {})'.format(object_.name, object_.score))
        print('Normalized bounding polygon vertices: ')
        for vertex in object_.bounding_poly.normalized_vertices:
            print(' - ({}, {})'.format(vertex.x, vertex.y))
    return objects
