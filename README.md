# Issues Encountered will be in this Doc

## Early stages (render pipeline and before)

- creation of the ECS model mimicing, by using arrays of functions
- Update function has been abstracted into a different class that contains an array of the different functions.
- Render pipeline crash due to reinstatiation of VAO, VBO, Fragment Shader & Vertex Shader
- Overhauled all rendering pipeline objects to class model, also now using scripts in objects
- Fragment Shader and vertex Shader storage system, specific formatting for easy understanding
- GameObject pool is self contained and calls all functions necessary