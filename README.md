# Project Descriptions for Team cc2524-fz246-js3296

## cc2524
(Submitted by group member)
## fz246
## js3296
 

Potential Ideas: We listed a few potential ideas here in case one of the ideas has already been implemented in the past.

 

Doodle Jump

* The goal of the game (doodle jump) is to get the avatar to jump as high as possible, from platform to platform. The screen would constantly be moving down at a certain speed such that if the avatar falls off the screen or doesn’t jump fast enough, the user loses. Users can control the avatar using the slider from the board. The screen would update the avatar’s position accordingly. 

* Users can alter the speed at which the screen moves downward. The faster/higher the speed, the faster the screen would move down and the harder the game is. Users can alter the speed by clicking on a button on the board where each press would increase the speed by a certain amount. 

 

Brick Breaker/Arkanoid

* In this game, the user controls a board to make sure that a ball bounces around on the screen (it doesn’t fall off the bottom of the screen). As the ball bounces, it will hit the bricks on the screen, such that each hit will cause the brick to break a bit. After a certain number of hits, the brick will break. The goal is for the user to break all the bricks. The user can use the slider to control how the board moves.  The screen would update the avatar’s position accordingly. 

 

Dodger/Catcher

* In this game, certain items would move/fall down the screen. The user controls an avatar and has to avoid/catch certain items to increase their score. Once the user reaches a certain score, they win. The user can use the slider to control how the avatar moves.  The screen would update the avatar’s position accordingly. 

* Users can alter the speed at which the screen moves downward. The faster/higher the speed, the faster the screen would move down and the harder the game is. Users can alter the speed by clicking on a button on the board where each press would increase the speed by a certain amount. 
 

Snood / Bust-a-Move

* The top of the board is filled with different colored bubbles. The player must shoot bubbles from the bottom to form a set of three bubbles of the same color in a row to make them disappear. The ceiling lowers over time. The player wins if the board is cleared of bubbles, and loses if the bubbles reach the bottom.

 
# Feedback

There are lots of "brick breaker" games this time and seeing some variety would be great. All of these would work and seem to be quite similar in terms of what would run on the board. In general, we are looking for some subsantial part of the code to run on the embedded system, so you need to do more than just sending button press events to the computer to control the game. The slider can be somewhat tircky to get to work well, so doning some singal processing on that front and sending the commands back would certainly be complex enough.  Also, since you have LEDs on the board, you could display some game state information on them. For example, you could blink for a new level or indicate the speed of the game with a frequency. 

In terms of priority, make the game-play fun, do as much as possible on the board. All these games are reasonable, and I would pick the one that would be the most fun to work on while requiring the least amount of development on the PC side. 


# Project Web-Page

The project web-pages will also be hosted on github in this repo in the "page" branch. You can edit it by switching branches and modifying the files, or by pushing to the branch. Here is a link to a minmal web-page that you can edit and modify: [https://pages.github.coecis.cornell.edu/ece3140-sp2024/cc2524-fz246-js3296](https://pages.github.coecis.cornell.edu/ece3140-sp2024/cc2524-fz246-js3296)
