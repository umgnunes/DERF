#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <string>
#include <vector>
#include <Eigen/Core>
#include "files.h"
#include "set_struct.h"
#include "de.h"
#include "de_node.h"
#include "random_tree.h"
using std::string;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------- derfcnd namespace - Random Forest definition -----------------------------------*/
namespace derfcnd
{
/*------------------------------------------ Constant variables definition ------------------------------------------*/
/* Default number of trees of the forest */
#define N_TREES 100
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------- Random Forest definition --------------------------------------------*/
class RandomForest
{
private:

	float conf_lvl_;
	unsigned int n_trees_;
	RandomTree *trees_;
	SetStruct set_struct_;
	DEParams de_params_;

	/* Allocate space for n_trees trees in the forest */
	void allocate(unsigned int n_trees);

public:

	/* Constructors */
	RandomForest(void);
	RandomForest(float conf_lvl);
	RandomForest(unsigned int n_pop, unsigned int n_iter, float f, float c);
	RandomForest(unsigned int n_pop, unsigned int n_iter, float f, float c, float conf_lvl);
	RandomForest(const DEParams &de_params);
	RandomForest(const DEParams &de_params, float conf_lvl);
	/* Destructor */
	~RandomForest(void);

	/* Clear method */
	void clear(void);

	/* Get methods */
	float getConfLvl(void) const;
	unsigned int getNTrees(void) const;
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
	void generate(const SetData &set_data);
	void generate(const SetData &set_data, unsigned int n_trees);
	void generate(const SetData &set_data, unsigned int n_trees, unsigned int n_rand_var);
	/* Test random decision tree */
	void test(const Ref<const RowVectorXf> &x_c_row, const Ref<const RowVectorXi> &x_d_row, Ref<RowVectorXf> certainty);
	void test(const SetData &set_data, Ref<MatrixXf> certainty);

	/* Read model methods */
	void read(ifstream &fin, const SetStruct &set_struct, unsigned int n_trees);
	char read(const string &fpath, const string &fname);
	void readBin(ifstream &fin, const SetStruct &set_struct, unsigned int n_trees);
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

#endif /* RANDOM_FOREST_H */