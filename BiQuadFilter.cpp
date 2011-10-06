#include "BiQuadFilter.h"

BiQuadFilter::BiQuadFilter()
{
	// set everything like a unit filter
	b0 = 1;
	b1 = b2 = 0;
	a1 = a2 = 0;
	x1 = x2 = y1 = y2 = 0;
}

BiQuadFilter::BiQuadFilter(float b0, float b1, float b2, float a1, float a2)
{
	this->b0 = b0;
	this->b1 = b1;
	this->b2 = b2;
	this->a1 = a1;
	this->a2 = a2;
	x1 = x2 = y1 = y2 = 0;
}

void BiQuadFilter::tick(float * in, float * out, int n)
{
	for(int i = 0; i < n; i++)
	{
		float x0 = in[i];
		float y0 = 0;
		y0 += b0 * x0;
		y0 += b1 * x1;
		y0 += b2 * x2;
		y0 -= a1 * y1;
		y0 -= a2 * y2;
		out[i] = y0;
		x2 = x1;
		x1 = x0;
		y2 = y1;
		y1 = y0;
	}
}

void BiQuadFilter::clear_status()
{
	x1 = x2 = y1 = y2 = 0;
}

void BiQuadFilter::tick_sum(float * in, float * out, int n)
{
	for(int i = 0; i < n; i++)
	{
		float x0 = in[i];
		float y0 = 0;
		y0 += b0 * x0;
		y0 += b1 * x1;
		y0 += b2 * x2;
		y0 -= a1 * y1;
		y0 -= a2 * y2;
		out[i] += y0;
		x2 = x1;
		x1 = x0;
		y2 = y1;
		y1 = y0;
	}
}
