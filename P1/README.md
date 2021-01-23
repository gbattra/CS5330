### Setup

- OS: Ubuntu Desktop 20.04
- IDE: VS Code

### OpenCV Installation

Follow the "Detailed Process" instructions at [OpenCV Installation for Linux](https://docs.opencv.org/4.5.1/d7/d9f/tutorial_linux_install.html)
1. Run `sudo apt install -y g++`
2. Run `sudo apt install -y cmake`
3. Run `sudo apt install make`
4. Clone the git repository from https://github.com/opencv/opencv.git
5. `mkdir build && cd build`
6. Run `cmake ../opencv`
7. Run `make -j8`
8. Run `sudo make install`
- **Note**: Make sure to have the packages `libgtk2.0-dev` and `pkg-config` installed prior to running `cmake ../opencv && sudo make install`:

### Build

1. In the project root directory, run: `cmake . && make`