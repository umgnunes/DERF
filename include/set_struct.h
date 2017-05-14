#ifndef SET_STRUCT_H
#define SET_STRUCT_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <string>
#include <vector>
#include <Eigen/Core>
#include "files.h"
using std::string;
using std::vector;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------- derfcnd namespace - Set Structure definition ----------------------------------*/
namespace derfcnd
{
/*----------------------------------------------- Set Data definition -----------------------------------------------*/
class SetData
{
private:

public:

	MatrixXf x_c_;
	MatrixXi x_d_;
	VectorXi x_class_;
	vector< vector<string> > x_d_lbl_;
	vector<string> class_lbl_;

	/* Constructors */
	SetData(void);
	SetData(unsigned int nrow, unsigned int ncol_c,  unsigned int ncol_d, unsigned int nclass);
	SetData(const SetData &data);
	/* Destructor */
	~SetData(void);

	/* Clear method */
	void clear(void);

	/* Resize methods */
	void resize(unsigned int nrow, unsigned int ncol_c,  unsigned int ncol_d, unsigned int nclass);
	void resize(const SetData &data);

	/* Read method */
	char read(const string &fpath, const string &fname);
	/* Write method */
	char write(const string &fpath, const string &fname) const;
};
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------- Set Structure definition --------------------------------------------*/
class SetStruct
{
private:

public:

	unsigned int nrow_, ncol_c_, ncol_d_, n_rand_var_;
	vector< vector<string> > x_d_lbl_;
	vector<string> class_lbl_;

	/* Constructors */
	SetStruct(void);
	SetStruct(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d,
						unsigned int n_rand_var, unsigned int nclass);
	SetStruct(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
						const vector< vector<string> > &x_d_lbl, const vector<string> &class_lbl);
	SetStruct(const SetStruct &set_struct);
	/* Destructor */
	~SetStruct(void);

	/* Clear method */
	void clear(void);

	/* Resize methods */
	void resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d,
							unsigned int n_rand_var, unsigned int nclass);
	void resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
							const vector< vector<string> > &x_d_lbl, const vector<string> &class_lbl);
	void resize(const SetStruct &set_struct);

	/* Read methods */
	void read(ifstream &fin);
	void readBin(ifstream &fin);
	/* Write methods */
	void write(ofstream &fout) const;
	void writeBin(ofstream &fout);
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* SET_STRUCT_H */