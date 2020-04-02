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
