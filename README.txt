Resistance: Retribution — Infected Mode EU v1.0
================================================

Purpose
-------
Enables the genuine Infected Mode directly on a PSP, without connecting to a PS3.

Verified configuration
----------------------
Game: Resistance: Retribution
Region/build: European release, UCES-01184
Hardware tested: PSP-1000
CFW tested: ARK-4

Confirmed Infected Mode effects
-------------------------------
- Yellow health bar
- Automatic health regeneration
- Infected appearance/eyes
- "Infected" Intel category becomes visible
- Infected-specific voice line

Safety guard
------------
The plugin checks four exact MIPS instructions before writing anything. If the
loaded game does not match the verified EU code, it silently refuses to patch.
It does not alter the ISO or save file.

Build
-----
Run these commands in this folder using a configured PSPDEV/PSPSDK environment:

    make clean
    make

Output:

    resistance_retribution_infected_eu.prx

ARK-4 installation
-------------------
1. Copy the PRX to:

       ms0:/SEPLUGINS/resistance_retribution_infected_eu.prx

2. Enable it as a GAME plugin through ARK-4's plugin manager, or add the
   equivalent GAME entry used by your ARK-4 setup.

3. Disable any earlier Resistance test or diagnostic plugins.

4. Restart the PSP, launch Resistance: Retribution, and load a level.

Removal
-------
Disable or delete the plugin. The game and save remain unmodified.

Compatibility
-------------
This release is specifically guarded for the tested European UCES-01184 build.
It is not intended for US, Japanese, modified, or differently patched builds.

Credits
-------
Reverse engineering, plugin development and testing: TheDooberdude & ChatGPT
Hardware testing and validation: TheDooberdude
Original game: Bend Studio / Sony Computer Entertainment

Version history
---------------
v1.0 — First clean release after successful PSP-1000/ARK-4 verification.
