
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/gentils/inc/gentils.h"
#include "lib/freihand/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"

#if 1
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

	fh_ModifyElementStyle(ele, "infill_color: #ff0000;");
	fh_ModifyElementStyle(ele, "vsize: 100%; hsize: 90%;");
	fh_ModifyElementStyle(ele, "spacing_right: 5%;");
	fh_ModifyElementStyle(ele, "spacing_left: 5%;");

	fh_ModifyElementStyle(ele, "padding_top:5%; padding_right: 5%;");
	fh_ModifyElementStyle(ele, "padding_bottom: 5%; padding_left: 5%;");

	container = ele;
	parent = container;

	for(i = 0; i < num; i++) {
		name[0] = i + 65;
		name[1] = 0;

		ele = fh_AddElement(doc, container, name, FH_BLOCK, NULL);

		fh_ModifyElementStyle(ele, "vsize: 40%; hsize: 40%;");
		fh_ModifyElementStyle(ele, "spacing_top: 10%; spacing_left: 5%;");

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

	add_blocks(win->document, fh_GetElement(win->document, "body"), 1);

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

#else

#define WIN_W	800
#define WIN_H	600

int main(void)
{
	struct fh_window *window;
	struct fh_shader shd;
	s32 gl_context;

	struct fh_vertex_attrib v_attributes[] = {
		{2, GL_INT}					/* position */
	};

	struct fh_batch *batch;

	s8 running = 1;
	s32 i;

	int x1 = 100;
	int y1 = 100;
	int x2 = 300;
	int y2 = 200;

	struct tempStruct {
		int x;
		int y;
	} vdata;

	unsigned int index[4];

	SDL_Event event;

	unsigned int progam;


	/* Initialize subsystems */
	fh_Init();

	if(!(window = fh_CreateWindow(NULL, "main", WIN_W, WIN_H))) {
		printf("Failed to create window!!\n");
		return -1;
	}

	/*
	 * Create the batch-renderer.
	 */
	batch = fh_batch_create(
			1,		/* Number of vertex attributes */
			v_attributes,	/* List of all vertex attributes */
			6000,		/* Vertex capacity */
			6000,		/* Index capacity */
			0,		/* Number of uniform buffers */
			NULL		/* List of all uniforms */
	);


	while(running) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				running = 0;
			}
		}

		vdata = (struct tempStruct){x1, y1};
		index[0] = fh_batch_push_vertex(batch, (void *)&vdata);
		vdata = (struct tempStruct){x2, y1};
		index[1] = fh_batch_push_vertex(batch, (void *)&vdata);
		vdata = (struct tempStruct){x2, y2};
		index[2] = fh_batch_push_vertex(batch, (void *)&vdata);
		vdata = (struct tempStruct){x1, y2};
		index[3] = fh_batch_push_vertex(batch, (void *)&vdata);

		fh_batch_push_index(batch, index[0]);
		fh_batch_push_index(batch, index[2]);
		fh_batch_push_index(batch, index[3]);

		fh_batch_push_index(batch, index[0]);
		fh_batch_push_index(batch, index[1]);
		fh_batch_push_index(batch, index[2]);

		/* Clear the window */
		glClear(GL_COLOR_BUFFER_BIT);

		fh_batch_flush(batch);

		/* Swap buffer */
		SDL_GL_SwapWindow(window->handle);

	}

	return 0;
}

#endif
