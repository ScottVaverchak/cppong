# TO DO TODO

* Introduce a 9slice texture for the game border 
    * Currently renders
    * Should use entity system 
* Make the collision better
    * Angle is based on where the ball hits the paddle 
    * Collision with the wall inverts the Y-axis 
    * Physics dont exist in my world, dude.
* Sprite sheets
    * sprite.sheet
        * filename
        * sprite atlas description: 
            : x1: 100, x2: 200, y1: 0, y2: 100
* Introduce GameState
    * Main Menu State
        * MainMenuState
        * OptionsScreen
    * Game State
        * GameState
        * PauseState 
    * Game Over State 
        * SomeSickCreditsState
* Text Rendering (-_-)
    * Perhaps the fonts should be rendered to one large, square texture
        * Will SDL optimize this to send it to the GPU only once?
* Make this legit Modern C++ by guessing and hoping for the best 
    * Book has been ordered 😎
    * Currently this is more C than Modern C++
    * The goal of this project is to learn Modern C++ I think
* Switch to OpenGL for rendering 
