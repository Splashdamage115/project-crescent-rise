# Issues Encountered will be in this Doc

## Early stages (render pipeline and before)

1. creation of the ECS model mimicing, by using arrays of functions
    - Every game object contains an array of scripts,
    - the scripts range from simple movers to models
    - Every game object will be comprised of multiple scripts
    - scripts all inherit from a base scriptable object class

2. Update function has been abstracted into a different class that contains an array of the different functions.
    - Update has an array of update functions, calls all update functions
    - (easier to implement early / late update later, easier to add functions to update)

3. Render pipeline crash due to reinstatiation of VAO, VBO, Fragment Shader & Vertex Shader
    - moved all render pipeline to own classes

4. Overhauled all rendering pipeline objects to class model, also now using scripts in objects
    - all pieces in the render pipeline (so far) are self contained in scripts

5. Fragment Shader and vertex Shader storage system, specific formatting for easy understanding
    - simple function call to get shader, storage of shader is preffered, as it makes rendering faster
    - shader formatting :
    - ![Breakdown of how the shaders are formatted](.\MarkdownImgs\ShaderFormat.png "ShaderFormatImage")

6. GameObject pool is self contained and calls all functions necessary
    - Game Object pool contains all world game objects (can be used to reinstantiate a scene quickly)

7. added the mathmatical functionality to move objects across the screen efficiently
    - changed movement to be based on the forward quternion vector

8. changed the vertex shader to account for movement
    - changed the vertex shader to account for camera movement in 3d space

9. Created an encapsulated input system that holds the key down events for keys
    - Vector of key presses, asign true if the button is pressed down

10. created a mouse input system to track mouse delta
    - save the x and y of the mouse position

11. issue with reading the escape to exit the mouse from the screen (fixed)
    - reading of the button inputs for non standard A-Z numerals was incorrect

12. allowing clicking back into the window
    - saved mouse down events

13. camera movement
    - made it so that you can use WASD to move (Q and E are used for up down movement)
    - ![Breakdown of how the camera position works](.\MarkdownImgs\CameraPassing.png "CameraPassingLayout")

## Ground Creation

1. grouund tile ebo usage, keeps breaking, reverted back to no ebo
    - now using, kind of, still using vertex connections, but not specifically using the vertices,
    - This is because the vertex shader UV coordinates

2. worked on random displacement for floor mesh
    - Changed Y value of floors
    - Will change to use nois later


3. linking of vertices in the floor mesh
    - quite tedious as need a way to recall previous coordinates, and iterate through them easily
    - changed to use x y deformation that is pre calculated
    - ![General floor mesh linking plan](.\MarkdownImgs\MeshLinking.png "mesh linking plan")

4. Render order issues
    - hard to understand what the render order wrapping was, had to redo the order and the UV coordinated Multiple times for the floor

5. ReInstantiation issue
    - ONE AMPERSAND HAS COST ME 3 DAYS

## sphere

1. creation of cube sphere
    - this in itself took me an insane amount of time with figuring out how to create a 3d sphere
    - researchon which method to use
    - research onmost efficient way tocreate it

2. noise
    - creation / finding of noise maps for 3d space, my methods were not sufficient, used online library instead

3. noise variations
    - changing of the noise to more align with myuse case, added different ways to edit the noise

4. editor
    - usage of IMGUI for editing of variables
    - using 2 main menus one for  coloour one for noise layerss
    - abstraction of all noise classes into own datastructures which mnake sense and are all tangible

5. colouring
    - creation of shader that allows for the colouring of the terrain based on heights from center of planet, extensive errors with shaders had tobe rectified
        including errors with min and max , naming conventions endedupbeing biggest error
    - addding ionto the editor for different height variations and on the fly editing

6. chat system /  multiplayer
    - outlined more in other docs, mainly worked on text being rendered to screen using external libraries
    - online server and client based interactions

7. persistence between multiple players
    - outlined in other docs

