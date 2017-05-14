/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "de.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------- derfcnd namespace - Differential Evolution methods -------------------------------*/
namespace derfcnd
{
/*------------------------------- Differential Evolution Parameters methods definition ------------------------------*/
/* Constructors */
DEParams::DEParams(void)
{
	setParams(N_POP, N_ITER, F, C);
}
DEParams::DEParams(unsigned int n_pop)
{
	setParams(n_pop, N_ITER, F, C);
}
DEParams::DEParams(unsigned int n_pop, unsigned int n_iter)
{
	setParams(n_pop, n_iter, F, C);
}
DEParams::DEParams(float f)
{
	setParams(N_POP, N_ITER, f, C);
}
DEParams::DEParams(float f, float c)
{
	setParams(N_POP, N_ITER, f, c);
}
DEParams::DEParams(unsigned int n_pop, float f)
{
	setParams(n_pop, N_ITER, f, C);
}
DEParams::DEParams(unsigned int n_pop, float f, float c)
{
	setParams(n_pop, N_ITER, f, c);
}
DEParams::DEParams(unsigned int n_pop, unsigned int n_iter, float f)
{
	setParams(n_pop, n_iter, f, C);
}
DEParams::DEParams(unsigned int n_pop, unsigned int n_iter, float f, float c)
{
	setParams(n_pop, n_iter, f, c);
}
DEParams::DEParams(const DEParams &de_params)
{
	setParams(de_params);
}
/* Destructor */
DEParams::~DEParams(void)
{
	clear();
}

/* Clear method */
void DEParams::clear(void)
{
	setParams(0, 0, 0, 0);
}

/* Get methods */
unsigned int DEParams::getNPop(void) const
{
	return n_pop_;
}
unsigned int DEParams::getNIter(void) const
{
	return n_iter_;
}
float DEParams::getF(void) const
{
	return f_;
}
float DEParams::getC(void) const
{
	return c_;
}

/* Set methods */
void DEParams::setNPop(unsigned int n_pop)
{
	n_pop_=n_pop;
}
void DEParams::setNIter(unsigned int n_iter)
{
	n_iter_=n_iter;
}
void DEParams::setF(float f)
{
	f_=f;
}
void DEParams::setC(float c)
{
	c_=c;
}
void DEParams::setParams(unsigned int n_pop, unsigned int n_iter, float f, float c)
{
	n_pop_=n_pop;
	n_iter_=n_iter;
	f_=f;
	c_=c;
}
void DEParams::setParams(const DEParams &de_params)
{
	n_pop_=de_params.n_pop_;
	n_iter_=de_params.n_iter_;
	f_=de_params.f_;
	c_=de_params.c_;
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------ Differential Evolution methods definition ------------------------------------*/
/* Randomly indexes selection */
void DE::selectIndex(unsigned int i, unsigned int *i1, unsigned int *i2,
										 unsigned int *i3, unsigned int *i4, unsigned int *i5)
{
	unsigned int n_pop=de_params_.getNPop();
	if(i1)
	{
		do
		{
			*i1=randomUniform((unsigned int) 0, n_pop);
  	} while(*i1==i);

  	if(i2)
  	{
  		do
  		{
  			*i2=randomUniform((unsigned int) 0, n_pop);
  		} while(*i2==i || *i2==*i1);

  		if(i3)
  		{
  			do
  			{
    			*i3=randomUniform((unsigned int) 0, n_pop);
  			} while(*i3==i || *i3==*i1 || *i3==*i2);

  			if(i4)
  			{
  				do
  				{
    				*i4=randomUniform((unsigned int) 0, n_pop);
  				} while(*i4==i || *i4==*i1 || *i4==*i2 || *i4==*i3);

  				if(i5)
  				{
  					do
  					{
    					*i5=randomUniform((unsigned int) 0, n_pop);
  					} while(*i5==i || *i5==*i1 || *i5==*i2 || *i5==*i3 || *i5==*i4);
  				}
  			}
  		}
  	}
	}
}

/* Constructors */
DE::DE(void)
	: de_params_(N_POP, N_ITER, F, C), pop_(0, N_POP), mut_(0, N_POP), evaluation_(N_POP)
{}
DE::DE(unsigned int len)
	: de_params_(N_POP, N_ITER, F, C), pop_(len, N_POP), mut_(len, N_POP), evaluation_(N_POP)
{}
DE::DE(unsigned int len, unsigned int n_pop)
	: de_params_(n_pop, N_ITER, F, C), pop_(len, n_pop), mut_(len, n_pop), evaluation_(n_pop)
{}
DE::DE(unsigned int len, unsigned int n_pop, unsigned int n_iter)
	: de_params_(n_pop, n_iter, F, C), pop_(len, n_pop), mut_(len, n_pop), evaluation_(n_pop)
{}
DE::DE(unsigned int len, float f)
	: de_params_(N_POP, N_ITER, f, C), pop_(len, N_POP), mut_(len, N_POP), evaluation_(N_POP)
{}
DE::DE(unsigned int len, float f, float c)
	: de_params_(N_POP, N_ITER, f, c), pop_(len, N_POP), mut_(len, N_POP), evaluation_(N_POP)
{}
DE::DE(unsigned int len, unsigned int n_pop, float f)
	: de_params_(n_pop, N_ITER, f, C), pop_(len, n_pop), mut_(len, n_pop), evaluation_(n_pop)
{}
DE::DE(unsigned int len, unsigned int n_pop, float f, float c)
	: de_params_(n_pop, N_ITER, f, c), pop_(len, n_pop), mut_(len, n_pop), evaluation_(n_pop)
{}
DE::DE(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f)
	: de_params_(n_pop, n_iter, f, C), pop_(len, n_pop), mut_(len, n_pop), evaluation_(n_pop)
{}
DE::DE(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f, float c)
	: de_params_(n_pop, n_iter, f, c), pop_(len, n_pop), mut_(len, n_pop), evaluation_(n_pop)
{}
DE::DE(unsigned int len, const DEParams &de_params)
	: de_params_(de_params),
		pop_(len, de_params.getNPop()), mut_(len, de_params.getNPop()), evaluation_(de_params.getNPop())
{}
/* Destructor */
DE::~DE(void)
{
	clear();
}

/* Clear method */
void DE::clear(void)
{
	setParams(0, 0, 0, 0, 0);
}

/* Get methods */
unsigned int DE::getNPop(void) const
{
	return de_params_.getNPop();
}
unsigned int DE::getNIter(void) const
{
	return de_params_.getNIter();
}
float DE::getF(void) const
{
	return de_params_.getF();
}
float DE::getC(void) const
{
	return de_params_.getC();
}
MatrixXf DE::getPop(void) const
{
	return pop_;
}
MatrixXf DE::getMut(void) const
{
	return mut_;
}
RowVectorXf DE::getEvaluation(void) const
{
	return evaluation_;
}
float DE::getBestIndividual(Ref<VectorXf> best_individual) const
{
	VectorXf::Index minInd;
	float best_evaluation=evaluation_.minCoeff(&minInd);
	best_individual=pop_.col(minInd);
	return best_evaluation;
}

/* Set methods */
void DE::setLen(unsigned int len)
{
	unsigned int n_pop=de_params_.getNPop();
	pop_.resize(len, n_pop);
	mut_.resize(len, n_pop);
	evaluation_.resize(n_pop);
}
void DE::setNPop(unsigned int n_pop)
{
	de_params_.setNPop(n_pop);
	pop_.resize(pop_.rows(), n_pop);
	mut_.resize(mut_.rows(), n_pop);
	evaluation_.resize(n_pop);
}
void DE::setNIter(unsigned int n_iter)
{
	de_params_.setNIter(n_iter);
}
void DE::setF(float f)
{
	de_params_.setF(f);
}
void DE::setC(float c)
{
	de_params_.setC(c);
}
void DE::setParams(unsigned int len, unsigned int n_pop, unsigned int n_iter, float f, float c)
{
	de_params_.setParams(n_pop, n_iter, f, c);
	setLen(len);
}
void DE::setParams(unsigned int len, const DEParams &de_params)
{
	de_params_.setParams(de_params);
	setLen(len);
}

/* Mutation and Recombination strategy */
void DE::strategy(void)
{
	unsigned int i, j, i1, i2, i3, sigma, len=pop_.rows();
	float f=de_params_.getF(), c=de_params_.getC();

	mut_=pop_;

	/* for the entire population */
	for(i=0; i<de_params_.getNPop(); i++)
	{
		/* i, i1, i2, i3 must be all distinct */
		selectIndex(i, &i1, &i2, &i3);
    sigma=randomUniform((unsigned int) 0, len);

    /* mutation and recombination */
    for(j=0; j<len; j++)
    {
    	if(randomUniform(0.0, 1.0)<c || sigma==j)
    	{
   			mut_(j, i)=pop_(j, i1)+f*(pop_(j, i2)-pop_(j, i3));
   		}
    }
	}
}

/* Randomly initialization of the population */
void DE::init(const Ref<const VectorXf> &min_val, const Ref<const VectorXf> &max_val)
{
	unsigned int i, len=pop_.rows();
	RowVectorXf pop_row(pop_.cols());

	/* population randomly initialization within the specified range */
	for(i=0; i<len; i++)
	{
		randomUniform(min_val(i), max_val(i), pop_row);
		pop_.row(i)=pop_row;
	}
	/* initial evaluation of the population*/
	for(i=0; i<de_params_.getNPop(); i++)
	{
		evaluation_(i)=objectiveFunc(pop_.col(i));
	}
}
/* Mutation and Recombination of the population */
void DE::mutRecomb(void)
{
	char op=0;
	float f=de_params_.getF();

	if(f<=0||2<=f)
	{
		de_params_.setF(randomUniform(0.5, 1.0));
		op=1;
	}

	/* mutation and recombination strategy */
	strategy();

	if(op==1)
	{
		de_params_.setF(3.0);
	}
}
/* Selection method */
void DE::selection(void)
{
	unsigned int i;
	float individual_evaluation;

	/* population selection */
	for(i=0; i<de_params_.getNPop(); i++)
	{
		/* mutated individual selection */
		individual_evaluation=objectiveFunc(mut_.col(i));
		if(individual_evaluation<evaluation_(i))
		{
			pop_.col(i)=mut_.col(i);
			evaluation_(i)=individual_evaluation;
		}
	}
}
/* Solve method */
float DE::solve(Ref<VectorXf> best_individual)
{
	unsigned int i;

	for(i=0; i<de_params_.getNIter(); i++)
	{
		/* population mutation and recombination */
		mutRecomb();
		/* population selection */
		selection();
	}
	/* return best individual found */
	return getBestIndividual(best_individual);
}
float DE::solve(Ref<VectorXf> best_individual,
								const Ref<const VectorXf> &min_val, const Ref<const VectorXf> &max_val)
{
	/* population randomly initialization */
	init(min_val, max_val);
	/* solve */
	return solve(best_individual);
}
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/