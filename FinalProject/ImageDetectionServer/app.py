# Greg Attra
# 04/12/2021

"""
This app serves object localization requests from the Unity AR labeling app. It's single endpoint takes a base64
encoded image and outputs a list of detected objects, their labels and bounding box dimensions.
"""

import detector

from flask import Flask
from flask import Response
from flask import request

app = Flask(__name__)


@app.route("/")
def process():
    """
    Process an image via the Google Vision API and returns the detected object labels
    and bounding boxes.
    :return: a list of object labels and their corresponding bounding box dimensions
    """
    content = request.args.get('image')
    objects = detector.detect(content)
    data = {'objects': objects}
    res = Response()
    res.data = data
    return res


