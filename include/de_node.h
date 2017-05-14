#ifndef DE_NODE_H
#define DE_NODE_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <Eigen/Core>
#include "de.h"
#include "impurity.h"
#include "split_node.h"
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*-------------- derfcnd namespace - Differential Evolution as splitting Node meta-heuristic definition -------------*/
namespace derfcnd
{
/*------------------------ Differential Evolution as splitting Node meta-heuristic definition -----------------------*/
class DENode : public DE
{
private:

	MatrixXf x_c_;
	VectorXi x_class_, lclass_, rclass_;
	Impurity node_impurity_;

public:

	/* Constructors */
	DENode(void);
	DENode(unsigned int len);
	DENode(unsigned int len, unsigned int n_pop);
	DENode(unsigned int len, unsigned int n_pop, unsigned int n_iter);
	DENode(unsigned int len, float f);
	DENode(unsigned int len, float f, float c);
	DENode(unsigned int len, unsigned int n_pop, float f);
	DENode(unsigned int len, unsigned int n_pop, float f, float c);
	DENode(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f);
	DENode(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f, float c);
	DENode(unsigned int len, const DEParams &de_params);
	/* Destructor */
	~DENode(void);

	/* Set method */
	void setParams(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &x_class,
								 const Ref<const VectorXi> &cclass);

	/* Objective function to minimize */
	float objectiveFunc(const Ref<const VectorXf> &individual);

	/* Randomly initialization of the population */
	void init(const Ref<const VectorXf> &min_val, const Ref<const VectorXf> &max_val);
	/* Solve method */
	float solve(Ref<VectorXf> best_individual);
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* DE_NODE_H */