/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <iostream>
#include <cmath>
#include <string>
#include <Eigen/Core>
#include "random_forest.h"
using std::cout;
using std::endl;
using std::string;
using namespace derfcnd;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

char generateRandomSet(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int nclass,
											 string fpath, string fname);

/*------------------------------------------------------- Main ------------------------------------------------------*/
int main(int argc, char *argv[])
{
	char s;
	unsigned int nrow_train=1000, nrow_test=1000, ncol_c=20, ncol_d=2, nclass=5;
	string fpath="", ftrain="ftrain.txt", ftest="ftest.txt", fmodel="fmodel.txt", fmodelbin="fmodel.bin";
	SetData training_set, testing_set;
	/* default parameters; identical to: RandomForest derf; */
	RandomForest derf(N_POP, N_ITER, F, C, CONF_LVL);
	VectorXf prec, rec;
	MatrixXf certainty;
	MatrixXi confmat;

	/* generate random training set */
	s=generateRandomSet(nrow_train, ncol_c, ncol_d, nclass, fpath, ftrain);
	if(!s)
	{
		cout<<"successful random set generation to file "<<fpath<<ftrain<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": random set generation to file "<<fpath<<ftrain<<endl;
		return 0;
	}
	/* generate random testing set */
	s=generateRandomSet(nrow_test, ncol_c, ncol_d, nclass, fpath, ftest);
	if(!s)
	{
		cout<<"successful random set generation to file "<<fpath<<ftest<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": random set generation to file "<<fpath<<ftest<<endl;
		return 0;
	}

	/* reading training set */
	s=training_set.read(fpath, ftrain);
	if(!s)
	{
		cout<<"successful training set reading from file "<<fpath<<ftrain<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": training set reading from file "<<fpath<<ftrain<<endl;
		return 0;
	}
	/* reading testing set */
	if(!testing_set.read(fpath, ftest))
	{
		cout<<"successful testing set reading from file "<<fpath<<ftest<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": testing set reading from file "<<fpath<<ftest<<endl;
		return 0;
	}

	/* generating DERF model */
	cout<<"generating DERF model..."<<endl;
	derf.generate(training_set, N_TREES, log2(ncol_c)+1);

	/* testing DERF model */
	cout<<"testing DERF model..."<<endl;
	certainty.resize(nrow_test, nclass);
	derf.test(testing_set, certainty);

	/* computing confusion matrix */
	confmat.resize(nclass, nclass+1);	
	confusionMatrix(testing_set.x_class_, certainty, 0, confmat);
	cout<<"confusion matrix:"<<endl<<confmat<<endl;
	/* computing precision */
	prec.resize(nclass);
	precision(confmat, prec);
	cout<<"precision:"<<endl<<prec.transpose()<<endl;
	/* computing recall */
	rec.resize(nclass);
	recall(confmat, rec);
	cout<<"recall:"<<endl<<rec.transpose()<<endl;

	/* writing DERF model */
	s=derf.write(fpath, fmodel);
	if(!s)
	{
		cout<<"successful model writing to file "<<fpath<<fmodel<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": model writing to file "<<fpath<<fmodel<<endl;
		return 0;
	}
	s=derf.writeBin(fpath, fmodelbin);
	if(!s)
	{
		cout<<"successful model writing to file "<<fpath<<fmodelbin<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": model writing to file "<<fpath<<fmodelbin<<endl;
		return 0;
	}

	/* reading DERF model */
	s=derf.read(fpath, fmodel);
	if(!s)
	{
		cout<<"successful model reading from file "<<fpath<<fmodel<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": model reading from file "<<fpath<<fmodel<<endl;
		return 0;
	}

	/* testing DERF model */
	cout<<"testing DERF model..."<<endl;
	certainty.resize(nrow_test, nclass);
	derf.test(testing_set, certainty);

	/* computing confusion matrix */
	confmat.resize(nclass, nclass+1);	
	confusionMatrix(testing_set.x_class_, certainty, 0, confmat);
	cout<<"confusion matrix:"<<endl<<confmat<<endl;
	/* computing precision */
	prec.resize(nclass);
	precision(confmat, prec);
	cout<<"precision:"<<endl<<prec.transpose()<<endl;
	/* computing recall */
	rec.resize(nclass);
	recall(confmat, rec);
	cout<<"recall:"<<endl<<rec.transpose()<<endl;

	s=derf.readBin(fpath, fmodelbin);
	if(!s)
	{
		cout<<"successful model reading from file "<<fpath<<fmodelbin<<endl;
	}
	else
	{
		cout<<"ERROR "<<(int) s<<": model reading from file "<<fpath<<fmodelbin<<endl;
		return 0;
	}

	/* testing DERF model */
	cout<<"testing DERF model..."<<endl;
	certainty.resize(nrow_test, nclass);
	derf.test(testing_set, certainty);

	/* computing confusion matrix */
	confmat.resize(nclass, nclass+1);	
	confusionMatrix(testing_set.x_class_, certainty, 0, confmat);
	cout<<"confusion matrix:"<<endl<<confmat<<endl;
	/* computing precision */
	prec.resize(nclass);
	precision(confmat, prec);
	cout<<"precision:"<<endl<<prec.transpose()<<endl;
	/* computing recall */
	rec.resize(nclass);
	recall(confmat, rec);
	cout<<"recall:"<<endl<<rec.transpose()<<endl;

	return 0;
}
/*-------------------------------------------------------------------------------------------------------------------*/

char generateRandomSet(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int nclass,
											 string fpath, string fname)
{
	unsigned int i, j;
	string lbl;
	SetData set_data(nrow, ncol_c, ncol_d, nclass);

	for(i=0; i<ncol_d; i++)
	{
		for(j=0; j<nclass; j++)
		{
			lbl="lbl";
			lbl+='1'+i;
			lbl+='1'+j;
			set_data.x_d_lbl_[i].push_back(lbl);
		}
	}
	for(i=0; i<nclass; i++)
	{
		lbl="class";
		lbl+='0'+i;
		set_data.class_lbl_[i]=lbl;
	}
	for(i=0; i<nrow; i++)
	{
		for(j=0; j<ncol_c; j++)
		{
			set_data.x_c_(i, j)=i*ncol_c+j;
		}
		for(j=0; j<ncol_d; j++)
		{
			set_data.x_d_(i, j)=rand()%set_data.x_d_lbl_[j].size();
		}
		set_data.x_class_(i)=rand()%nclass;
	}

	return set_data.write(fpath, fname);
}