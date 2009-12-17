#ifndef PARAMS_H_
#define PARAMS_H_

namespace Hex {

class Params {
public:
	static float alpha;
	static float beta;
	static unsigned initialization;
};

float Params::alpha = 1.0;
float Params::beta = 20.0;
unsigned Params::initialization = 1;

} // namespace Hex

#endif /* PARAMS_H_ */
