#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/inc/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"



struct fh_model *mdl;


void load_model(void)
{
	FILE *fd;
	struct amo_model *data;

	int vtxnum;
	float *vtx = NULL;
	float *tex = NULL;
	float *nrm = NULL;

	int idxnum;
	unsigned int *idx = NULL;

	struct fh_model_c *c;

	char *pth = "./res/models/cube.amo";


	if(!(fd = fopen(pth, "r"))) {
		printf("Failed to open file\n");
		goto err_return;
	}

	if(!(data = amo_load(fd))) {
		printf("Failed to load model data\n");
		goto err_close_fd;
	}

	amo_getdata(data, &vtxnum, (void **)&vtx, (void **)&tex, (void **)&nrm,
			NULL, NULL, &idxnum, &idx);


	if(!(c = fh_mdlc_begin("test", vtxnum, idxnum, idx))) {
		printf("Failed to begin construction\n");
		goto err_free_buffers;
	}


	fh_mdlc_shader(c, "model");
	fh_mdlc_texture(c, "red");

	fh_mdlc_attrib(c, "vtxPos", 3, GL_FLOAT, vtx);
	fh_mdlc_attrib(c, "vtxTex", 2, GL_FLOAT, tex);
	fh_mdlc_attrib(c, "vtxNrm", 3, GL_FLOAT, nrm);

	fh_mdlc_uniform(c, "camera", sizeof(struct cam_p));


	if(!(mdl = fh_mdlc_finish(c))) {
		printf("Failed to finalize construction\n");
		goto err_free_buffers;
	}


	fclose(fd);

	return;

err_free_buffers:
	fh_free(vtx);
	fh_free(tex);
	fh_free(nrm);
	fh_free(idx);

	amo_destroy(data);

err_close_fd:
	fclose(fd);

err_return:
	return;
}


void load_ui(void)
{
	struct fh_model_c *c;

	unsigned int vtxnum = 4;
	unsigned int idxnum = 6;

	float vertices[] = {
		-1,  0,  -1,
		-1,  0,   1,
		 1,  0,   1,
		 1,  0,  -1
	};

	/* Texture coordinates (2 floats per vertex) */
	float texCoords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	/* Indices (3 ints per triangle) */
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	if(!(c = fh_mdlc_begin("test", vtxnum, idxnum, indices))) {
		printf("Failed to begin construction\n");
		return;
	}

	fh_mdlc_shader(c, "ui");
	fh_mdlc_texture(c, "red");

	fh_mdlc_attrib(c, "vtxPos", 3, GL_FLOAT, vertices);
	fh_mdlc_attrib(c, "vtxTex", 2, GL_FLOAT, texCoords);

	if(!(mdl = fh_mdlc_finish(c))) {
		printf("Failed to finalize construction\n");
		return;
	}
}


int main(void) {
	struct fh_event evt;
	s32 wd;
	struct fh_element *ele;
	struct fh_window *win;	

	struct fh_shader *shader;
	struct fh_texture *texture;


	float x = 0;



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
	if(fh_load_shader("ui", "./res/shaders/ui.vert", "./res/shaders/ui.frag") < 0) {
		printf("Failed to load shader\n");
	}

	fh_load_shader("model", "./res/shaders/model.vert", "./res/shaders/model.frag");
	texture = fh_tex_load("red", "./res/images/red.png");
	fh_tex_insert(texture);

	load_model();


	fh_set_ui_shader("ui");

	win = fh_win_get(wd);
	fh_add(wd, fh_get(wd, "body"), "Test1", 0);

	win->document->renderer->shader = fh_shd_get("ui");

	fh_shd_show_attrib(fh_shd_get("ui"));

	fh_doc_tree(win->document);



	while(fh_update()) {	
		while(fh_pull_event(&evt)) {
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fh_mdl_render(mdl);

		SDL_GL_SwapWindow(win->handle);
	}

err_cleanup:
	fh_quit();

err_return:
	return 0;
}

