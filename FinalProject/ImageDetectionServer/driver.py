#!/usr/bin/env python

# Greg Attra
# 04/12/2021

"""
Executable for running the object detection system using a local image file and OpenCV. Mainly used for testing.
"""

import cv2
import detector


def process(frame):
    """
    Converts the image to base64 and runs it through the Google Vision API
    :param frame: the image to process
    :return: the detected objects
    """
    enc = cv2.imencode('.jpg', frame)[1].tobytes()
    objects = detector.detect(enc)
    return objects


def draw_annotations(obj, frame):
    """
    Draws the bounding boxes and labels of the detected object in the image.
    :param obj: the labels/bounding boxes to draw
    :param frame: the frame to draw on
    :return: the updated frame with drawn bounding boxes
    """
    rows = frame.shape[0]
    cols = frame.shape[1]
    pixel_coords = []
    for vertices in obj.bounding_poly.normalized_vertices:
        loc = (int(cols * vertices.x), int(rows * vertices.y))
        pixel_coords.append(loc)

    for p, pixel in enumerate(pixel_coords):
        start = pixel
        end = pixel_coords[p+1] if p + 1 < len(pixel_coords) else pixel_coords[0]
        frame = cv2.line(frame, start, end, (0, 255, 0), 1)

    frame = cv2.putText(
        frame,
        obj.name,
        (pixel_coords[0][0] + 5, pixel_coords[0][1] + 20),
        cv2.FONT_HERSHEY_SIMPLEX,
        0.75,
        (0, 0, 255),
        1)

    return frame


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
            objects = process(frame)
            for obj in objects:
                frame = draw_annotations(obj, frame)
                cv2.imshow('Boxes', frame)

    cap.release()
    cv2.destroyAllWindows()
    return


if __name__ == '__main__':
    main()
