/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "split_node.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------- derfcnd namespace - Split Node methods -------------------------------------*/
namespace derfcnd
{
/* Count the number of classes of the instances */
void countClasses(const Ref<const VectorXi> &x_class, Ref<VectorXi> cclass)
{
	/* set counters to 0*/
	cclass.setZero();

	/* count the number of classes of the training instances */
	for(unsigned int i=0; i<x_class.size(); i++)
	{
		cclass(x_class(i))++;
	}
}

/* Determine the number of instances going left, based on continuous variables */
unsigned int instancesGoingLeft(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &x_class,
																const Ref<const VectorXf> &split_val, Ref<VectorXi> lclass, Ref<VectorXi> rclass)
{
	unsigned int i, counter;

	/* set all counters to 0 */
	lclass.setZero();
	rclass.setZero();

	for(i=0, counter=0; i<x_class.size(); i++)
	{
		if(x_c.row(i)*split_val<SPLIT_COEF)
		{
			counter++;
			lclass(x_class(i))++;
		}
		else
		{
			rclass(x_class(i))++;
		}
	}

	return counter;
}
/* Determine the number of instances going left, based on discrete variables */
unsigned int instancesGoingLeft(const Ref<const VectorXi> &x_d, const Ref<const VectorXi> &x_class,
																int split_coef, Ref<VectorXi> lclass, Ref<VectorXi> rclass)
{
	unsigned int i, counter;

	/* set all counters to 0 */
	lclass.setZero();
	rclass.setZero();

	for(i=0, counter=0; i<x_class.size(); i++)
	{
		if(x_d(i)!=split_coef)
		{
			counter++;
			lclass(x_class(i))++;
		}
		else
		{
			rclass(x_class(i))++;
		}
	}

	return counter;
}

/* Split the node, by continuous variables */
void splitNode(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &row_ind,
							 const Ref<const VectorXf> &split_val, Ref<VectorXi> l_row_ind, Ref<VectorXi> r_row_ind)
{
	unsigned int i, l_i, r_i;

	for(i=0, l_i=0, r_i=0; i<row_ind.size(); i++)
	{
		/* falling left branch */
		if(x_c.row(i)*split_val<SPLIT_COEF)
		{
			l_row_ind(l_i)=row_ind(i);
			l_i++;
		}
		/* falling right branch */
		else
		{
			r_row_ind(r_i)=row_ind(i);
			r_i++;
		}
	}
}
/* Split the node, by discrete variables */
void splitNode(const Ref<const VectorXi> &x_d, const Ref<const VectorXi> &row_ind,
							 int split_coef, Ref<VectorXi> l_row_ind, Ref<VectorXi> r_row_ind)
{
	unsigned int i, l_i, r_i;

	for(i=0, l_i=0, r_i=0; i<row_ind.size(); i++)
	{
		/* falling left branch */
		if(x_d(i)!=split_coef)
		{
			l_row_ind(l_i)=row_ind(i);
			l_i++;
		}
		/* falling right branch */
		else
		{
			r_row_ind(r_i)=row_ind(i);
			r_i++;
		}
	}
}

/* Evaluate node for splitting, based on the chi-squared distribution law */
float evaluateNodeChiSquared(float lfrac, const Ref<const VectorXi> &lclass, const Ref<const VectorXi> &cclass)
{
	unsigned int i, term2=0, term3=0, sum_cclass;
	float term1=0, term4;

	/* evaluate node, based on the chi-squared distribution law */
	if(lfrac==0 || lfrac==1)
	{
		return 0;
	}

	sum_cclass=cclass.sum();

	for(i=0; i<cclass.size(); i++)
	{
		if(cclass(i)>0)
		{
			term4=sum_cclass*lclass(i)/cclass(i);
			term1+=(float) term4*term4/cclass(i);
			term2+=term4;
			term3+=cclass(i);
		}
	}
	/* return evaluation */
	return (float) (term1/lfrac)-(2*term2)+(lfrac*term3);
}
}
/*-------------------------------------------------------------------------------------------------------------------*/