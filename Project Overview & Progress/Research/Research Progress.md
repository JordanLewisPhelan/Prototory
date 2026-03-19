
#### ***Note 1: Written on 7/11/25***
Research has led me down upon some new Strategy Patterns that may apply!

So far research has been my main focus - that and applying it. I have been trying to form a Modular architecture system. So that things can go wrong and won't break the game.
This has began by the Scene Manager I made initially to control the well active scene, menu-gameplay, etc., this is the first development as I began trying to add Settings to it but realized I will need them in the Gameplay scene too not just as  a main menu option. 

So research reasoning and result time! 
Settings will be a Modal module, the idea being it will be re-usable but will apply differently in certain contexts, e.g in Main Menu scene it will be a "New Scene" where it will have a blank background and options, this is so it is a focus and contained, but in Gameplay it will only dim or blur the background(still ironing out what i'll do with it exactly) so that the game is still there just a backdrop, this will allow players to stop and think about options in game and also allow them to click further options, such as actual Settings or quitting the game.


Why did this become an idea? Well i wanted some dynamism with my options. This spiralled into wanting to investigate how I could do this and so came up Modal operations - Which spiralled into because I was investigating and tinkering with(in current project with Buttons) callbacks and lambdas. I wanted to allow for 1 button component, to be reusable and customizable, so using callbacks and applying a lambda to it allows me to call the lambda function i generate when creating the scene! Currently this is all i done, i effectively used those buttons to traverse the scenes and I am yet to implement as of writing; the Settings Modal. 

This has Been a Notice to explain what happened research wise - Trying to organise scenes, I investigated a lot of random dead ends. Modal application of re-usable options will prevent needing to make a new element for each scene or even recreate them if i set this up correctly.
To traverse them I investigated Lambdas & callbacks and have tried to apply them; there could be a better way but this way is customizable and extensible, which will be crucial if I go to make more. 


 #### ***Note 2: Written on 19/3/26***
Note taking for research has stagnated due to time - Overall anything important will be shown some work process logic or elements through [[References]] to prevent a complete flow loss of Research Reporting for the project.