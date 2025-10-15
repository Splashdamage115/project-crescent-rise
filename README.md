# Issues Encountered will be in this Doc

## Early stages (render pipeline and before)

- creation of the ECS model mimicing, by using arrays of functions
- Update function has been abstracted into a different class that contains an array of the different functions.
- Render pipeline crash due to reinstatiation of VAO, VBO, Fragment Shader & Vertex Shader
- Overhauled all rendering pipeline objects to class model, also now using scripts in objects
- Fragment Shader and vertex Shader storage system, specific formatting for easy understanding
- GameObject pool is self contained and calls all functions necessary
- added the mathmatical functionality to move objects across the screen efficiently
- changed the vertex shader to account for movement
- Created an encapsulated input system that holds the key down events for keys
- created a mouse input system to track mouse delta
- issue with reading the escape to exit the mouse from the screen (fixed)
- allowing clicking back into the window
- grouund tile ebo usage, keeps breaking, reverted back to no ebo
- worked on random displacement for floor mesh
- linking of vertices in the floor mesh
