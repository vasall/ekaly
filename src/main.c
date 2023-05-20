
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/inc/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"

struct fh_model *load_model(struct fh_context *ctx);


struct uniform_buffer {
	mat4_t pos;
	mat4_t rot;
};

struct camera_buffer {
	mat4_t view;
	mat4_t proj;
};



struct fh_model *load_model(struct fh_context *ctx)
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
	struct fh_model *mdl;

	char *pth = "./res/models/cube.amo";

	vec3_t ini_pos = {0, 0, 0};
	vec3_t ini_rot = {0, 0, 0};


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

	if(!(c = fh_BeginModelConstr("model_a", vtxnum, idxnum, idx))) {
		printf("Failed to begin construction\n");
		goto err_free_buffers;
	}

	fh_ModelConstrShader(c, fh_GetShader(ctx, "model"));
	fh_ModelConstrTexture(c, fh_GetTexture(ctx, "brick"));

	fh_ModelConstrAttrib(c, "v_pos", 3, GL_FLOAT, vtx);
	fh_ModelConstrAttrib(c, "v_uv", 2, GL_FLOAT, tex);
	fh_ModelConstrAttrib(c, "v_nrm", 3, GL_FLOAT, nrm);

	fh_ModelConstrUniform(c, "model", sizeof(struct uniform_buffer));
	fh_ModelConstrUniform(c, "camera", sizeof(struct camera_buffer));


	if(!(mdl = fh_EndModelConstr(c, ctx, ini_pos, ini_rot))) {
		printf("Failed to finalize construction\n");
		goto err_free_buffers;
	}


	fh_ModelConstrCleanup(c);

	fclose(fd);

	return mdl;

err_free_buffers:
	fh_free(vtx);
	fh_free(tex);
	fh_free(nrm);
	fh_free(idx);

	amo_destroy(data);

err_close_fd:
	fclose(fd);

err_return:
	return NULL;
}



int main(void)
{
	struct fh_window *win;
	struct fh_context *ctx;

	struct fh_model *mdl;

	struct fh_event evt;

	struct uniform_buffer unibuf;
	struct camera_buffer camview;
	float x = 0;

	struct fh_camera_info cam_info;
	struct fh_camera *cam;

	mat4_idt(unibuf.pos);
	mat4_idt(unibuf.rot);
	mat4_idt(camview.view);
	mat4_idt(camview.proj);


	srand(time(NULL));

	/* Initialize the FH-framework */
	if(fh_init() < 0)
		goto err_return;

	/* Create the window and context */
	win = fh_CreateWindow(NULL, "main", 800, 600);
	ctx = fh_CreateContext(win);

	/* Load resources */
	fh_LoadTexture(ctx, "red", "./res/images/red.png");
	fh_LoadTexture(ctx, "brick", "./res/images/brick.png");
	fh_LoadTexture(ctx, "unt", "./res/images/Untitled.png");

	/* Load and create the model */
	mdl = load_model(ctx);

	/* Create a camera */
	cam_info.area_of_view = 60;
	cam_info.aspect_ratio = 100.0f / 100.0f;
	cam_info.near = 0.01;
	cam_info.far = 1000;
	cam = fh_CreateCamera(ctx, "main", cam_info);


	while(fh_update()) {	
		while(fh_pull_event(&evt)) {
		}
		
		glViewport(200, 300, 100, 100);
		glEnable(GL_SCISSOR_TEST);
		glScissor(200, 200, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		x += 0.01;
		mat4_idt(unibuf.pos);
		mat4_pfpos_s(unibuf.pos, 0, x, 0);

		fh_GetViewMat(cam, camview.view);
		fh_GetProjectionMat(cam, camview.proj);

		fh_SetModelUniform(mdl, "model", &unibuf);
		fh_SetModelUniform(mdl, "camera", &camview);
		fh_RenderModel(mdl);	

		fh_redraw_window(win);
	}


	fh_DestroyContext(ctx);

	return 0;

err_return:
	return -1;
}
