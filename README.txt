/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: Contains information on compiling and using the game
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 10, 2020
- End Header --------------------------------------------------------*/

# Opening
Open `./Game Engine/GameEngine.vcxproj` using Visual Studio.

# Compiling
A clean rebuild may be required.
Build the solution and run the Local Windows Debugger in `Release` mode.

# Controls
WASD or Arrow Keys - Move
Space - Jump
Ctrl - Dash in your currently-input direction, 0.5s cooldown
Left Mouse - Slash.  Hold down for continual slashing.
Right Mouse - Cheater's Cleaver.  Sets HP to max and deals massive damage.
F - Toggle Debug
Right Arrow - Move to Next Level

# Events
Direct - When the player is hit by the sniper or augmentor, they are directly contacted and moved to x position 30.
Broadcast - The augmentor occcasionally moves the y position of all units on screen.
Delayed Broadcast - Standing within the green telegraph will 1 second later cause knockback (which therefore means it can happen many frames in a row, lingering while out of the zone)
Subscription - When the player is hit, the updown unit grows its model size.
