## Setup

### Environment

- OS: Ubuntu Desktop 20.04
- IDE: VS Code

### Dependencies

- build-essentials
- opencv

### OpenCV Installation

Follow the "Detailed Process" instructions at [OpenCV Installation for Linux](https://docs.opencv.org/4.5.1/d7/d9f/tutorial_linux_install.html):
1. Run: `$ sudo apt install -y g++`
2. Run: `$ sudo apt install -y cmake`
3. Run: `$ sudo apt install make`
4. Clone the git repository from https://github.com/opencv/opencv.git
5. Run: `$ mkdir build && cd build`
6. Run: `$ cmake ../opencv`
7. Run: `$ make -j8`
8. Run: `$ sudo make install`
- **Note**: Make sure to have the packages `libgtk2.0-dev` and `pkg-config` installed prior to running `$ cmake ../opencv && sudo make install`:

### Build

1. In the project root directory, run: `$ cmake . && make`

## Program

### OR2D

The OR2D program is a 2D object recognition program which can either take an image or video stream as input.

Image mode: `$ ./OR2D -i <image_path>`

Video mode: `$ ./OR2D -v`

**Commands**

- `t` - Threshold the image. Will prompt user for threshold value.
- `d` - Segment the image. Will prompt user the max number of regions to segment.
- `f` - Compute features for each region in the image.
- `l` - Label the object in the image. Will prompt the user for the label name.
- `c` - Classify the regions in the image. To classify multiple objects simultaneously, press `d` and specify the max number of regions. Then press `c` to classify up to the max number of regions.

**Extensions**
- `k` - Classify regions using the K-nearest neighbor algorithm. Will prompt user for the `K` value to use (the number of neighbors).
- `u` - Segment an image with a 2-pass algorithm. Will prompt user for the max number of regions.
