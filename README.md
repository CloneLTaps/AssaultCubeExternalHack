# AssaultCubeExternalHack 
[AssaultCube](https://assault.cubers.net/) is a free fps game without any form of anti-cheat, perfect for learning game hacking! This project is still in development
however, it still has a functioning box esp allowing you to see players through walls. Blue boxed players represent friendly’s while red boxed players represent enemies.
This project also queries info such as ammo, health, and armor from memory but as of now its not used. This project is intended to be used against bots and not in a 
multiplayer setting against real non consenting players.  

# How it works
This hack is developed for the most update to date version of AssaultCube (AC) specifically for windows machines. It first uses a LPCSTR (String) containing “AssaultCube”
to locate the opened window. Then using the FindWindow() function we can grab the HWND (hardware id) which we can pass into the GetWindowThreadProcessId() function to
get AssaultCube’s process id. We can also pass the hardware id into the GetWindowRect() function to gain information about AC's windows dimensions.

Next, we can pass in the pid into the OpenProcess() function to receive a HANDLE that has read process memory permissions. Next we create a transparent overlay that is
layered on top of AC’s window. Then we use the SetTimer() function to set how often we want our boxes to be redrawn. For example I set it to 16 which means every 16ms 
the SetTimer() function will call the TimerProc() CALLBACK function which calls the InvalidateRect() function. InvlidateRect notifies windows that the screen needs to 
be redrawn. 

The redrawing occurs by windows calling the WndProc() CALLBACK function. Inside this we provide a WM_PAINT case which calls functions from my Addresses.c file. These will
then use the memory addresses to query and update all of our players and the other entities struct info. The entity info is allocated on the heap, so we need to remember
to deallocate the previous entity values first. Lastly, it will call my esp() function which uses entities view matrices to perform matrix math in order to change 3d 
coordinates into 2d so that boxes can be drawn correctly over each entities position relative to you. 

![image](https://user-images.githubusercontent.com/83735831/211214403-a957870a-ae7a-4814-8a67-1293a73e259a.PNG)


# How to obtain persistent memory addresses
We all know that ram stands for Random-Access-Memory but the side effect of this means that all of our in game values will be assigned to different memory addresses
each time we load the game up. This obviously causes issues since we don’t want to reverse every memory address each time we load up AssaultCube. To fix this we need to
first find our games base address. This is a memory address that our program will boot to this is thanks to [Virtual Addresses](https://yourtechdiet.com/blogs/virtual-address-vs-physical-address-understanding-the-difference/).
Once that is found we will need to find the offset to where our specific data is stored. The combination of the base address and the offset yields a memory address 
that often contains a pointer to where our data is stored especially for larger blocks of data like our player object. Now we will always be able to get our player
object since adding the base address with the offset will provide us with a persistent memory address that always contains a pointer to where our object is stored.
Keep in mind the pointer will likely point to a different memory address each time you load up AssaultCube but that is fine since the player object will always be
structured the same.

## How to find our player object 
First download [CheatEngine](https://github.com/cheat-engine/cheat-engine) you can choose to use their releases or you can do what I did and follow the basic build 
instructions to compile it your self using Lazarus which is a Pascal IDE. Once downloaded you can select a process whose memory you want to read. Now if you want to figure 
out where to find your rifles bullet count you would first type your current count into Cheat Engines “Value” box. Then click “First Scan”. Next shoot a bullet and type
the updated value into the “Value” section before then clicking “Next Scan”. You should be left with a few options. Try changing the values till you find which one 
actually changes your bullet count. For me the correct address is 0x006B06B8 which is what I will use to try and find the player object.

![image](https://user-images.githubusercontent.com/83735831/211215751-7619f1ab-215a-49cb-b1ff-d8f4277cfc79.png)


Now that we have the current address of our ammo we need to try and find the player object that contains this address. To do this right click your address then click
"Find what accesses this address". Once the new gui opens up start shooting your gun then stop the scan. You should get around 4 results. We can now compare how many
times we shot to the count which should narrow it down to 2 options. The other 2 are most likely used to update our screens which is why its being called so many 
times. 

![image](https://user-images.githubusercontent.com/83735831/211219592-3075ccaa-f66d-42bb-a32c-9973d73d6ba4.png)


Next, click on the cmp “compare two operands” assembly call then click “Show disassembler”. Now this may take a while to get use to but one suggestion it to try and
find calls coming from memory addresses that are similar to our ammos address. After looking around a bit I found the following image. Here we can see that ecx is 
equal to the pointer of [ac_client.exe + 18AC00]. Furthermore, ecx is equal to 0x006B0578 which is only offset 0x140 from our ammos address which we know to be 
0x006B06B8.

![image](https://user-images.githubusercontent.com/83735831/211220130-db3e414a-f8c7-4b5b-b6a5-af5bc14609dc.png) ![image](https://user-images.githubusercontent.com/83735831/211220087-c99caa89-5345-4203-8a29-56b709b76e61.png)


This makes me believe that 0x006B0578 is the memory address of where our player object starts and 0x140 represents the relative offset from the start of the player
object to the address of where our guns ammo is stored. If this is true, that means that “ac_client.exe + 18AC00” is our base address and persistent offset to our 
player object. Now clicking on the "Opcode" inside of Memory Viewer will bring up “cmp ecx,[0058AC00]” where 0x0058AC00 represents ac_client.exe plus 0x18AC00. Now
subtracting  0x18AC00 from 0x0058AC00 gives us 0x400000 which is our base address and is also equal to ac_client.exe

To verify that this is our player object click “Tools” then “Dissect data/structure”. Then add 0x0058AC00 into the box then click “Structures” then “Define new 
structure”. The first option should contain a pointer to 0x006B0578. Clicking on that pointer should bring up our player object. Now scroll down to an offset of 0x140
and shoot your gun to verify that your bullet count goes down. 

![image](https://user-images.githubusercontent.com/83735831/211220862-22619fa8-9e66-46ef-87e0-a80c017cb5e2.png) ![image](https://user-images.githubusercontent.com/83735831/211220909-9d67b41f-ec09-4081-a3af-0392455e9995.png)


