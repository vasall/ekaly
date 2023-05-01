#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/inc/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"


struct uniform_buffer {
	mat4_t pos;
	mat4_t rot;
	mat4_t view;
	mat4_t proj;
};

struct camera_compact {
	mat4_t view;
	mat4_t proj;
};



struct stuff {
	struct fh_window *window;
	struct fh_shader *shader;
	struct fh_texture *texture;
	struct fh_camera *cam;


	struct uniform_buffer unibuf;
	struct camera_compact camcom; 

	vec3_t rot;
	float x;

	struct fh_model *mdl;
	struct fh_model *ui;
};

void load_model(struct fh_window *win, struct stuff *s)
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
	fh_mdlc_texture(c, "brick");

	fh_mdlc_attrib(c, "vtxPos", 3, GL_FLOAT, vtx);
	fh_mdlc_attrib(c, "vtxTex", 2, GL_FLOAT, tex);
	fh_mdlc_attrib(c, "vtxNrm", 3, GL_FLOAT, nrm);

	fh_mdlc_uniform(c, "camera", sizeof(struct uniform_buffer));


	if(!(s->mdl = fh_mdl_create(win, c))) {
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


void load_ui(struct fh_window *win, struct stuff *s)
{
	struct fh_model_c *c;

	unsigned int vtxnum = 4;
	unsigned int idxnum = 6;

#if 0
	float vertices[] = {
		-1,  0,  -1,
		-1,  0,   1,
		 1,  0,   1,
		 1,  0,  -1
	};
#else
	float vertices[] = {
		-1,  -1,   0,
		-1,   1,   0,
		 1,   1,   0,
		 1,  -1,   0
	};
#endif

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
	fh_mdlc_texture(c, "unt");

	fh_mdlc_attrib(c, "vtxPos", 3, GL_FLOAT, vertices);
	fh_mdlc_attrib(c, "vtxTex", 2, GL_FLOAT, texCoords);

	fh_mdlc_uniform(c, "camera", sizeof(struct camera_compact));

	if(!(s->ui = fh_mdl_create(win, c))) {
		printf("Failed to finalize construction\n");
		return;
	}
}



void init_stuff(struct stuff *s, struct fh_window *parent, char *name, u32 w, u32 h)
{

	struct fh_camera_info cam_info;

	mat4_idt(s->unibuf.pos);
	mat4_idt(s->unibuf.rot);
	mat4_idt(s->unibuf.view);
	mat4_idt(s->unibuf.proj);

	mat4_idt(s->camcom.view);
	mat4_idt(s->camcom.proj);

	vec3_clr(s->rot);


	s->window = fh_add_window(parent, name, w, h);

	/* Create the UI shader */
	fh_load_shader(s->window, "ui", "./res/shaders/ui.vert", "./res/shaders/ui.frag");
	fh_load_shader(s->window, "model", "./res/shaders/model.vert", "./res/shaders/model.frag");
	fh_load_texture(s->window, "red", "./res/images/red.png");
	fh_load_texture(s->window, "brick", "./res/images/brick.png");
	fh_load_texture(s->window, "unt", "./res/images/Untitled.png");


	load_ui(s->window, s);
	load_model(s->window, s);

	/* Create a camera */
	cam_info.area_of_view = 60;
	cam_info.aspect_ratio = 800.0f / 600.0f;
	cam_info.near = 0.0001;
	cam_info.far = 1000;
	s->cam = fh_create_camera(s->window, "main", cam_info);
}





int main(void) {
	struct fh_event evt;
	struct fh_element *ele;


	struct stuff one;
	struct stuff two;


	srand(time(NULL));

	if(fh_init() < 0)
		goto err_return;
	

	init_stuff(&one, NULL, "Hauptfenster", 800, 600);
	init_stuff(&two, one.window, "Unterfenster", 400, 300);


	while(fh_update()) {	
		while(fh_pull_event(&evt)) {
		}




		/* ==================================================== */

		one.x -= 0.01;
		one.rot[0] += M_PI / 200;
		one.rot[1] += M_PI / 100;
		one.rot[2] += M_PI / 50;

		fh_activate_window(one.window);
		fh_clear_window(one.window);


		one.cam->pos[1] = one.x;

		fh_cam_update_proj(one.cam);
		fh_cam_update_view(one.cam);

		mat4_rfagl(one.unibuf.rot, one.rot);

		
		fh_cam_get_proj(one.cam, one.unibuf.proj);
		fh_cam_get_view(one.cam, one.unibuf.view);

		fh_mdl_set_uniform(one.ui, "camera", &one.camcom);
		fh_mdl_set_uniform(one.mdl, "camera", &one.unibuf);

		fh_mdl_render(one.mdl);
		fh_mdl_render(one.ui);

		fh_redraw_window(one.window);
	
		/* ==================================================== */	

		two.x -= 0.01;
		two.rot[0] -= M_PI / 200;
		two.rot[1] -= M_PI / 100;
		two.rot[2] -= M_PI / 50;

		fh_activate_window(two.window);
		fh_clear_window(two.window);

		two.cam->pos[1] = two.x;

		fh_cam_update_proj(two.cam);
		fh_cam_update_view(two.cam);

		mat4_rfagl(two.unibuf.rot, two.rot);

		
		fh_cam_get_proj(two.cam, two.unibuf.proj);
		fh_cam_get_view(two.cam, two.unibuf.view);

		fh_mdl_set_uniform(two.ui, "camera", &two.camcom);
		fh_mdl_set_uniform(two.mdl, "camera", &two.unibuf);

		fh_mdl_render(two.mdl);
		fh_mdl_render(two.ui);

		fh_redraw_window(two.window);

		/* ==================================================== */
	}

err_cleanup:
	fh_quit();

err_return:
	return 0;
}

