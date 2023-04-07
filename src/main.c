#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/inc/freihand.h"


int main(void) {
	struct fh_event evt;
	s32 wd;
	struct fh_element *ele;
	struct fh_window *win;	
	
	struct fh_shader *shader;


	char *vars[2] = {"pos", "tex"};

	srand(time(NULL));

	if(fh_init() < 0)
		goto err_return;

	fh_sdl_print_info();

	if((wd = fh_add_window(0, "Hauptfenster", 800, 600)) < 0) {
		printf("Failed to create window");
		goto err_cleanup;
	}

	win = fh_win_get(wd);

	fh_gl_print_info();

	/* Create the UI shader */
	if(fh_load_shader("ui", "./res/shaders/ui.vert", "./res/shaders/ui.frag", 2, vars) < 0) {
		printf("Failed to load shader\n");
	}

	win = fh_win_get(wd);
	fh_add(wd, fh_get(wd, "body"), "Test1", 0);

	win->document->renderer->shader = fh_shader_get("ui");

	fh_doc_tree(win->document);



	while(fh_update()) {
		while(fh_pull_event(&evt)) {
			
		}
	}

err_cleanup:
	fh_quit();

err_return:
	return 0;
}

