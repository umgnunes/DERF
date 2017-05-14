/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "de_node.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------- derfcnd namespace - Differential Evolution as splitting Node meta-heuristic methods ---------------*/
namespace derfcnd
{
/*-------------------- Differential Evolution as splitting Node meta-heuristic methods definition -------------------*/
/* Constructors */
DENode::DENode(void) : DE()
{}
DENode::DENode(unsigned int len) : DE(len)
{}
DENode::DENode(unsigned int len, unsigned int n_pop) : DE(len, n_pop)
{}
DENode::DENode(unsigned int len, unsigned int n_pop, unsigned int n_iter) : DE(len, n_pop, n_iter)
{}
DENode::DENode(unsigned int len, float f) : DE(len, f)
{}
DENode::DENode(unsigned int len, float f, float c) : DE(len, f, c)
{}
DENode::DENode(unsigned int len, unsigned int n_pop, float f) : DE(len, n_pop, f)
{}
DENode::DENode(unsigned int len, unsigned int n_pop, float f, float c) : DE(len, n_pop, f, c)
{}
DENode::DENode(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f) : DE(len, n_pop, n_iter, f)
{}
DENode::DENode(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f, float c) : DE(len, n_pop, n_iter, f, c)
{}
DENode::DENode(unsigned int len, const DEParams &de_params) : DE(len, de_params)
{}
/* Destructor */
DENode::~DENode(void)
{}

/* Set method */
void DENode::setParams(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &x_class,
											 const Ref<const VectorXi> &cclass)
{
	unsigned int nclass=cclass.size();
	x_c_=x_c;
	x_class_=x_class;
	lclass_.resize(nclass);
	rclass_.resize(nclass);
	node_impurity_.entropy(cclass, x_class.size());
}

/* Objective function to minimize */
float DENode::objectiveFunc(const Ref<const VectorXf> &individual)
{
	unsigned int nrow=x_c_.rows(), lcounter;
	float lfrac;
	Impurity aux_impurity;

	lcounter=instancesGoingLeft(x_c_, x_class_, individual, lclass_, rclass_);
	lfrac=(float) lcounter/nrow;
	return -node_impurity_.drop(aux_impurity.entropy(lclass_, lcounter), aux_impurity.entropy(rclass_, nrow-lcounter), lfrac);
}

/* Randomly initialization of the population */
void DENode::init(const Ref<const VectorXf> &min_val, const Ref<const VectorXf> &max_val)
{
	unsigned int  i, n_pop=de_params_.getNPop();
	VectorXf diff_maxmin=max_val-min_val;

	/* population randomly initialization within the specified range */
	for(i=0; i<pop_.rows(); i++)
	{
		if(diff_maxmin(i)!=0)
		{
			pop_.row(i)=RowVectorXf::Random(n_pop)/diff_maxmin(i);
		}
		else
		{
			pop_.row(i)=RowVectorXf::Zero(n_pop);
		}
	}
	/* initial evaluation of the population*/
	for(i=0; i<n_pop; i++)
	{
		evaluation_(i)=objectiveFunc(pop_.col(i));
	}
}

/* Solve method */
float DENode::solve(Ref<VectorXf> best_individual)
{
	/* population randomly initialization */
	init(x_c_.colwise().minCoeff().transpose(), x_c_.colwise().maxCoeff().transpose());

	/* solve */
	for(unsigned int i=0; i<de_params_.getNIter(); i++)
	{
		/* population mutation and recombination */
		mutRecomb();
		/* population selection */
		selection();
	}
	/* return best individual found */
	return getBestIndividual(best_individual);
}
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/