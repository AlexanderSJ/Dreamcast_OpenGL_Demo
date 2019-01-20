# Dreamcast OpenGL Demo
An OpenGL Demo running on Dreamcast hardware

# Dependencies
- KallistiOS
- [kazade's GLdc libaries](http://dcemulation.org/phpBB/viewtopic.php?f=29&t=104595)

# Building
1. After loading up the KallistiOS environment, navigate to the project's directory and run the command ```make```.
2. Run the command ```sh-elf-objcopy -R .stack -O binary main.elf output.bin``` to create the binary.
3. Run the command ```/opt/toolchains/dc/kos/utils/scramble/scramble output.bin 1st_read.bin```
4. Convert the binaries to CDI using BootDreams, or cdi4dc

# Running the Demo
There are two ways of running this demo:
1. Converting it to a CDI and running it through an emulator (I'd recommend "demul". NullDC has a weird flickering issue when using OpenGL)
2. Running it on hardware

To run it on hardware, you'll either [need to burn the CDI to a disc](https://www.wikihow.com/Burn-Dreamcast-Games, "How to Burn Dreamcast Games to a Disc"), or use a coder's cable and dcload to transfer over the binaries to the console.
