
Scene Graph Refactoring
-----------------------

The scene graph consists of:
   - geometry
   - cameras
   - shots (used to be "effect")
       - have a camera
       - activate geometry in the scene graph
       - may render other shots as well (portals)

Audio events?

In addition to the scene graph, there are also screen-space effects (effect) with the following state:

   - input image(s)
   - output image
   - uniform parameters
   - a shader name


The scene graph might look like:

ROOT
 - Curves
    - GrassIntroCurve
    - RandomVariation
 - Geom
   - World
       - Stars
   - Country
       - GroundPlane
       - Grass
       - Pond
       - FireFlies
       - Trees
       - ...
   - City
       - Buildings
           ...
 - Shots 
   - GrassIntro
   - GrassReveal
   - WomanIntro
   - WomanReveal
   - KidIntro
   - KidReveal
   - Triptych
   - Credits

There is a single rasterizer that has the following state
   - screen space effects
       - a set of effects to apply to the shot
   - shot
       - the current shot to render
   - a default view port (the entire screen)



Objects
-------
Node:
    - name
    - shader name
    - children[]
    - args{}
    - parent
    - init()
    - update()
    - draw()

Camera(Node):
    - curve
    - vec4 center
    - vec4 up
    - near
    - far
    - fov
    - aspect

Shot(Node):
    - camera
    - duration
    - vector<string> Geometry




Shot: Grass Intro
    - Play audio
    - Screen black
    - Fade in/out cedits
    - Load country set
    - Start animating camera
    - Fade in, person walking through grass
    
    Scene: {
        - low blue light, 
        - person stops, 
        - looks up at the stars, 
        - sighs, 
    }

    Cut: 
        - switch to city scene, stars







