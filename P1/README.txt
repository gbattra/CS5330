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
- `s` - Saves the frame to an image file in `images/saved`.

Filter commands:

 (**Note:** Press `s` after applying a filter to save the image. Or press any key to continue.)

- `g` - **Grayscale**: Converts the image to grayscale.
- `b` - **Blur**: Blurs the image using a 5x5 (1x5 seperable) Gaussian filter.
- `c` - **Cartoon**: Converts the image into the style of a cartoon.
- `x` - **SobelX**: Applies a SobelX filter to the image.
- `y` - **SobelY**: Applies a SobelY filter to the image.
- `m` - **Magnitude**: Computes the gradient magnitude of the SobelX and SobelY filters of the image.
- `l` - **Quantize**: Blurs and quantizes the image.
- `n` - **Negative**: Produces a negative of the image.

Extensions:
- `o` - **Orientation**: Produces an orientation map of the graidents of the SobelX and SobelY filters of the image.
