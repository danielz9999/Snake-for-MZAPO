snake
Application developed by Daniel Žampach and Petr Stanke
for the MicroZed based MZ_APO board during the 2022 summer semester.

The developement of this project is logged using git on GitLab
GitLab link: https://gitlab.fel.cvut.cz/stankpe4/mz_apo_zampach_stanke
The main branch for the project is "two-players"

Documentation is done through the wiki of the project on GitLab.

Quickstart guide:
To get the program working there are 2 to 3 things that need to be done:

1. Change target ip of the MZ_APO board in the MAKEFILE (line 13)
2. Change the path to the directory where the mzapo-root-key ssh key is located (line 26) (or use other means of authentication)
3. If the main menu on boot does not look as pictured on the wiki (does not display "PLAYERS?" with a 1 and 2 under) then navigate to mzapo_parlcd.c.
There on lines 17-18 you will find: 

//#define HX8357_B
#define ILI9481

Try commenting #define ILI9481 and uncommenting #define HX8357_B and then reboot, if that does not work try commenting both lines and then rebooting.