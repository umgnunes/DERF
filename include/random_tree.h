#ifndef RANDOM_TREE_H
#define RANDOM_TREE_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <string>
#include <vector>
#include <Eigen/Core>
#include "files.h"
#include "basic_stats.h"
#include "set_struct.h"
#include "de.h"
#include "de_node.h"
#include "impurity.h"
#include "node.h"
#include "split_node.h"
using std::string;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------- derfcnd namespace - Random Decision Tree definition -------------------------------*/
namespace derfcnd
{
/*------------------------------------------ Constant variables definition ------------------------------------------*/
/* Chi-squared confidence level */
/* 0.1-2.71; 0.05-3.64; 0.025-5.02; 0.01-6.63; 0.005-7.88; 0.001-10.8 */
#define CONF_LVL 6.63
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------- Random Decision Tree definition -----------------------------------------*/
class RandomTree
{
private:

	float conf_lvl_;
	Node *root_;
	SetStruct set_struct_;
	DEParams de_params_;

	/* Clear method */
	void clear(Node *node);

	/* Find best splits */
	float findBestContinuousSplit(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &x_class,
																const Ref<const VectorXi> &cclass, Ref<VectorXf> split_val);
	float findBestDiscreteSplit(const Ref<const MatrixXi> &x_d, const Ref<const VectorXi> &x_class,
															const Ref<const VectorXi> &cclass, int *split_coef, int *split_var);
	/* Generate random decision tree recursively */
	void generate(Node *node, const SetData &set_data, const Ref<const VectorXi> &row_ind);
	/* Test random decision tree */
	void test(Node *node, const Ref<const RowVectorXf> &x_c_row, const Ref<const RowVectorXi> &x_d_row,
						Ref<RowVectorXf> certainty);

	/* Read model methods */
	void read(Node *node, ifstream &fin);
	void readBin(Node *node, ifstream &fin);
	/* Write model methods */
	void write(Node *node, ofstream &fout) const;
	void writeBin(Node *node, ofstream &fout);

public:

	/* Constructors */
	RandomTree(void);
	RandomTree(float conf_lvl);
	RandomTree(unsigned int n_pop, unsigned int n_iter, float f, float c);
	RandomTree(unsigned int n_pop, unsigned int n_iter, float f, float c, float conf_lvl);
	RandomTree(const DEParams &de_params);
	RandomTree(const DEParams &de_params, float conf_lvl);
	/* Destructor */
	~RandomTree(void);

	/* Clear method */
	void clear(void);

	/* Get methods */
	float getConfLvl(void) const;
	unsigned int getNRow(void) const;
	unsigned int getNColC(void) const;
	unsigned int getNColD(void) const;
	unsigned int getNRandVar(void) const;
	vector< vector<string> > getXDLabel(void) const;
	vector<string> getClassLabel(void) const;
	unsigned int getDEParamNPop(void) const;
	unsigned int getDEParamNIter(void) const;
	float getDEParamF(void) const;
	float getDEParamC(void) const;

	/* Set methods */
	void setConfLvl(float conf_lvl);
	void setDEParamNPop(unsigned int n_pop);
	void setDEParamNIter(unsigned int n_iter);
	void setDEParamF(float f);
	void setDEParamC(float c);
	void setDEParams(unsigned int n_pop, unsigned int n_iter, float f, float c);
	void setDEParams(const DEParams &de_params);

	/* Generate random decision tree */
	void generate(const SetData &set_data, const Ref<const VectorXi> &row_ind, unsigned int n_rand_var);
	/* Test random decision tree */
	void test(const Ref<const RowVectorXf> &x_c_row, const Ref<const RowVectorXi> &x_d_row, Ref<RowVectorXf> certainty);
	void test(const SetData &set_data, Ref<MatrixXf> certainty);

	/* Read model methods */
	void read(ifstream &fin, const SetStruct &set_struct);
	char read(const string &fpath, const string &fname);
	void readBin(ifstream &fin, const SetStruct &set_struct);
	char readBin(const string &fpath, const string &fname);
	/* Write model methods */
	void write(ofstream &fout) const;
	char write(const string &fpath, const string &fname) const;
	void writeBin(ofstream &fout);
	char writeBin(const string &fpath, const string &fname);
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* RANDOM_TREE_H */