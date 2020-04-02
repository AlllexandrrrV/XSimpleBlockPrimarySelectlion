/*
    Special thanks to the author of the project 
    https://github.com/milaq/XMousePasteBlock. 
    Its code is taken as the basis of this project.
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

static Display *display;
const int NANOSEC = 333333333;

int main(int argc, const char* argv[]) {
   struct timespec tw = {0, NANOSEC};
   struct timespec tr;
   display = XOpenDisplay(NULL);
   if (display == NULL) {
      printf("ERROR: %s\n","Failed to connect to the X server");
      return 1;
   }
   while(1){
      XSetSelectionOwner(display, XA_PRIMARY, None, CurrentTime);
      XSync(display, False);
      nanosleep (&tw, &tr);
   }

   return 0;
}
