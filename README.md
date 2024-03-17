# Banana Split

Command-line application to quickly make small, simple edits to image files.

Supported transformations:
- Precision cropping
- Division into multiple equally-sized images
- Rotation
- Lossy compression

## How to Use

1. If you use Linux, install SFML's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:
    ```
    sudo apt update
    sudo apt install \
        libxrandr-dev \
        libxcursor-dev \
        libudev-dev \
        libfreetype-dev \
        libopenal-dev \
        libflac-dev \
        libvorbis-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev
    ```
2. Clone project into desired local directory
3. Build cmake
    For Linux & macOS:
    ```
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```

    For Windows:
    ```
    cmake -S . -B build
    cmake --build build --config Release
    ```
4. Run the BananaSplit executable found in the build/bin folder

5. Enjoy!
