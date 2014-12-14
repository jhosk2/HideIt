/*
 * SvmWrapper.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: 원빈
 */

#include "SvmWrapper.h"
#include "svm.h"
#include <vector>
#include <errno.h>
#include <FBase.h>
#include <FApp.h>

void SvmWrapper::predictt()
{
	Tizen::Base::String resPath = Tizen::App::App::GetInstance()->GetAppResourcePath();

	resPath.Append("test.txt");

	std::string after;
	std::wstring before(resPath.GetPointer());
	after.assign(before.begin(), before.end());

	char ** argv = new char*[4];
	argv[0] = "";
	argv[1] = const_cast<char*>(after.c_str());
	argv[2] = const_cast<char*>(GetModelPath().c_str());
	argv[3] = const_cast<char*>(GetOutputPath().c_str());

	predict_entry(4, argv);
}

std::string SvmWrapper::GetModelPath(){
	Tizen::Base::String resPath = Tizen::App::App::GetInstance()->GetAppResourcePath();
//	switch(type)
//	{
//	case PORTRAIT:
//		resPath.Append("portrait.txt.model");
//		break;
//	case LANDSCAPE:
//		resPath.Append("landscape.txt.model");
//		break;
//	case LANDREVERSE:
//		resPath.Append("reverse.txt.model");
//		break;
//	}

	resPath.Append("model.txt");

	std::string after;
	std::wstring before(resPath.GetPointer());
	after.assign(before.begin(), before.end());

	return after.c_str();
}

std::string SvmWrapper::GetOutputPath(){
	Tizen::Base::String resPath = Tizen::App::App::GetInstance()->GetAppDataPath();
	resPath.Append("result.txt");

	std::string after;
	std::wstring before(resPath.GetPointer());
	after.assign(before.begin(), before.end());

	return after.c_str();
}

bool SvmWrapper::init()
{
	if((model=svm_load_model(GetModelPath().c_str()))==0)
	{
		AppLog("can't open model file %s\n",GetModelPath().c_str());
		return false;
	}

	x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));
	if(predict_probability)
	{
		if(svm_check_probability_model(model)==0)
		{
			AppLog("Model does not support probabiliy estimates\n");
			return false;
		}
	}
	else
	{
		if(svm_check_probability_model(model)!=0)
			AppLog("Model supports probability estimates, but disabled in prediction.\n");
	}

	return true;
}

double SvmWrapper::predict(const std::vector<std::string>& input, FUNC func)
{
	if ( false == init() )
	{
		AppLog("svm init fail");
		return -1;
	}

	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type=svm_get_svm_type(model);
	int nr_class=svm_get_nr_class(model);

	double *prob_estimates=NULL;
	int j;

	if(predict_probability)
	{
		if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
		{
			func("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
		}
		else
		{
			int *labels=(int *) malloc(nr_class*sizeof(int));
			svm_get_labels(model,labels);
			prob_estimates = (double *) malloc(nr_class*sizeof(double));
			func("labels");
			for(j=0;j<nr_class;j++)
				func(" %d",labels[j]);
			func("\n");
			free(labels);
		}
	}

	max_line_len = 1024;

	double predict_label=0;
	for(std::vector<std::string>::const_iterator itr = input.begin();itr!=input.end();++itr )
	{
		char line[1024] ={0,};
		strcpy( line, (*itr).c_str());

		int i = 0;
		double target_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0

		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			func("Wrong input format at line %d\n", total+1);

		target_label = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			func("Wrong input format at line %d\n", total+1);

		while(1)
		{
			if(i>=max_nr_attr-1)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct svm_node *) realloc(x,max_nr_attr*sizeof(struct svm_node));
			}

			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
				func("Wrong input format at line %d\n", total+1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				func("Wrong input format at line %d\n", total+1);

			++i;
		}
		x[i].index = -1;

		if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
		{
			predict_label = svm_predict_probability(model,x,prob_estimates);
			func("%g",predict_label);
			for(j=0;j<nr_class;j++)
				func(" %g",prob_estimates[j]);
			func("\n");
		}
		else
		{
			predict_label = svm_predict(model,x);
			func("%g\n",predict_label);
		}

		if(predict_label == target_label)
			++correct;
		error += (predict_label-target_label)*(predict_label-target_label);
		sump += predict_label;
		sumt += target_label;
		sumpp += predict_label*predict_label;
		sumtt += target_label*target_label;
		sumpt += predict_label*target_label;
		++total;

	}

	if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
	{
		func("Mean squared error = %g (regression)\n",error/total);
		func("Squared correlation coefficient = %g (regression)\n",
			((total*sumpt-sump*sumt)*(total*sumpt-sump*sumt))/
			((total*sumpp-sump*sump)*(total*sumtt-sumt*sumt))
			);
	}
	else
		func("Accuracy = %g%% (%d/%d) (classification)\n",
			(double)correct/total*100,correct,total);
	if(predict_probability)
		free(prob_estimates);

	svm_free_and_destroy_model(&model);
	free(x);

	return predict_label;
}

/*
void predict(FILE *input, FILE *output)
{
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type=svm_get_svm_type(model);
	int nr_class=svm_get_nr_class(model);
	double *prob_estimates=NULL;
	int j;

	if(predict_probability)
	{
		if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
			info("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
		else
		{
			int *labels=(int *) malloc(nr_class*sizeof(int));
			svm_get_labels(model,labels);
			prob_estimates = (double *) malloc(nr_class*sizeof(double));
			fprintf(output,"labels");
			for(j=0;j<nr_class;j++)
				fprintf(output," %d",labels[j]);
			fprintf(output,"\n");
			free(labels);
		}
	}

	max_line_len = 1024;
	line = (char *)malloc(max_line_len*sizeof(char));
	while(readline(input) != NULL)
	{
		int i = 0;
		double target_label, predict_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0

		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(total+1);

		target_label = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(total+1);

		while(1)
		{
			if(i>=max_nr_attr-1)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct svm_node *) realloc(x,max_nr_attr*sizeof(struct svm_node));
			}

			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
				exit_input_error(total+1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(total+1);

			++i;
		}
		x[i].index = -1;

		if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
		{
			predict_label = svm_predict_probability(model,x,prob_estimates);
			fprintf(output,"%g",predict_label);
			for(j=0;j<nr_class;j++)
				fprintf(output," %g",prob_estimates[j]);
			fprintf(output,"\n");
		}
		else
		{
			predict_label = svm_predict(model,x);
			fprintf(output,"%g\n",predict_label);
		}

		if(predict_label == target_label)
			++correct;
		error += (predict_label-target_label)*(predict_label-target_label);
		sump += predict_label;
		sumt += target_label;
		sumpp += predict_label*predict_label;
		sumtt += target_label*target_label;
		sumpt += predict_label*target_label;
		++total;
	}
	if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
	{
		info("Mean squared error = %g (regression)\n",error/total);
		info("Squared correlation coefficient = %g (regression)\n",
			((total*sumpt-sump*sumt)*(total*sumpt-sump*sumt))/
			((total*sumpp-sump*sump)*(total*sumtt-sumt*sumt))
			);
	}
	else
		info("Accuracy = %g%% (%d/%d) (classification)\n",
			(double)correct/total*100,correct,total);
	if(predict_probability)
		free(prob_estimates);
}
*/
