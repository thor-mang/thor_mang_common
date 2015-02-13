# thor_mang_common
Provides thor_mang related files required for both simulation and real robot use (analogous to pr2_common)


## Standalone installation


* Install wstool package

    ```
    sudo apt-get install python-wstool
    ```
    
* Create a catkin workspace if you don't already have one (we recommend a separate one for Baxter)

    ```
    mkdir -p ~/ros/ws_baxter/src
    cd ~/ros/ws_baxter/src
    wstool init .
    ```
* Pull repo into workspace and update
    ```
    wstool set --git thor_mang_common https://github.com/thor-mang/thor_mang_common.git
    wstool update
    ```
* Build workspace
    ```
    cd ..
    catkin_make
    ```
* Source workspace setup script
    ```
    source devel/setup.bash
    ```
    
## Usage

You can take a look at the thor_mang model by running
    ```
    roslaunch thor_mang_description xacrodisplay_thor_mang_tud.launch
    ```
