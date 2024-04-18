# Shared Memory Buffer Addon

This is a Node.js addon for working with shared memory using C++ and Boost libraries.

## MSVS Build Prerequisites

Before building the addon, make sure you have set up Boost. You can download Boost from [here](https://www.boost.org/). After downloading, follow these steps:

1. Set the `BOOST_ROOT` environment variable to the directory where Boost is installed.

2. Open the x64 Native Tools Command Prompt for VS 2022.

3. Navigate to the Boost directory using the command prompt:
    ```sh
    cd C:\Projects\shm-addon\libraries\boost_1_85_0
    ```

4. Run the bootstrap script to prepare Boost for building:
    ```sh
    bootstrap.bat
    ```

5. Build Boost libraries with the following command:
    ```sh
    b2 --build-type=complete
    ```

## Building the Addon

After setting up Boost, you can build the Node.js addon using node-gyp. Run the following commands:

```sh
node-gyp configure
node-gyp build
