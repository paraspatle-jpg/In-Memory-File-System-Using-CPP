# In_memory_file_system

## How to run

    Visual Studio 2022 should be installed, with Desktop development with C++ tools.
    Clone the Repository.
    Click on the In_memory_file_system.sln file, to open the project.
    Click to on f5 to run the project.

## Documentation
### Important Note
All the paths must be relative to current directory, absolute path will not work.
### mkdir - Create a New Directory

Description:
Creates a new directory in the current or specified path.

    mkdir <directory_name>

### cd - Change Directory

Description:
Changes the current directory to the specified path. Supports navigating to the parent directory using ".." and moving to the root directory using "/" or to a specified absolute path.

    cd <path>

### ls - List Contents of a Directory

Description:
Lists the contents of the current or specified directory.

    ls [<directory_path>]

### touch - Create a New Empty File

Description:
Creates a new empty file in the current directory.

    touch <filename>

### echo - Write Text to a File

Description:
Writes the specified text to a file. There should be spaces around '>' for the command to work.

    echo '<text>' > <filename>

### cat - Display Contents of a File

Description:
Displays the contents of the specified file.

    cat <filename>

### mv - Move a File or Directory

Description:
Moves a file or directory from the source path to the destination path.


    mv <source_path> <destination_path>

### cp - Copy a File or Directory

Description:
Copies a file or directory from the source path to the destination path.

    cp <source_path> <destination_path>

### rm - Remove a File or Directory

Description:
Removes a file or directory from the current or specified path.

    rm <path>
    

  
