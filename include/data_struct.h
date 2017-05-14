#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <string>
#include <vector>
#include <Eigen/Core>
#include "files.h"
using std::string;
using std::vector;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------- rfcnd namespace - Data Structure definition -----------------------------------*/
namespace rfcnd
{
/*-------------------------------------------- Data Structure definition --------------------------------------------*/
class DataStruct
{
private:

public:

	MatrixXf x_c_;
	MatrixXi x_d_;
	VectorXi x_class_;
	vector< vector<string> > x_d_lbl_;
	vector<string> class_lbl_;

	/* Constructors */
	DataStruct(void);
	DataStruct(unsigned int nrow, unsigned int ncol_c,  unsigned int ncol_d, unsigned int nclass);
	DataStruct(const DataStruct &data);
	/* Destructor */
	~DataStruct(void);

	/* Clear method */
	void clear(void);

	/* Resize methods */
	void resize(unsigned int nrow, unsigned int ncol_c,  unsigned int ncol_d, unsigned int nclass);
	void resize(const DataStruct &data);

	/* Read method */
	char read(const string &fpath, const string &fname);
	/* Write method */
	char write(const string &fpath, const string &fname) const;
};
/*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------- Training Set Data Structure definition -------------------------------------*/
class TrainingSetData
{
private:

public:

	unsigned int nrow_, ncol_c_, ncol_d_, n_rand_var_;
	vector< vector<string> > x_d_lbl_;
	vector<string> class_lbl_;

	/* Constructors */
	TrainingSetData(void);
	TrainingSetData(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
									unsigned int nclass);
	TrainingSetData(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
									const vector< vector<string> > &x_d_lbl, const vector<string> &class_lbl);
	TrainingSetData(const TrainingSetData &training_set_data);
	/* Destructor */
	~TrainingSetData(void);

	/* Clear method */
	void clear(void);

	/* Resize methods */
	void resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
							unsigned int nclass);
	void resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
							const vector< vector<string> > &x_d_lbl, const vector<string> &class_lbl);
	void resize(const TrainingSetData &training_set_data);

	/* Read method */
	void read(ifstream &fin);
	void readBin(ifstream &fin);
	/* Write method */
	void write(ofstream &fout) const;
	void writeBin(ofstream &fout);
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* DATA_STRUCT_H */