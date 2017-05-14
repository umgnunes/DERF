/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "impurity.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------- derfcnd namespace - Impurity methods --------------------------------------*/
namespace derfcnd
{
/*------------------------------------------- Impurity methods definition -------------------------------------------*/
/* Constructors */
Impurity::Impurity(void)
{
	impurity_=0;
	drop_impurity_=0;
}
/* Destructor */
Impurity::~Impurity(void)
{}

/* Calculates the value of impurity, based on Entropy impurity */
float Impurity::entropy(const Ref<const VectorXi> &cclass, unsigned int nx_class)
{
	if(nx_class==0)
	{
		return 0;
	}

	unsigned int i, nclass=cclass.size();
	VectorXf cclass_log2(nclass);

	for(i=0; i<nclass; i++)
	{
		cclass(i)>0 ? cclass_log2(i)=log2(cclass(i)) : cclass_log2(i)=0;
	}
	/* computation of the Entropy impurity */
	impurity_=cclass.transpose().cast<float>()*cclass_log2;
	impurity_=-(impurity_/nx_class)+log2(nx_class);
	return impurity_;
}
/* Calculates the value of impurity, based on Gini impurity */
float Impurity::gini(const Ref<const VectorXi> &cclass, unsigned int nx_class)
{
	if(nx_class==0)
	{
		return 0;
	}
	/* computation of the Gini impurity */
	impurity_=cclass.transpose()*cclass;
	impurity_=1-(impurity_/(nx_class*nx_class));
	return impurity_;
}
/* Calculates the value of impurity, based on Misclassification impurity */
float Impurity::misclassification(const Ref<const VectorXi> &cclass, unsigned int nx_class)
{
	if(nx_class==0)
	{
		return 0;
	}
	/* computation of the Misclassification impurity */
	impurity_=1-((float) cclass.maxCoeff()/nx_class);
	return impurity_;
}

/* Calculates the value of the drop in impurity */
float Impurity::drop(float limpurity, float rimpurity, float lcfrac)
{
	drop_impurity_=impurity_-lcfrac*limpurity-(1-lcfrac)*rimpurity;
	return drop_impurity_;
}

/* Get methods*/
float Impurity::get(void) const
{
	return impurity_;
}
float Impurity::getDrop(void) const
{
	return drop_impurity_;
}
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/