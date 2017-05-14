/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "basic_stats.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------- derfcnd namespace - Basic Stats methods -------------------------------------*/
namespace derfcnd
{
/* Generate confusion matrix */
void confusionMatrix(const Ref<const VectorXi> &ground_truth, const Ref<const MatrixXf> &certainty,
										 float thresh, Ref<MatrixXi> confmat)
{
	float max_certainty;
	unsigned int i, nclass=confmat.rows();
	RowVectorXf::Index max_ind;

	confmat.setZero();

	for(i=0; i<ground_truth.size(); i++)
	{
		max_certainty=certainty.row(i).maxCoeff(&max_ind);
		if(max_certainty>=thresh)
		{
			confmat(ground_truth(i), max_ind)++;
		}
		else
		{
			confmat(ground_truth(i), nclass)++;
		}
	}
}

/* Compute precision based on the confusion matrix */
void precision(const Ref<const MatrixXi> &confmat, Ref<VectorXf> prec)
{
	unsigned int nclass=confmat.rows();
	VectorXi nper_col(nclass);

	nper_col=confmat.leftCols(nclass).colwise().sum();
	prec=confmat.diagonal().array().cast<float>()/nper_col.array().cast<float>();
}

/* Compute recall based on the confusion matrix */
void recall(const Ref<const MatrixXi> &confmat, Ref<VectorXf> rec)
{
	unsigned int nclass=confmat.rows();
	VectorXi nper_row(nclass);

	nper_row=confmat.leftCols(nclass).rowwise().sum();
	rec=confmat.diagonal().array().cast<float>()/nper_row.array().cast<float>();
}

/* Write results to file */
char writeResults(const string &fpath, const string &fname, const Ref<const MatrixXi> &confmat,
									const Ref<const VectorXf> &prec, const Ref<const VectorXf> &rec, const vector<string> &class_lbl,
									float train_time, float test_time, unsigned int size_model)
{
	char class_char;
	unsigned int i, nclass=class_lbl.size(), digits;
	string fname_path=fpath;
	fname_path+=fname;
	ofstream fout(fname_path.c_str(), ios::out);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	/* setting precision of the formatted output */
	fout<<setprecision(4);
	for(i=1, digits=class_lbl[0].size(); i<nclass; i++)
	{
		if(digits<class_lbl[i].size())
		{
			digits=class_lbl[i].size();
		}
	}

	/* writing the results */
	fout<<"Time taken to build model: "<<train_time<<" sec"<<endl;
	fout<<"Time taken to test model on supplied test set: "<<test_time*1000<<" msec"<<endl;
	fout<<"Space required to store model: "<<size_model<<" Byte"<<endl<<endl;
	fout<<"=== Precision and Recall by Class ==="<<endl<<endl;
	fout<<setw(19)<<"Precision"<<setw(7)<<"Recall"<<setw(7)<<"Class"<<endl;
	for(i=0, class_char='a'; i<nclass; i++, class_char++)
	{
		fout<<setw(15)<<prec(i)*100<<setw(10)<<rec(i)*100<<setw(digits+3)<<class_lbl[i]<<setw(3)<<"("<<class_char<<")"<<endl;
	}
	fout<<setw(10)<<"average "<<prec.mean()*100<<setw(10)<<rec.mean()*100<<endl<<endl;
	digits=numDigits(confmat.maxCoeff())+1;
	fout<<"=== Confusion Matrix ==="<<endl<<endl<<" ";
	for(i=0, class_char='a'; i<nclass; i++, class_char++)
	{
		fout<<setw(digits)<<class_char;
	}
	fout<<setw(digits)<<"U"<<setw(19)<<"<-- classified as"<<endl;
	for(i=0, class_char='a'; i<nclass; i++, class_char++)
	{
		fout<<setw(2)<<confmat.row(i)<<setw(3)<<"|"<<setw(3)<<class_char<<endl;
	}

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
char writeResults(const string &fpath, const string &fname)
{
	string fname_path=fpath;
	fname_path+=fname;
	ofstream fout(fname_path.c_str(), ios::out);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	fout<<setw(7)<<"A"<<setw(11)<<"Precision"<<setw(7)<<"Recall"<<setw(7)<<"Class"<<endl;

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
char writeResults(const string &fpath, const string &fname, char a,
									const Ref<const VectorXf> &prec, const Ref<const VectorXf> &rec, const vector<string> &class_lbl)
{
	unsigned int i, nclass=class_lbl.size(), digits;
	string fname_path=fpath;
	fname_path+=fname;
	ofstream fout(fname_path.c_str(), ios::out | ios::app);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	/* setting precision of the formatted output */
	fout<<setprecision(4);
	for(i=1, digits=class_lbl[0].size(); i<nclass; i++)
	{
		if(digits<class_lbl[i].size())
		{
			digits=class_lbl[i].size();
		}
	}

	/* writing the results */
	for(i=0; i<nclass; i++)
	{
		fout<<setw(7)<<a<<setw(7)<<prec(i)*100<<setw(10)<<rec(i)*100<<setw(digits+3)<<class_lbl[i]<<endl;
	}
	fout<<"average  "<<prec.mean()*100<<setw(10)<<rec.mean()*100<<endl;

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
char writeResults(const string &fpath, const string &fname, float prec, float rec,
									float train_time, float test_time, unsigned int size_model)
{
	string fname_path=fpath;
	fname_path+=fname;
	ofstream fout(fname_path.c_str(), ios::out | ios::app);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	/* setting precision of the formatted output */
	fout<<setprecision(4);

	/* writing the results */
	fout<<endl<<"average  "<<prec*100<<setw(10)<<rec*100<<endl<<endl;
	fout<<"Time taken to build model, per tree: "<<train_time*1000<<" msec"<<endl;
	fout<<"Time taken to test model on supplied test set, per tree: "<<test_time*1000<<" msec"<<endl;
	fout<<"Space required to store model, per tree: "<<size_model<<" Byte"<<endl;

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}

/* Determine the size of a file */
unsigned int sizeFile(const string &fpath, const string &fname)
{
	unsigned int size;
	string fname_path=fpath;
	fname_path+=fname;
	ifstream fin(fname_path.c_str(), ios::in | ios::ate | ios::binary);

	/* opening file failure */
	if(fin.fail())
	{
		return FOPEN_FAIL;
	}

	/*determining the size of the file */
	size=fin.tellg();
	/* closing file */
	fin.close();
	return size;
}
}
/*-------------------------------------------------------------------------------------------------------------------*/