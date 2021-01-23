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

## Programs

### ImgDisplay

Usage: `$ ./ImgDisplay <image_filepath>`
- i.e. `$./ImgDisplay images/examples/cat.jpeg` - This will display a picture of a cat.

Press `q` to exit the program.

### VidDisplay

Usage: `$ ./VidDisplay`

This will open up a video stream, pausing at each frame and allowing the user to execute commands on that frame via keystrokes. Below are the available keystrokes:
- `s` - Saves the frame to an image file in `images/saved`
- `g` - Converts the image to grayscale. The user may press `s` to save this grayscale image. Otherwise press any key to continue.
