# Clone-nx
> Fork of nx-tas (https://github.com/hamhub7/nx-tas)

Clone-nx Gives You the ability to control More than one player with a single controller mainly with Super Smash Bros Ultimate in mind but yes you can use this in any multiplayer game.

## Support the Creators
Join our [Discord server](http://discord.gg/qeamCcV)  
Support Hamhub7 on [Patreon](https://www.patreon.com/aaronhh) or [PayPal](https://www.paypal.me/aaronhamhub)  
Make a pull request!  
Support Hamhub7 Clone-nx Is Free!

## Features
Attaching the amount of controllers you want to control with the number button.
Detaching controllers when your finish by pressing minus. Nothings is Limited Besides the amount of controllers which is max.

## Usage
First of all, you need to have an [Atmosphere-injected Switch](https://switch.homebrew.guide/).

### Installation
0. If you dont want to do it this way i suggest you do it my way go to 1B.

1A. Acquire CLONE-nx.nsp either from the latest release, or more preferrably build yourself via the instructions below

2A. Create the relevant title directory on your Switch's SD card at sdmc:/atmosphere/titles/0100000000000cab/ (sdmc:/atmosphere/contents/0100000000000cab on Atmosphere versions 0.10.0 or newer)

3A. Place the NSP as sdmc:/atmosphere/titles/0100000000000cab/exefs.nsp (sdmc:/atmosphere/contents/0100000000000cab/exefs.nsp on Atmosphere versions 0.10.0 or newer)

4A. Create an empty file at sdmc:/atmosphere/titles/0100000000000cab/flags/boot2.flag (sdmc:/atmosphere/contents/0100000000000cab//flags/boot2.flag on Atmosphere versions 0.10.0 or newer)

5A. Reboot into CFW

1B. Go to the latest release at https://github.com/Expertsonicgamer32x/CLONE-nx/releases and download the archive.

2B. Copy the atomsphere folder to the root of you sd card.

3B. Reboot into cfw

### Keybinds
You need to have a USB keyboard attached to your Switch, either via the dock or a USB-A to USB-C adapter in the charging port or wirelessly. Dont worry it works in handheld too.

#### Controllers
If on keyboard
- `1` = Adds a Virtual Pro Controller you can control with player 1 or in handheld mode if theres no player 1. 
- `-` = Remove the most recently added controller.
If on Full Controller(Means no single joy cons)
- `ZL+L+DpadRight+Y` = Adds a Virtual Pro Controller you can control with player 1 or in handheld mode if theres no player 1. 
- `ZR+R+DpadUp+X` = Remove the most recently added controller.

## Building from Source
1. [Set up the required libraries](https://switchbrew.org/wiki/Setting_up_Development_Environment)
2. In the newly installed MSYS2, navigate to the folder you'd like to build this project in, either by opening MSYS in that directory or using the `cd` command to navigate
3. `git clone https://github.com/Expertsonicgamer32x/CLONE-nx.git`
4. `cd Clone-nx`
5. `make`
6. You should see a file called `Clone-nx.nsp`

The `make` process will fail for one of three reasons:
- You cloned a commit that doesn't compile (unlikely)
- Your folder is named something other than `Clone-nx`; solution is to `ln -s Clone-nx.json config.json`
- Your directory's full path has spaces in it (usually your account name)

## Upcoming Features
1. A UI. maybe GUI if possible im still learning.
2. The ability to change the speed of the virtual controllers.
3. A axis editor.
4. The option to use a controller combination if you dont have access to a keyboard at all times mainly for handheld mode in mind.(FINISHED)


