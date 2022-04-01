# streumon_fps
project for streumon application

Source repartition :
Aim assist present in the character file.

Wallrun present in the character and the movement component.

Scoreboard present in PlayerController and gamemode.

Scoreboard UI in the UI file named Scoreboard.

Ability UI in the UI file named Ability.

(// Debug purpose comments are present where debug draw can be uncommented).

How it work :
Wallrun is automatic when in air and near a wall.
Aim assist is like call of duty.

Controls :
Press 1 for switching wallrun enable.
Press 2 for switching wallrun enable.
( keys are displayed inder the UI)

UI :
Ability icon on the top.
Scoreboard on the top left.

Weird things :
No wallrun animation.
Wallrun logic only look at the hip of the player on the right and the left, would be better with more detection.
Wallrun does not stop if against a wall (90°).
Aim assist has a minimum range to avoid having the camera turning too fast.
When entering wallrun, the mumentum is kept until leaving the wallrun, I forget to reset it.
I also forget to replicate the projectile... (so it is called from client to server and not directly on the server).
I did not initialized and update scoreboard with new players (is supposed to be only 2 lines...) so it only display when someone is markking points.

Cool things :
Evrything is commented. (I forget to explain the architecture).
Wallrun jump.
Wallrun work with angled walls.
Smooth aim assist.

Also included :
Programmer art.
Ultra customisable parameters.
Everyone has the same name, so you can play among us at the same time.
