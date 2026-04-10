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

## Open GL

The first few weeks of my project was learning how to use Open GL.
This is because i never really worked in it properlly, this means I had no idea what any of the functions do, how they work, and why theywork the way they do
this means I researched ll the functions in the basic template and played around with how they work

after this I planned thestructure of my game

## Structure

One thing i dont like about basic game libraries (such as sfml)
is how every function needs to somehow pass down functions
for instance the render function in sfml
a standard structure may have:

    Game ->  Player -> weaponManager -> weapon -> bullet

and this is just one simple structure
each of those above need to pass down a render function (for instance, in a standard structure)
this means that Game passes render to player, which passes render to weapon manager etc.
this is especially bad when something like weapon manager doesnt render anything
weapon manager may only hold the weapon (which is rendered), but then weapon manager still needs to implement a render function, even though it doesnt render anythinbg itself
I dont like this
so i invested a lot of time in creating abstractions for update for instance
and i also created abstract script classes which are in abstract game objects

## Game objects

I wanted the game objects to function similar to how they work in engines like Unity (one game object, contains scripts)
This evolved later to also contain things like tags, for quickly getting the objects of cewrtain tags
The game object needed a few specific simple things
scripts, transform, and some functions
each script needed to implement a start, update, render, and send message function
the send message was to try to ressemble how unity handles objects calling functions in other game objects in a decoupled way
    this ended up looking like such
        Game Object 1 needs to call another game objects function (will use player shoot enemy as example)
        ```md
                player gets all enemies (through tags)
                            \/
                player checks for transform hit scan on enemy
                            \/
                player hits enemy
                sends message of hit, with damage amount ~ explained next
                            \/
                enemy health controller recieves hit
                            \/
                enemy health cahnges / dies
            ```

the send message script also often needs to recieve information,
this was implemented through the use of std::any
std::any is a c++ library function which allows for any parameter type (I use this instead of typenames and such for simplicityand faster compilation)

a vector of an abstract script is central to the design I have
This idea mimics how it works in different game engines like Unity
My structure allows for the implementation of update functions, or not if it isnt necessary by the script itself


so each game object looks like this
```md
GameObject
    |
    |
    |- Array of tags
    |
    |- array of scripts
    |
    |- Transform
```

I have abstracted the calling of all the functions to be in a GameObjects class
this uses a pool of game objects that are also abstracted away
so that the abstract script class is just appended to the game object, and called

```md
Game objects
    |
    |
    |- Array of GameObjects
    |
    |- Exposed functions to add new game object
    |
    |- exposed functions to get game objects (eg getAllOfTag([tag]))
```

These abstractions make it easy to compose game objects in future

## input

There is no good way to track inputs, and key presses in Open Gl
so I ended up making my own abstract version of the input system, I tried to mirror this to be similar to how it is done in other game engines
This ended up being a kind of amalgamation of how Unity handles key inputs in the new input system,
And how sfml / raylib handles key inputs, with just a ping if KeyDown is true
I also had to add a mouse button tracker, which i decided to abstract by adding a way to quickly assign a button press to an array, and it is then handled and called as needed 
so you can just append an abstract function, and assign a mouse key, and it will be called (single line)

## Planet

The main center point of my project is the planet
What i wanted for the planet is for it to be a fully explorable sphere
the sphere to have things on it and for enemies to spawn on it that you can shoot
then you proceed to the next planet after defeating a boss
So the first thin i needed was the noise to deform the planet
I originally started by making a ico sphere and trying to apply noise to that
This failed for one main reason. ICO spheres use subdivisions
this means that after instantiation and subdivisions, subdivisions after 3 / 4 lag the game tremendously
so after all the research and work that was put in to make the ico sphere, i had to scrap it because it was too slow
(this was around 2 weeks of work that was scrapped due to lag)
so i did more research, then i stumbled on the cube sphere, which allows for very fine control of the vertex count on the sphere, and didnt have the issue of a normal sphere where the poles would distort movement

so i got to work on the sphere
after creating it and hooking up the mesh i tried to implement the noise function i had for the lflat ground tile
this obviously didnt work so i proceeded to look up alternatives, I found a noise class available online which allows for the deformation to be even around the surface
specifically allowing specific points to always be the same height, this got rid of the need to also remeber previous postions as the deformation at specific locations is always the same
this fixed the lag issue

