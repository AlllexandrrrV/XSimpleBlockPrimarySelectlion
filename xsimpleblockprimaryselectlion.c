/*
 *   Special thanks to the authors of the project
 *   https://github.com/baskerville/sxhkd.git,
 *   which was taken as a basis. 
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_event.h>

const int debug =  0;

const int true = 1;
static Display *display;


xcb_connection_t *dpy;
xcb_screen_t *screen;
xcb_generic_error_t *err;
xcb_generic_event_t *evt;
xcb_button_press_event_t *e;
xcb_window_t root;
int fd;
fd_set descriptors;



int main(int argc, const char* argv[]) {
   dpy = xcb_connect(NULL, NULL);
   if (xcb_connection_has_error(dpy)){
      if(debug) fprintf(stderr, "Error: XCB connection has error!\n");
      exit(1);
   };
   screen = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
   if (screen == NULL){
      if(debug) fprintf(stderr, "Error: Can't acquire screen.!\n");
      exit(1);
   };
   root = screen->root;
   err = xcb_request_check(
      dpy, 
      xcb_grab_button_checked(
         dpy, 
         true, 
         root, 
         XCB_EVENT_MASK_BUTTON_PRESS|XCB_EVENT_MASK_BUTTON_RELEASE, 
         XCB_GRAB_MODE_SYNC, 
         XCB_GRAB_MODE_ASYNC, 
         XCB_NONE, 
         XCB_NONE, 
         XCB_BUTTON_INDEX_2, 
         XCB_MOD_MASK_ANY));
   if (err != NULL) {
      if(debug) fprintf(stderr, "Could not grab");
      if (err->error_code == XCB_ACCESS){
         if(debug) fprintf(stderr, "the combination is already grabbed.\n");
      }
      else {
         if(debug) fprintf(stderr, "error %u encountered.\n", err->error_code);
      };
      free(err);
   } else {
      if(debug) fprintf(stderr, "Grab estabilished\n");
   };
   fd = xcb_get_file_descriptor(dpy);
   xcb_flush(dpy);
   while (1) {
      FD_ZERO(&descriptors);
      FD_SET(fd, &descriptors);
      if (select(fd + 1, &descriptors, NULL, NULL, NULL) > 0) {
         while ((evt = xcb_poll_for_event(dpy)) != NULL) {
            uint8_t event_type = XCB_EVENT_RESPONSE_TYPE(evt);
            switch (event_type) {
               case XCB_BUTTON_PRESS:
               case XCB_BUTTON_RELEASE:
                  e = (xcb_button_press_event_t *) evt;
                  if(debug) fprintf(stderr, "Button %u %u %u\n", event_type, e->detail, e->state);
                  if(e->detail == 2){
                     display = XOpenDisplay(NULL);
                     if (display == NULL) {
                        if(debug) fprintf(stderr, "ERROR: Failed to connect to the X server.\n");
                     }
                     else {
                        XSetSelectionOwner(display, XA_PRIMARY, None, CurrentTime);
                        XSync(display, False);
                        if(debug) fprintf(stderr, "Selection cleared\n");
                     }
                  };
                  xcb_allow_events(dpy, XCB_ALLOW_SYNC_POINTER, XCB_CURRENT_TIME);
                  break;
               default:
                  if(debug) fprintf(stderr, "Received event %u type\n", event_type);
                  break;
            }
            free(evt);
            xcb_flush(dpy);
         }
      } else {
      };
   };
   return 0;
}
