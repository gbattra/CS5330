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

### Image Dataset Download

**Note**: only do this if the current `images/db` directory is empty or does not exist.

The image dataset used can be found here: https://northeastern.instructure.com/courses/76659/files/7800323?wrap=1

1. Create the directory `images/db` at the root of the project
2. Download the image dataset and extract the images into this directory

### Build

1. In the project root directory, run: `$ cmake . && make`

## Programs

### ImgSearch

Usage: `$ ./ImgSearch <target image path> <database path> <feature type> <metric type> <count>`
- i.e. `$ ./ImgSearch images/db/pic.1016.jpg images/db square9x9 sumSquaredDistance 10`

The target image will be displayed. Press any key to continue. After a few moments, the top results will appear,
with Result 0 being the target image itself.

**Commands**
- **Part 1**: `$ ./ImgSearch images/db/pic.1016.jpg images/db square9x9 sumSquaredDistance 10`
- **Part 2**: `$ ./ImgSearch images/db/pic.0164.jpg images/db redGreenBlue intersection 10`
- **Part 3**: `$ ./ImgSearch images/db/pic.0135.jpg images/db multi rgRgb 10`
- **Part 4**: `$ ./ImgSearch images/db/pic.0535.jpg images/db colorTexture rgGms 10`
- **Part 5**: `$ ./ImgSearch images/db/pic.1070.jpg images/db lawsRg lawsRg 10` - **WARNING: SLOW**

**Extensions**

- **Sliding Laws Filter**: `$ ./ImgSearch images/db/pic.1070.jpg images/db slidingLawsRg lawsRg 10` - **WARNING: INCREDIBLY SLOW**