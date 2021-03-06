﻿//#define ATMOS_ASSIMP
#ifdef ATMOS_ASSIMP
// assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <core/log/a3Log.h>

const struct aiScene* scene = NULL;

int main()
{
    scene = aiImportFile("../../../../resources/models/test/test.obj", aiProcessPreset_TargetRealtime_MaxQuality);

    int numOfMesh = scene->mNumMeshes;
    a3Log::print("Num of meshes: %d\n", numOfMesh);

    for(int i = 0; i < numOfMesh; i++)
    {
        const struct aiMesh* mesh = scene->mMeshes[i];

        aiVector3D* vertices = mesh->mVertices;

        for(int i = 0; i < mesh->mNumVertices; i++)
        {
            a3Log::print("v[%d](%f, %f, %f)\n", i, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        }

        a3Log::print("\n");

        for(int i = 0; i < mesh->mNumVertices; i++)
        {
            a3Log::print("n[%d](%f, %f, %f)\n", i, mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        a3Log::print("\n");

        if(mesh->mTextureCoords[0])
        {
            for(int i = 0; i < mesh->mNumVertices; i++)
            {
                a3Log::print("vt[%d](%f, %f, %f)\n", i, mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].z);
            }
        }

        a3Log::print("\n");

        for(int i = 0; i < mesh->mNumFaces; i++)
        {
            a3Log::print("f[%d]( ", i);
            for(int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            {
                a3Log::print("%d ", mesh->mFaces[i].mIndices[j]);
            }
            a3Log::print(")\n");
        }
    }

    getchar();
    return 0;
}

#else
// tinyobjloader
#include <core/log/a3Log.h>
#include <tiny_obj_loader.h>

void main()
{
    const char* filePath = "../../../../resources/models/mitsuba.obj";

    a3Log::debug("Parsering model file \"%s\"...\n", filePath);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath);

    if(!err.empty())
        a3Log::warning("%s\n", err.c_str());

    if(!ret)
    {
        a3Log::error("无法打开文件%s\n", filePath);
        getchar();
        return;
    }

    a3Log::debug("Total vertices: %d, normals : %d\n texcorrds : %d\n",
                 (int)attrib.vertices.size() / 3,
                 (int) attrib.normals.size() / 3,
                 (int) attrib.texcoords.size() / 2);

    // Loop over shapes
    for(size_t s = 0; s < shapes.size(); s++)
    {
        int numFaceVertices = shapes[s].mesh.num_face_vertices.size();
        a3Log::debug("faces: %f\n", (int) numFaceVertices / 3);

        // Loop over faces(polygon)
        // --!FAQ: https://github.com/syoyo/tinyobjloader
        size_t index_offset = 0;
        for(size_t f = 0; f < numFaceVertices; f++)
        {
            a3Log::info("Loading...%8.2f \n", (float) f / numFaceVertices * 100.0f);

            int fv = shapes[s].mesh.num_face_vertices[f];

            if(fv != 3)
            {
                a3Log::error("Atmos仅支持三角形作为片元, 而当前模型面由%d个顶点组成\n", fv);
                getchar();
                return;
            }

            // Loop over vertices in the face.
            //for(size_t v = 0; v < fv; v++)
            //{
                // access to vertex
                // -----------------------------v0-----------------------------
            tinyobj::index_t idx0 = shapes[s].mesh.indices[index_offset + 0];
            float vx0 = attrib.vertices[3 * idx0.vertex_index + 0];
            float vy0 = attrib.vertices[3 * idx0.vertex_index + 1];
            float vz0 = attrib.vertices[3 * idx0.vertex_index + 2];

            float nx0 = attrib.normals[3 * idx0.normal_index + 0];
            float ny0 = attrib.normals[3 * idx0.normal_index + 1];
            float nz0 = attrib.normals[3 * idx0.normal_index + 2];

            a3Log::print("v[0](%f, %f, %f)\n", vx0, vy0, vz0);
            a3Log::print("n[0](%f, %f, %f)\n", nx0, ny0, nz0);

            if(idx0.texcoord_index != -1)
            {
                float tx0 = attrib.texcoords[2 * idx0.texcoord_index + 0];
                float ty0 = attrib.texcoords[2 * idx0.texcoord_index + 1];

                a3Log::print("t[0](%f, %f)\n", tx0, ty0);
            }

            // -----------------------------v1-----------------------------
            tinyobj::index_t idx1 = shapes[s].mesh.indices[index_offset + 1];
            float vx1 = attrib.vertices[3 * idx1.vertex_index + 0];
            float vy1 = attrib.vertices[3 * idx1.vertex_index + 1];
            float vz1 = attrib.vertices[3 * idx1.vertex_index + 2];

            float nx1 = attrib.normals[3 * idx1.normal_index + 0];
            float ny1 = attrib.normals[3 * idx1.normal_index + 1];
            float nz1 = attrib.normals[3 * idx1.normal_index + 2];


            a3Log::print("v[1](%f, %f, %f)\n", vx1, vy1, vz1);
            a3Log::print("n[1](%f, %f, %f)\n", nx1, ny1, nz1);

            if(idx1.texcoord_index != -1)
            {
                float tx1 = attrib.texcoords[2 * idx1.texcoord_index + 0];
                float ty1 = attrib.texcoords[2 * idx1.texcoord_index + 1];
                a3Log::print("t[1](%f, %f)\n", tx1, ty1);
            }

            // -----------------------------v2-----------------------------
            tinyobj::index_t idx2 = shapes[s].mesh.indices[index_offset + 2];
            float vx2 = attrib.vertices[3 * idx2.vertex_index + 0];
            float vy2 = attrib.vertices[3 * idx2.vertex_index + 1];
            float vz2 = attrib.vertices[3 * idx2.vertex_index + 2];

            float nx2 = attrib.normals[3 * idx2.normal_index + 0];
            float ny2 = attrib.normals[3 * idx2.normal_index + 1];
            float nz2 = attrib.normals[3 * idx2.normal_index + 2];

            a3Log::print("v[2](%f, %f, %f)\n", vx2, vy2, vz2);
            a3Log::print("n[2](%f, %f, %f)\n", nx2, ny2, nz2);

            if(idx2.texcoord_index != -1)
            {
                float tx2 = attrib.texcoords[2 * idx2.texcoord_index + 0];
                float ty2 = attrib.texcoords[2 * idx2.texcoord_index + 1];
                a3Log::print("t[2](%f, %f, %f)\n", tx2, ty2);
            }

            //}

            index_offset += fv;
        }
    }

    getchar();
    return;
}
#endif
