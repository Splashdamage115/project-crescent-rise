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

## Research:
    - Rock climbing game, with surface (snowboarding?)

## Plan

# Priority Order List : 

### ✅ - vornoi noise
### 🔴 - Sky Box
### 🔴 - Water Textures
### 🔴 - ground mesh texturing
### 🔴 - map genertation size increase
### 🔴 - angle map to be spherical
### 🔴 - ground tile texturing
### 🔴 - movement on the mesh

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