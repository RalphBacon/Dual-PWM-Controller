# Dual-PWM-Controller (See video #72 here : https://www.youtube.com/c/RalphBacon)
The Arduino can provide TWO totally independent PWM outputs that run regardless of what the microcontroller is busing doing.

PWM is easy to do on the Arduino, but flawed in its native implementation. Thank goodness we have a brilliant PWM library to help us out. Flawed? Help out? Why do we need helping out? Watch the video and find out!

Here's how to control TWO PWM motors independently with a single rotary potentiometer (resistor) and a switch to tell the Arduino which output you're currently controlling.

I also show you why the three independent timers on the Arduino are so useful but why you should never adjust the Timer Zero if you want your sketches to run without potentially weird results.

This is a personal project that implements several of the techniques I've described in my previous videos and demonstrates I really do what I say in those videos. It's not just theory, it's real life implementation. You might want to do something similar to control two motors on a robot arm, car, tank or something else.

The PWM library, the project (demo) sketch and anything else I can find that might be useful can all be found here on GitHub. Just click the "Clone or Download" button to download a zipped file which Windows or Mac will unzip for you.

Oh, there's a short clip in the video of me constructing this on my workbench - please forgive the total chaos there as I was in the middle of another project when I decided to complete this one. I'm ashamed of the mess shown there! But you do get to look at the "Strange Third Hand" unit which I'm finding invaluable when building projects like this.
