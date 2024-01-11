# spvr2png
Image converter from Sega Dreamcast PVR format to PNG

# Description

This is a simple command line tool to convert Sega Dreamcast PVR images to PNG format.

## Usage

```
spvr2png -s [input.pvr] -d [output.png]
```

Where:

- `input.pvr` is the input PVR file to convert 
- `output.png` is the name of the output PNG file


```
spvr2png -s [input_dir] -d [output_dir]
```

Where:

- `input_dir` is the input directory with PVR images
- `output_dir` is the name of the output directory where all converted PNGs will be saved

## Building

Use next commands to build project:

```
make all
```

This will produce the `spvr2png` executable.

## License

This project is licensed under the MIT License. See LICENSE for details.

