#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"


static void add_blocks(struct fh_document *doc, struct fh_element *par, u8 num)
{
	u8 i;
	struct fh_element *ele;
	struct fh_element *container;
	struct fh_element *parent;

	char name[15];
	char text[128];

	/*
	 * Create a container element.
	 */
	ele = fh_AddElement(doc, par, "container", FH_BLOCK, NULL);

	fh_ModifyElementStyle(ele, "infill_color: #ffffff;");
	fh_ModifyElementStyle(ele, "vsize: 100%; hsize: 90%;");
	fh_ModifyElementStyle(ele, "spacing_right: 5%;");
	fh_ModifyElementStyle(ele, "spacing_left: 5%;");

	fh_ModifyElementStyle(ele, "padding_top:5%; padding_right: 5%;");
	fh_ModifyElementStyle(ele, "padding_bottom: 5%; padding_left: 5%;");
	fh_ModifyElementStyle(ele, "border_width: 3; border_color: #000000;");

	container = ele;
	parent = container;

	for(i = 0; i < num; i++) {
		name[0] = i + 65;
		name[1] = 0;

		ele = fh_AddElement(doc, container, name, FH_BLOCK, NULL);

		fh_ModifyElementStyle(ele, "vsize: 20%; hsize: 10%;");
		fh_ModifyElementStyle(ele, "spacing_top: 10%;spacing_left: 22%;");
		fh_ModifyElementStyle(ele, "radius_top_left: 15;");
		fh_ModifyElementStyle(ele, "border_width: 3; border_color: #000000;");

		sprintf(text, "infill_color: #00%02x00;", (((i+3) * 20) % 255));
		fh_ModifyElementStyle(ele, text);

		parent = ele;
	}

	fh_UpdateDocument(doc);
	fh_RenderDocumentUI(doc);
}

static struct fh_window *add_window(void)
{
	struct fh_window *win;
	struct fh_window *main;
	static char *names[5] = {"main", "eins", "zwei", "drei", "vier"};
	int i;

	/* Create the window and context */
	if(!(win = fh_CreateWindow(NULL, names[0], 800, 600)))
		return NULL;

	/* Load resources */
	fh_LoadTexture(win->context, "red", "./res/images/red.png");
	fh_LoadTexture(win->context, "brick", "./res/images/brick.png");
	fh_LoadTexture(win->context, "unt", "./res/images/Untitled.png");

	add_blocks(win->document, fh_GetElement(win->document, "body"), 7);

	main = win;

	return main;
}


s8 test_callback(struct fh_event *evt, void *data)
{
	printf(">>>>>>>< HELLO WORLD!!!\n");
	return 1;
}



int main(void)
{
	struct fh_window *mainwin;
	int i;

	struct fh_event evt;

	srand(time(NULL));

	/* Initialize the FH-framework */
	if(fh_Init() < 0)
		goto err_return;


	mainwin = add_window();

	fh_DumpWindowTree();

#if 0
	fh_BindEventListener(
			fh_GetElement(mainwin->document, "D")->event_handler, 
			FH_EVT_MOUSEBUTTONDOWN,
			"test",
			&test_callback,
			NULL
			);
#endif

	while(fh_Update());

	printf("Quitting...\n");
	fh_Quit();

err_return:
	return 0;
}