I then needed the mesh to be coloured
I used a simple height function, that looks for the highest point, and the lowest point, this is my 0 and 1
then every height in between can be of different colours eg 0 - 0.3 sand, 0.3 - 0.7 grass, 0.7 - 1.0 rock (mountains)
this looked very good so i ended up keeping it, i added small blending factor between layers and that was the colours

Later I converted the colours to also accept textures at the certain heights

## planet editor

I contemplated how to make the planet generation more interesting and came up with the idea of making a planet editor
this proceeded and ended up taking roughly 3 weeks to fully implement correctly and reliably. 
the editor needed to edit the planet colour, and the noise settings (for now)
I also needed a way to open and close this menu, and i didnt want to make it into a keybind (too basic in my opinion)
so i created the chat system, this is very similar to many other games
I also needed to track commands that are called and the arguements that thgey recieve
I decided to then abstract this and move it to a seperate static class.
This was to increase speed of development in future, so all i would need to do is add a single line to add a new command to the command interpreter.
This included the /GUI command which opens the world editor
(later added /godmode, / changename, /clear (clear chat), /connect /reconnect, etc, this was very easy to implement thanks to the abstractions)

## Save / load

Json parser
I thought of making a manual json parser or parsing the planet into a simple text file,
however after a quick bit of research io found nhlomanns json parser for c++
I decided to use it to speed up the process of creating new planets a bit

1. load planet from json
2. save planet to json
3. load random from json

these are the main things that were implemented, this also had to get added to the planet editor for ease of use and creation
after this implementation it meant that the creation of new planets could be done in the matter of minutes, and then saved for the user
the user then loading a random planet to play on, making every play through unique

## shader store

I added an abstract for shaders, I noticed every shader had similar code for instantiation and after learning that each shader needed to be reset for each object (different variables for objects, but use same shader)
I realised that the shader can be very easily abstracted to a different file
so i ended up making the shader store which stores all shader pairs (fragment, and vertex shader)
this meant that allthe instantiation of the shaders could be abstracted from the normal files. Making them much simpler to read

## model store

Loading of models from FBX files is very complicated due to the encryption of .fbx, and the storage being weird
this left me a few options, using a premade library, or making my own
i used the ufbx library which is by far the lightest one

![Comparison of fbx times](.\MarkdownImgs\fbx-parser-times.png "ufbx is the fastest by far, and fbx sdk is the slowest")

This allowed me to load fbx files, but i still had to parse them manually into my render object
after figuring all that out, i finally had models in my game

I then proceeded to make a model store, as it seemed that reloading the same model more than once caused errors, and crashes
this means that every model is loaded and cached allowing for fast retrieval

## texture store

Texture store functions similar to the fbxstore where it stores the data for each image
this makes it easy for the code to retrieve the same image for multiple different objects

## particles

Particle pool and animated textures
this uses a similar idea to the previous where all the code is abstracted away, and the spawning of particles only takes 1 single line
this works well and allows for variants, and such to be used by the code, this can be seen in the muzzle flash, where differnt flashes are used for the muzzle

## enemy abstract state

I used a simple command pattern to abstract the enemy pattern and to keep the code more simple
this works similar to a finite state machine
an abstract enemy state is given to the enemy on instantiation
the enemy then uses this abstract state to move around, attack, idle, die etc

This allows for the fast creation of new enemies with unique states, so this was chosen as the way to handle enemies for that sole reason

this took a while to implement however as there were errors with the manager, and different errors with the enemy movement along the surface

## model partner script

when grass instancing was created (explained next) there was an issue.
each instance needed a seperate model
however this was super inneficient and laggy
so i tried to replicate the flyweight pattern using my own implementation (flyweight designed by myself, but after research i realised the code mimics fly weight)
this functions similarly, and works very well and alleviates lag
the model partner script exists to set object specific parameters
this being positions, the model is a single instance and the partner script changes the position and such
this is because the model can be heavy to instantiate on computation, this design worked and alleviated the lag, allowing for more instances for almost free (outside of cost of instancing itself)

