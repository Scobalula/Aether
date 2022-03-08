# Aether

Aether is an environment map converter that supports multiple conversion modes such as cube maps, octahedrons, and equirectangular. Aether was made mostly due to my own need to have a single command line tool for offline conversion that also supports HDR floating point formats and EXR for direct conversion to Call of Duty: Black Ops III.

# Usage

Aether's usage depends mostly on the mode being used, the base command to kick things off is:

```
Aether --mode=*mode short hand* [mode options]
```

More modes, options, and features are being developed based off use case and my own time.

Aether currently supports the following modes with the following options for each mode:

## Cube Array To Equirectangular (cubearray2equ)

This mode takes in a DDS cube map file with an array of 6 images and converts it to a single equirectangular image.

The following options are available:

* **--input**: The input image file path. (Required)
* **--output**: The file path of where to save the equirectangular image to. (Required)

Example:

```
Aether --mode=cubearray2equ --input="input.dds" --output="output.exr"
```

## Hemi-Octahedron To Equirectangular (hemiocta2equ)

This mode takes in a single image file with 2 hemi-octahedron enviroment maps side by side, the upper and lower of the octahedron, and converts it to a single equirectangular image.

The following options are available:

* **--input**: The input image file path. (Required)
* **--output**: The file path of where to save the equirectangular image to. (Required)

Example:

```
Aether --mode=hemiocta2equ --input="input.dds" --output="output.exr"
```

# Building 

To start off you'll need Visual Studio 2022 with C++. Aether also depends on DirectXTex and OpenEXR, these can be gotten from vcpkg with the following commands:

```
vcpkg install directxtex:x64-windows-static
vcpkg remove openexr:x64-windows-static
```

Once that is done you should be able to build it no problem in Visual Studio, if you run into issues feel free to open an issue as it'll help me identify issues with the documentation that could be made clearer for people or to identify parts left out.

# License

Aether is licensed under the General Public License 3.0. You can use it under the terms of the GPL 3.0, but it comes with no warranty, please refer to the license file for more information.

# Feedback

This is my first time working with DirectX and serves not only as a utility for me and others to use but a learning exercise for myself. Ultimately that also means some parts may be incorrectly done, may have bugs, or other minor issues. Please feel free to open an issue if you found a bug or want to provide feedback. 💕

# Credits/References

Libraries:

* Icon: [https://www.flaticon.com/free-icon/cloudy_2272194](https://www.flaticon.com/free-icon/cloudy_2272194?term=sky&page=1&position=5&page=1&position=5&related_id=2272194&origin=search)
* DirectXTex: [https://github.com/microsoft/DirectXTex](https://github.com/microsoft/DirectXTex)
* OpenEXR: [https://github.com/AcademySoftwareFoundation/openexr](https://github.com/AcademySoftwareFoundation/openexr)
* Argh: [https://github.com/adishavit/argh](https://github.com/adishavit/argh)

Information:

* [https://gpuopen.com/learn/fetching-from-cubes-and-octahedrons/](https://gpuopen.com/learn/fetching-from-cubes-and-octahedrons/)
* [https://jcgt.org/published/0003/02/01/paper.pdf](https://jcgt.org/published/0003/02/01/paper.pdf)
* [https://www.semanticscholar.org/paper/Octahedron-Environment-Maps-Engelhardt-Dachsbacher/fcb9a6dbdf7b4c31f94e481cf101c83b73ea6410?p2df](https://www.semanticscholar.org/paper/Octahedron-Environment-Maps-Engelhardt-Dachsbacher/fcb9a6dbdf7b4c31f94e481cf101c83b73ea6410?p2df)
* [https://www.researchgate.net/publication/317107172_A_Sampling-Agnostic_Software_Framework_for_Converting_Between_Texture_Map_Representations_of_Virtual_Environments](https://www.researchgate.net/publication/317107172_A_Sampling-Agnostic_Software_Framework_for_Converting_Between_Texture_Map_Representations_of_Virtual_Environments)

* Test Files:

* [https://polyhaven.com/hdris](https://polyhaven.com/hdris)
* [https://www.humus.name/index.php?page=Textures&start=0](https://www.humus.name/index.php?page=Textures&start=0)

