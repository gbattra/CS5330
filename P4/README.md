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

### AR

The `AR` program runs in two modes: `Calibration` and `Projection` modes.

`Calibration` mode should be used to calibrate the camera and save the intrinsic
parameters to a file.

`Projection` mode should be used to load intrinsic params from the saved file and project
objects onto the checkerboard using those params.

**Usage**

`Calibration` mode:<br>
```
$ ./AR -c
```

`Projection` mode:<br>
```
$./AR -p
```


**Commands**

`Calibration` mode:<br>
- `s` - Saves the checkerboard corners and 3D world points for the current image
- `c` - Calibrates the camera given at least `5` saved samples created using the `s` command
- `w` - Writes the calibrated intrinsic parameters to `files/params`
- `l` - Loads the saved intrinsic parameters from `files/params`

`Projection` mode:<br>
- `p` - Projects the 3D axis of the checkerboard onto the checkerboard. This is the default behavior.
- `o` - Projects a 3D object onto the checkerboard.

### Harris Corners

The Harris Corners program uses the Harris equation to find strong corners for feature tracking.

**Usage**

```
$ ./HarrisCorners
```