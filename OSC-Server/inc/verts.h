#ifndef VERTS_H
#define VERTS_H

struct vert
{
    GLfloat v[3];
    GLfloat n[3];
    GLfloat t[2];
};

static vert square_verts[4] = {
	{{ -1.f,  1.f,  -3.f }, {  0.f,  0.f,  1.f }, {0.0f,0.0f}},
    {{ -1.f, -1.f,  -3.f }, {  0.f,  0.f,  1.f }, {0.0f,1.0f}},
    {{  1.f, -1.f,  -3.f }, {  0.f,  0.f,  1.f }, {1.0f,1.0f}},
    {{  1.f,  1.f,  -3.f }, {  0.f,  0.f,  1.f }, {1.0f,0.0f}}
};

static vert isotri_verts[3] = {
	{{-1.f,-1.f,1.f},{0.f,0.f,1.f}, {0.0f,0.0f}},
	{{ 1.f,-1.f,1.f},{0.f,0.f,1.f}, {0.0f,0.0f}},
	{{  0.f,1.f,1.f},{0.f,0.f,1.f}, {0.0f,0.0f}}
};

static vert cube_verts[24] = {

    /* +X */
    {{  1.f,  1.f,  1.f }, {  1.f,  0.f,  0.f }, {0.0f,0.0f}},
    {{  1.f, -1.f,  1.f }, {  1.f,  0.f,  0.f }, {0.0f,0.0f}},
    {{  1.f, -1.f, -1.f }, {  1.f,  0.f,  0.f }, {0.0f,0.0f}},
    {{  1.f,  1.f, -1.f }, {  1.f,  0.f,  0.f }, {0.0f,0.0f}},

    /* -X */
    {{ -1.f,  1.f, -1.f }, { -1.f,  0.f,  0.f }, {0.0f,0.0f}},
    {{ -1.f, -1.f, -1.f }, { -1.f,  0.f,  0.f }, {0.0f,0.0f}},
    {{ -1.f, -1.f,  1.f }, { -1.f,  0.f,  0.f }, {0.0f,0.0f}},
    {{ -1.f,  1.f,  1.f }, { -1.f,  0.f,  0.f }, {0.0f,0.0f}},

    /* +Y */
    {{ -1.f,  1.f, -1.f }, {  0.f,  1.f,  0.f }, {0.0f,0.0f}},
    {{ -1.f,  1.f,  1.f }, {  0.f,  1.f,  0.f }, {0.0f,0.0f}},
    {{  1.f,  1.f,  1.f }, {  0.f,  1.f,  0.f }, {0.0f,0.0f}},
    {{  1.f,  1.f, -1.f }, {  0.f,  1.f,  0.f }, {0.0f,0.0f}},

    /* -Y */
    {{ -1.f, -1.f,  1.f }, {  0.f, -1.f,  0.f }, {0.0f,0.0f}},
    {{ -1.f, -1.f, -1.f }, {  0.f, -1.f,  0.f }, {0.0f,0.0f}},
    {{  1.f, -1.f, -1.f }, {  0.f, -1.f,  0.f }, {0.0f,0.0f}},
    {{  1.f, -1.f,  1.f }, {  0.f, -1.f,  0.f }, {0.0f,0.0f}},

    /* +Z */
    {{ -1.f,  1.f,  1.f }, {  0.f,  0.f,  1.f }, {0.0f,0.0f}},
    {{ -1.f, -1.f,  1.f }, {  0.f,  0.f,  1.f }, {0.0f,0.0f}},
    {{  1.f, -1.f,  1.f }, {  0.f,  0.f,  1.f }, {0.0f,0.0f}},
    {{  1.f,  1.f,  1.f }, {  0.f,  0.f,  1.f }, {0.0f,0.0f}},

    /* -Z */
    {{  1.f,  1.f, -1.f }, {  0.f,  0.f, -1.f }, {0.0f,0.0f}},
    {{  1.f, -1.f, -1.f }, {  0.f,  0.f, -1.f }, {0.0f,0.0f}},
    {{ -1.f, -1.f, -1.f }, {  0.f,  0.f, -1.f }, {0.0f,0.0f}},
    {{ -1.f,  1.f, -1.f }, {  0.f,  0.f, -1.f }, {0.0f,0.0f}},
};

static GLushort square_elems[6] = {0,1,2,0,2,3};
static GLushort isotri_elems[3] = {0,1,2};

static GLushort cube_elems[36] = {
    0,  1,  2,  0,  2,  3,
    4,  5,  6,  4,  6,  7,
    8,  9, 10,  8, 10, 11,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 18, 19,
    20, 21, 22, 20, 22, 23
};

#endif
