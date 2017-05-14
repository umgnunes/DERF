#ifndef IMPURITY_H
#define IMPURITY_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <math.h>
#include <Eigen/Core>
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------- derfcnd namespace - Impurity definition -------------------------------------*/
namespace derfcnd
{
/*----------------------------------------------- Impurity definition -----------------------------------------------*/
class Impurity
{
protected:

	/* Impurity value */
	float impurity_;
	/* Drop in impurity value */
	float drop_impurity_;

public:

	/* Constructors */
	Impurity(void);
	/* Destructor */
	virtual ~Impurity(void);

	/* Calculates the value of impurity, based on Entropy impurity */
	float entropy(const Ref<const VectorXi> &cclass, unsigned int nx_class);
	/* Calculates the value of impurity, based on Gini impurity */
	float gini(const Ref<const VectorXi> &cclass, unsigned int nx_class);
	/* Calculates the value of impurity, based on Misclassification impurity */
	float misclassification(const Ref<const VectorXi> &cclass, unsigned int nx_class);
	/* Calculates the value of the drop in impurity */
	float drop(float limpurity, float rimpurity, float lcfrac);
	
	/* Get methods*/
	float get(void) const;
	float getDrop(void) const;
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* IMPURITY_H */