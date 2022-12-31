RAYTRACER IN C++ (WINDOWS ADAPTATION)
This project was created under Professor Fadaifard at Columbia University, Fall 2022 Computer Graphics Course.
Original RayTracing engine was developed for Linux & Mac, this is a refactoring for Windows.
Correcting inconsistencies between platforms is still in progress.

The Basics:
-Simple Raytracing engine
-Uses MonteCarlo RayTracing integration for area lights and soft shadows
-Imports obj files and create trimesh objects, (obj, off, etc)
-Reads image textures and applies to trimesh and implicit surfaces
-Handles Phong and Phong-Dielectric materials
-Utilizes mesh and scene-level bounding volume heirarchies with AABBs for performance improvement

*Uses scene file format as given by class instructor:

Here are the possible commands:

Comment:
/ Any line starting with / should be ignored

Geometry:
/ sphere at position x y z with radius r:
s x y z r

/ triangle with counterclockwise point order:
t x1 y1 z1 x2 y2 z2 x3 y3 z3

/ plane with normal n and scalar value d:
p nx ny nz d

/ triangle mesh
w meshpath

Camera:
/ camera at position [x y z] looking in direction [vx vy vz], with focal length d,
/ an image plane sized iw by ih (width, height) and number of pixels pw ph.
c x y z vx vy vz d iw ih pw ph

Lights: (note second parameter to denote which kind of light)
/ a point light at position x y z, color & intensity coded as r g b
l p x y z r g b
/ a directional light with direction vx vy vz and color & intensity coded as r g b
l d vx vy vz r g b
/ the ambient light (there will be, at most, only one of these):
l a r g b

Materials:
When a new material is defined, all subsequently-created geometry will be assigned that material.
/ Phong material with solid color:
/ defined by diffuse components [dr dg db], specular components
/ [sr sg sb], ideal specular components [ir ig ib], and with “roughness”
/ or Phong exponent “r”
m dr dg db sr sg sb r ir ig ib
/ textured Phong material:
/ defined by texture id “ti”, diffuse components [dr dg db], specular components
/ [sr sg sb], ideal specular components [ir ig ib], and with “roughness”
/ or Phong exponent “r”
/ (note: the texture referenced by this material must be defined first)
n ti dr dg db sr sg sb r ir ig ib
/ dielectric Phong material:
/ defined by index of refraction ior and attenuation components [dr dg db]
d ior dr dg db

Textures:
/ image texture with id “ti” and path “path”.
/ “flipx” and “flipy” are integer values in {0, 1}, specifying whether to flip the image
/ horizontally or vertically, respectively.
i ti flipx flipy path

Options:
/ for your own additions and coding, you may add option flags on a single line in this way
o myopt1 myopt2 etc.
(for example, you might want to be able to switch shadows on/off by including a “shadows” option.
Leaving the “shadows” option out will tell your renderer not to do the shadow computation, etc.)

***Sources:
Professor Fadaifard, basic organization of class structure, inheritance heirarchy, setup

https://stackoverflow.com/questions/341817/is-there-a-replacement-for-unistd-h-for-windows-visual-c

OpenMesh

OpenCV for Images

spdlog

https://github.com/tqdm/tqdm.cpp/blob/master/include/tqdm/tqdm.h