## surface instancing

Surface instancing took a long time to figure out, this is because the data for the planet isnt really stored anywhere outside of the vbo, 
this is okay for the planet rendering but to place an instance onthe mesh would require a lot of work for the pc to sample all vertices
so i found another way of doing it by sampling the noise from the original noise class
this worked very well and allowed me to disperse objects across the mesh randomly

## weapons

weapon creation is also abstracted to a high level
allowing for very fast creation of new weapons,
the only things that are needed are the data (full auto, mag size, reload time etc.) and a gun model & texture
then by simply adding to the array of weapons, it is added as a useable weapon
this makes it very simple to make new weapons, and makes it very easy to make a weapon editor (if it were saved to json and such)
this was done for future expansion, but has not yet been implemented.

## interaction

using the tag system i have added an interaction method, this simply uses a hit sphere and ray calculation
after that it sends a pick up method (caught by a script) for it to destroy, this is easily expandable thanks to the sendMessage function,
It allows for the creation of new items that can be on the ground / interacted withy, this is currently used for just collecting rocks,
and has not been expanded much past that, this is due to lack of time, I would in future use this for opening doors, picking up new weapons
etc.

## enemy

the enemy who uses the enemy state machine has an interesting attack pattern, this was created the way it was thanks to testing
now it highlights white and has a blood splatter particle whenever it is shot, this clearly shows hit effect

the slime enemy has an interesting attack (tested for fun sake)
when it gets close it enters the attack state
it stands still for 0.7 seconds (most balanced time)
it then turns red 0.1 seconds before charging, if the player doesnt move outside of radius in that 0.1 seconds (rewarding for very fast reaction players)
it charges at the player, this charge lasts 1 second (so it is possible to avoid if you are fast to sprint out of the way)
and this damages the player if it hits the player (red during entire charge attack)
it then waits 3 seconds stationary before it tries to charge again

this sounds complicated, but is very normal when the player is playing, and very clear for the user
it also rewards players that are payting attention (to the red glow)
it is also fair as the player has some time to get away after aavoiding / getting hit


## main menu

the main menu was specifically made to be simple and hopefully if needed expandable
I used ImGui becasue I already knew how it worked and had it implemented elsewhere in the repo
This also meant I didnt have to overcomplicate the code because it is instanced, and updates live
this all meant that the main menu could be reliable anduseable

## screen overlay

The screen overlay is used for 3 things, taking damage overlay (red flash)
fade in and out of the level
and underwater overlay
the underwater overlay is there to mimic how other games handle underwater, making it more believable as it is harder to see underwater
I used a simple quad that is displayed in the game view, and overlays over everything, this gave the effect I wanted 
(I wanted it to be clearly visible when you are damaged)

## future plans


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
    <li> 🔴 - Weapon hud info</li>
    <li> ✅ - Weapon variants</li>
    <li> ✅ - foliage improvements</li>
    <li> ✅ - enemy variants</li>
    <li> ✅ - enemy state machine</li>
    <li> ✅ - enemy idle</li>
    <li> ✅ - enemy wander</li>
    <li> ✅ - enemy follow player</li>
    <li> ✅ - enemy pathfinding</li>
    <li> ✅ - weapon animation (shooting)</li>
    <li> ✅ - weapon reload animations</li>
    <li> ✅ - weapon hit effect</li>
    <li> ✅ - weapon shoot particle effect</li>
    <li> ✅ - object pick up </li>
    <li> ✅ - enemy attack player</li>
    <li> ✅ - player death</li>
    <li> ✅ - main menu screen</li>
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

### phase 3
#### ✅ - player Weapon
#### ✅ - attacking enemy
#### ✅ - enemy on surface
#### ✅ - grass instancing
#### ✅ - surface instancing


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


  - Video of grass instancing on planet surface
    also visible, distance culling, trees instancing, and rock instancing
    (trees need orientation fixes)
  (Grass Instancing)[https://setuo365-my.sharepoint.com/:v:/g/personal/c00283152_setu_ie/IQCzBRn4ryxbRp1zUIiUIIdpAamoiizAmQmZiyVDSb__JR4?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=hiBhH6]