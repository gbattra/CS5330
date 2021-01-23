### Setup

- OS: Ubuntu Desktop 20.04
- IDE: VS Code

### OpenCV Installation

Follow the "Detailed Installation" instructions at [OpenCV Installation for Linux](https://docs.opencv.org/4.5.1/d7/d9f/tutorial_linux_install.html)

- **Note**: Make sure to have the packages `libgtk2.0-dev` and `pkg-config` installed prior to running `cmake ../opencv && sudo make install`:

### Build

1. In the root directory, run: `cmake . && make`