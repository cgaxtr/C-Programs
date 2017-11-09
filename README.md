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

