#ifndef DE_H
#define DE_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <cstdlib>
#include <Eigen/Core>
#include "basic_stats.h"
using namespace Eigen;
using namespace derfcnd;
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------ derfcnd namespace - Differential Evolution definition ------------------------------*/
namespace derfcnd
{
/*------------------------------------------ Constant variables definition ------------------------------------------*/
/* Number of individuals in population */
#define N_POP 20
/* Number of iterations of the algorithm */
#define N_ITER 6
/* Parameter for mutation */
#define F 3
/* Parameter for recombination */
#define C 0.9
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------- Differential Evolution Parameters definition ----------------------------------*/
class DEParams
{
private:
	unsigned int n_pop_;
	unsigned int n_iter_;
	float f_;
	float c_;

public:

	/* Constructors */
	DEParams(void);
	DEParams(unsigned int n_pop);
	DEParams(unsigned int n_pop, unsigned int n_iter);
	DEParams(float f);
	DEParams(float f, float c);
	DEParams(unsigned int n_pop, float f);
	DEParams(unsigned int n_pop, float f, float c);
	DEParams(unsigned int n_pop, unsigned int n_iter, float f);
	DEParams(unsigned int n_pop, unsigned int n_iter, float f, float c);
	DEParams(const DEParams &de_params);
	/* Destructor */
	~DEParams(void);

	/* Clear method */
	void clear(void);

	/* Get methods */
	unsigned int getNPop(void) const;
	unsigned int getNIter(void) const;
	float getF(void) const;
	float getC(void) const;

	/* Set methods */
	void setNPop(unsigned int n_pop);
	void setNIter(unsigned int n_iter);
	void setF(float f);
	void setC(float c);
	void setParams(unsigned int n_pop, unsigned int n_iter, float f, float c);
	void setParams(const DEParams &de_params);
};
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------ Differential Evolution abstract definition -----------------------------------*/
class DE
{
protected:

	DEParams de_params_;
	MatrixXf pop_;
	MatrixXf mut_;
	RowVectorXf evaluation_;

	/* Randomly indexes selection */
	void selectIndex(unsigned int i, unsigned int *i1=0, unsigned int *i2=0,
									 unsigned int *i3=0, unsigned int *i4=0, unsigned int *i5=0);

public:

	/* Constructors */
	DE(void);
	DE(unsigned int len);
	DE(unsigned int len, unsigned int n_pop);
	DE(unsigned int len, unsigned int n_pop, unsigned int n_iter);
	DE(unsigned int len, float f);
	DE(unsigned int len, float f, float c);
	DE(unsigned int len, unsigned int n_pop, float f);
	DE(unsigned int len, unsigned int n_pop, float f, float c);
	DE(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f);
	DE(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f, float c);
	DE(unsigned int len, const DEParams &de_params);
	/* Destructor */
	virtual ~DE(void);

	/* Clear method */
	void clear(void);

	/* Get methods */
	unsigned int getNPop(void) const;
	unsigned int getNIter(void) const;
	float getF(void) const;
	float getC(void) const;
	MatrixXf getPop(void) const;
	MatrixXf getMut(void) const;
	RowVectorXf getEvaluation(void) const;
	float getBestIndividual(Ref<VectorXf> best_individual) const;

	/* Set methods */
	void setLen(unsigned int len);
	void setNPop(unsigned int n_pop);
	void setNIter(unsigned int n_iter);
	void setF(float f);
	void setC(float c);
	void setParams(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f, float c);
	void setParams(unsigned int len, const DEParams &de_params);

	/* Objective function to minimize */
	virtual float objectiveFunc(const Ref<const VectorXf> &individual)=0;
	/* Mutation and Recombination strategy */
	virtual void strategy(void);

	/* Randomly initialization of the population */
	virtual void init(const Ref<const VectorXf> &min_val, const Ref<const VectorXf> &max_val);
	/* Mutation and Recombination of the population */
	void mutRecomb(void);
	/* Selection method */
	void selection(void);
	/* Solve methods */
	virtual float solve(Ref<VectorXf> best_individual);
	virtual float solve(Ref<VectorXf> best_individual,
											const Ref<const VectorXf> &min_val, const Ref<const VectorXf> &max_val);
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* DE_H */