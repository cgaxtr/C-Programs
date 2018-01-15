## Mytar
Mytar program allows you to package multiple files into a single file.

#### Compile
Run the command `make` to compile the program

#### Usage
To package files
`./Mytar -cf output_file.mtar inputFile1 inputFile2 inputFile3 ... `

To unpackage files
`./Mytar -xf file_to_unpackage.mtar`

## FUSE_myFS
FUSE_myFS allows you to create a Unix type file system using the FUSE library in a regular file. For that the file is divided in blocks with size 4KiB. The first block contains the superblock, the second block contains the bitmap, the next block contains the root directory, the next five blocks contains i-Nodes, and the rest blocks correspond to data blocks 

#### Compile
Run the command `make` to compile the program

#### Usage
Create directory mount-point
`mkdir mount-point`

And execute the program fs-fuse
`./fs-fuse -t 2097152 -a virtual-disk -f '-d -s mount-point`

##### Params
-t size of file system
-a regular file when we create our file system
-f FUSE library params

## Schedsim
Scheduler simulator, that read a file with tasks and E/S interruptions, and generate a file with output with time used for all tasks

### Compile
Run the command `make` to compile the program

#### Usage
`./schedsid ./schedsim -i <input-file> [options]`

##### Params
-h: Displays help message
-n <cpus>: Sets number of CPUs for the simulator (default 1)
-m <nsteps>: Sets the maximum number of simulation steps (default 50)
-s <scheduler>: Selects the scheduler for the simulation (default RR)
-d: Turns on debug mode (default OFF)
-p: Selects the preemptive version of SJF or PRIO (only if they are selected with -s)
-t <msecs>: Selects the tick delay for the simulator (default 250)
-q <quantum>: Set up the timeslice or quantum for the RR algorithm (default 3)
-l <period>: Set up the load balancing period (specified in simulation steps, default 5)
-L: List available scheduling algorithms

## Driver
kernel module, which allows the use of keyboard leds as a character device.
### Compile
To compile module, run the command  `make`
To compile led_user.c `gcc -o led_user led_user.c`
Now you need insert module into kernel with `sudo insmod chardev_leds.ko`
Then execute `dmesg | tail` to catch the information of kernel module as `sudo mknod -m 666 /dev/leds c 250 0`, now you can write into device as `sudo echo 123 > /dev/led`
