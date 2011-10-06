#ifndef BIQUADFILTER_H
#define BIQUADFILTER_H

/**
 simplest implementation of a BiQuad filter
 */
class BiQuadFilter
{
protected:
	float b0, b1, b2, a1, a2;	// coeff
	float x1, x2, y1, y2;		// x(t-1), ...

public:
	// dummy constructor
	BiQuadFilter();
	/** a0 coeff is by default 1 */
	BiQuadFilter(float b0, float b1, float b2, float a1, float a2);
	/** process buffer in, writing in buffer out; n is buffer length */
	void tick(float * in, float * out, int n);
	/** process buffer in, summing in buffer out; n is buffer length */
	void tick_sum(float * in, float * out, int n);
	/** reset the status variables (to 0) */
	void clear_status();
};

#endif
