#ifndef _SVMWRAPPER_H_
#define _SVMWRAPPER_H_

#include "svm.h"
#include <iostream>
#include <vector>

typedef void(*FUNC)(const char*, ...);

class SvmWrapper
{
	public:
		SvmWrapper()
			:predict_probability(0),
			 max_nr_attr(64)
		{

		}
		void predictt();
		double predict(const std::vector<std::string>& input, FUNC func);
		std::string GetModelPath();
		std::string GetOutputPath();
	private:
		bool init();

		struct svm_model* model;
		struct svm_node *x;

		int predict_probability;
		int max_line_len;
		int max_nr_attr;
		const static int PORTRAIT = 1;
		const static int LANDSCAPE = 1;
		const static int LANDREVERSE = 1;
};

extern "C" {
	int predict_entry(int argc, char **argv);
}

#endif
