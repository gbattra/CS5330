#!/usr/bin/env python

# Greg Attra
# 04/12/2021

"""
Executable for running the object detection system using a local image file and OpenCV.
"""

import numpy as np
import cv2
import detector
import base64


def process(frame):
    enc = cv2.imencode('.jpg', frame)[1].tobytes()
    objects = detector.detect(enc)
    return None


# credit: https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_gui/py_video_display/py_video_display.html
def main():
    """
    Driver of the detector application. Mainly used for testing.
    :return: 0 success, 1 error
    """
    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        cv2.imshow('Video', frame)

        key = cv2.waitKey(10)
        if key == ord('q'):
            break
        if key == ord('p'):
            process(frame)

    cap.release()
    cv2.destroyAllWindows()
    return


if __name__ == '__main__':
    main()
