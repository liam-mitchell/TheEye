Welcome to The Eye raytracer demo!

Usage: TheEye <scene file>
Example usage: ./TheEye scenes/scene1.txt

You can steer the camera using the arrow keys. On simple scenes this will go reasonably quickly.

To take a screenshot of the current display, press the '3' key at any time. The screenshots are saved to screenshot1.jpg ... screenshotN.jpg.

To test the depth-of-field feature, press the '2' key. This will change the rendering to use four depth-of-field rays in order to give you a real-time-ish idea of what the screenshot will look like. You can set the aperture width with the left and right keys, and the focal length with the up and down keys. When taking a screenshot in this mode, the depth-of-field ray count will be set to 64 for the screenshot. (Note that this can take some time - around 10-20 seconds for the scenes provided.)

To compile and test the code on your own machine, just go to the /src directory and invoke make, then invoke the TheEye program created in the directory.

The example screenshots were taken from scene1.txt (scene1.jpg), scene2.txt (scene2.jpg), trees.txt (trees.jpg), and scene3.txt (dofN.jpg). The four depth-of-field screenshots serve to illustrate different focal lengths and aperture widths.

-----------------------------

PARSING:

The scene description language parsed by the raytracer implements the following commands:

- # comments: This is a line of comments. Ignore any line starting with a #.
- Blank line: The input file can have blank lines that should be ignored.


- size width height: The size command must be the first command of the file, which controls the image size.
- maxdepth depth: The maximum depth (number of bounces) for a ray (default should be 5).
- output filename: The output file to which the image should be written. You can either require this to be specified in the input, or you can use a suitable default like stdout or raytrace.png (The final files should be .png, with the specific names the autograder requests).
- camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov: Specifies a camera, with necessary orientation vectors

- sphere x y z radius: Defines a sphere with a given position and radius.
- maxverts number: Defines a maximum number of vertices for later triangle specifications. It must be set before vertices are defined. (Your program may not need this; it is simply a convenience to allocate arrays accordingly. You can ignore this command [but still parse it] if you don't need it).
- vertex x y z: Defines a vertex at the given location. The vertex is put into a pile, starting to be numbered at 0.
- tri v1 v2 v3 Create a triangle out of the vertices involved (which have previously been specified with the vertex command). The vertices are assumed to be specified in counter-clockwise order. Your code should internally compute a face normal for this triangle.
- plane n0 n1 n2 v0 v1 v2 Create a plane using normal (n0, n1, n2) and point (v0, v1, v2).


- translate: x y z A translation 3-vector.
- rotate: x y z angle Rotate by angle (in degrees) about the given axis as in OpenGL.
- scale: x y z Scale by the corresponding amount in each axis (a non-uniform scaling).
- pushTransform: Push the current modeling transform on the stack as in OpenGL. You might want to do pushTransform immediately after setting the camera to preserve the identity transformation.
- popTransform: Pop the current transform from the stack as in OpenGL. The sequence of popTransform and pushTransform can be used if desired before every primitive to reset the transformation (assuming the initial camera transformation is on the stack as discussed above).


- directional x y z r g b: The direction to the light source, and the color, as in OpenGL.
- point x y z r g b: The location of a point source and the color, as in OpenGL.
- attenuation const linear quadratic: Sets the constant, linear and quadratic attenuations (default 1,0,0) as in OpenGL. By default there is no attenuation (the constant term is 1, linear and quadratic are 0; that's what we mean by 1,0,0).
- ambient r g b: The global ambient color to be added for each object (default is .2,.2,.2).


- diffuse r g b specifies the diffuse color of the surface.
- specular r g b specifies the specular color of the surface.
- shininess s specifies the shininess of the surface.
- emission r g b gives the emissive color of the surface.
