
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/inc/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"


static void addElements(struct fh_document *doc)
{
	struct fh_element *ele;

	ele = fh_AddElement(doc, fh_GetElement(doc, "body"), "test", FH_BLOCK);
}



int main(void)
{
	struct fh_window *win;


	struct fh_event evt;




	srand(time(NULL));

	/* Initialize the FH-framework */
	if(fh_Init() < 0)
		goto err_return;

	/* Create the window and context */
	if(!(win = fh_CreateWindow(NULL, "main", 800, 600))) goto err_quit;

	/* Load resources */
	fh_LoadTexture(win->context, "red", "./res/images/red.png");
	fh_LoadTexture(win->context, "brick", "./res/images/brick.png");
	fh_LoadTexture(win->context, "unt", "./res/images/Untitled.png");

	/* Add some elements */
	addElements(win->document);	

	while(fh_Update()) {	
		while(fh_pull_event(&evt)) {
		}

		fh_RedrawWindow(win);
	}

err_quit:
	fh_Quit();

err_return:
	return 0;
}
