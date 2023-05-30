
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

#include "lib/freihand/inc/freihand.h"
#include "lib/amoloader/amoloader.h"
#include "lib/mate/inc/mate.h"


struct model_m {
	mat4_t pos;
	mat4_t rot;
};

struct camera_m {
	mat4_t view;
	mat4_t proj;
};



static struct fh_model *loadCube(struct fh_context *ctx)
{
	struct fh_model *mdl;

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


	printf("model!\n");

	if(!(c = fh_BeginModelConstr("model_a", vtxnum, idxnum, idx))) {
		printf("Failed to begin construction\n");
		goto err_free_buffers;
	}


	fh_ModelConstrShader(c, fh_GetShader(ctx, "model"));
	fh_ModelConstrTexture(c, fh_GetTexture(ctx, "red"));

	fh_ModelConstrAttrib(c, "v_pos", 3, GL_FLOAT, vtx);
	fh_ModelConstrAttrib(c, "v_uv", 2, GL_FLOAT, tex);
	fh_ModelConstrAttrib(c, "v_nrm", 3, GL_FLOAT, nrm);

	fh_ModelConstrUniform(c, "model", sizeof(struct model_m));
	fh_ModelConstrUniform(c, "camera", sizeof(struct camera_m));


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
	struct fh_document *doc;

	struct fh_event evt;

	struct fh_element *canvas;
	struct fh_view *view;

	struct fh_camera *camera;
	struct fh_model *cube;

	struct model_m model_mat;
	struct camera_m camera_mat;

	float x;

	mat4_idt(model_mat.pos);
	mat4_idt(model_mat.rot);

	mat4_idt(camera_mat.view);
	mat4_idt(camera_mat.proj);


	srand(time(NULL));

	/* Initialize the FH-framework */
	if(fh_Init() < 0)
		goto err_return;

	/* Create the window and context */
	if(!(win = fh_CreateWindow(NULL, "main", 800, 600))) goto err_quit;
	ctx = fh_GetContext(win);
	doc = fh_GetDocument(win);

	/* Load resources */
	fh_LoadTexture(win->context, "red", "./res/images/red.png");
	fh_LoadTexture(win->context, "brick", "./res/images/brick.png");
	fh_LoadTexture(win->context, "unt", "./res/images/Untitled.png");

	/* Create the canvas */
	canvas = fh_AddElement(doc, fh_GetElement(doc, "body"), "canvas", FH_VIEW, NULL);
	view = fh_GetView(canvas);

	/* Get the camera */
	camera = fh_GetViewCamera(view);

	/* Create the cube */
	cube = loadCube(ctx);

	fh_ViewAddModel(view, cube);

	x = 5;

	while(fh_Update()) {	
		while(fh_pull_event(&evt)) {
		}

		x += 0.01;


		mat4_pfpos_s(model_mat.pos, 0, x, 0);


		fh_GetViewMat(camera, camera_mat.view);
		fh_GetProjectionMat(camera, camera_mat.proj);

	

		fh_SetModelUniform(cube, "model", &model_mat);
		fh_SetModelUniform(cube, "camera", &camera_mat);
		

		fh_RedrawWindow(win);
	}

err_quit:
	fh_Quit();

err_return:
	return 0;
}
