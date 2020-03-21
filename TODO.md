# TO DO TODO

* Make this legit Modern C++ by guessing and hoping for the best 
    * Currently this is more C than Modern C++
    * The goal of this project is to learn Modern C++ I think
* Make the collision better
    * Based on the side of the collision, the vector should change
        * T/B: {  1, -1 }
        * L/R: { -1,  1 }
* Angle of the bounce should be depend on a few factors 
    * Where did the ball hit the paddle 
        * Currently tracking a very ditry and terrible position of collision
        * 0.0f - 1.0f
    * Was the paddle moving and for how long?
        * This would allow "power" hits
* Sprite sheets
    * sprite.sheet
        * filename
        * sprite atlas description: 
            : x1: 100, x2: 200, y1: 0, y2: 100
* Introduce GameState
    * Main Menu State
        * Options Screen
    * Game State
        * Pause State 
    * Game Over State 
        * Some Sick Credits State
* Text Rendering (-_-)
    * Perhaps the fonts should be rendered to one large, square texture
        * Will SDL optimize this to send it to the GPU only once?