8. LOD failures
    - trying to add LOD to the planet, this has been error after error of it not working
    - this has been temporarily abandoned as I am unable to implement it now

9. textured cube
    - made a custom cubve with textures, lighting, and height maps

10. sky box
    - moved through mappingto box, to cubesphere, multiple iterations
    - added high and low def

## Make it pretty


1. vornoi noise

    - added noise deformation to the objects surface
    - added different layers of noise
    - added way of adding more noise layers as needed to the object
    - added customisation for the  noise mapping
    - added GUI for the deformation

2. Sky Box

    - using Cubemap for the sky box
    - customisable sky box, just needs correct name
    - added quality control in code for high and low quality

3. Water Textures

    - fresnel water displacement
    - layered fresnel noise for water mesh
    - overhaul water to seperate object
    - water mesh noise mapping

4. ground mesh texturing

    - added texture mapping based on height
    - texture mapping with noise mapping
    - customisation in menu
    - correct height adjustments
    - feather

5. map genertation size increase

    - increase the size of the planet

6. movement on the mesh

    - surface attachement script
    - surface movement from world space
    - surface alignment on local up


## Research:
    - Rock climbing game, with surface (snowboarding?)

## -
- document noise / cube sphere - links, reasoning


## Links
- cube sphere implementation documentation
- https://songho.ca/opengl/gl_sphere.html
- Cube sphere has been used to make the process of joining faces easier, and so that uv coordinates are even across faces, as oposed to the ico sphere where faces are not even
    Also gives a higher level of control of the vertices of the asset

- Noise Map generation
- https://github.com/SaintGimp/LibNoise
- this noise has been converted from c# to c++
    used to keep terrain changes consistent


## Plan

# Priority Order List : 

<ul>
    <li> 🔴 - surface instancing</li>
    <li> 🔴 - grass instancing</li>
    <li> 🔴 - building collisions</li>
    <li> 🔴 - object with collision instancing</li>
    <li> 🔴 - enemy on surface</li>
    <li> 🔴 - attacking enemy</li>
    <li> 🔴 - enemy attack player</li>
    <li> 🔴 - player death</li>
    <li> 🔴 - main menu screen</li>
    <li> 🔴 - player Weapon</li>
</ul>

## Completed

### phase 1
#### ✅ - vornoi noise
#### ✅ - Sky Box
#### ✅ - Water Textures
#### ✅ - ground mesh texturing
#### ✅ - map genertation size increase
#### ✅ - angle map to be spherical
#### ✅ - ground tile texturing
#### ✅ - movement on the mesh

### phase 2
#### ✅ - load planet from json
#### ✅ - save planet to json
#### ✅ - load random from json

<br><br><br>
  </em></p>
    <h6><em>**Progress indicators**</em>
     <ul>
       <li>🔴 - Not done</li>
       <li>🟡 - In progress / Planned</li>
       <li>🟠 - Return to this</li>
       <li>✅ - Completed</li>
       <li>🔷 - Under Review</li>
       <li>⬜ - Add comments</li>
       <li>🐛 - Bug!</li>
     </ul>
    </h6>
  </em>
  <em>
  <h6>**Note : Work on these in no particular order, but mark them off as completed</h6><br>
  <h6>**Note : Mark the next task to be worked on / current item</h6>
  </em>



  ## Videos

- video demoing surface instancing and terrain reading random values based off presets
    visible in video, 3d green bottles whcih are read in from fbx files
    bottles instanced on planet surface
    world changes on each load
  (Surface Instancing)[https://setuo365-my.sharepoint.com/personal/c00283152_setu_ie/_layouts/15/stream.aspx?id=%2Fpersonal%2Fc00283152%5Fsetu%5Fie%2FDocuments%2FProject%2F2026%2D01%2D21%20SurfaceInstancing%20surfaceLoading%2Emp4&referrer=StreamWebApp%2EWeb&referrerScenario=AddressBarCopied%2Eview%2Ee5881e58%2D63c3%2D4145%2D926a%2D112de3e674e1]