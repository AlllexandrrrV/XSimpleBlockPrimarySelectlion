# XSimpleBlockPrimarySelectlion
This program is the simplest way disables pasting from the X-server clipboard, which hangs on pressing the middle mouse button.

# Building

make

# Using for example

mv xsimpleblockprimaryselectlion /home/username/local/bin/

create file /home/username/local/bin/xsimpleblockprimaryselectlion.sh

----------------------------------------------------------------------

#!/bin/bash

/home/username/local/bin/xsimpleblockprimaryselectlion 2>/dev/null &

----------------------------------------------------------------------

chmod + x /home/username/local/bin/xsimpleblockprimaryselectlion.sh

And add this script to the "Scripts" subsection in the 
"Automatically launch applications" section in the KDE settings.
