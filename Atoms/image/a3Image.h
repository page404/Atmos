#ifndef A3_IMAGE_H
#define A3_IMAGE_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>

class a3CameraSample;

class a3Image
{
public:
	a3Image(int width, int height, const std::string& fileName);

	~a3Image();

    void addSample(const a3CameraSample* sample, const t3Vector3f& L);

	void write();

	t3Vector3f **pixels;

	int width, height;

	std::string fileName;
};

#endif