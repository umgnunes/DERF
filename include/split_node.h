#ifndef SPLIT_NODE_H
#define SPLIT_NODE_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <Eigen/Core>
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------ derfcnd namespace - Split Node definition ------------------------------------*/
namespace derfcnd
{
/*------------------------------------------ Constant variables definition ------------------------------------------*/
/* Splitting coeficient for continuous variables */
#define SPLIT_COEF 1
/*-------------------------------------------------------------------------------------------------------------------*/

/* Count the number of classes of the instances */
void countClasses(const Ref<const VectorXi> &x_class, Ref<VectorXi> cclass);
/* Determine the number of instances going left, based on continuous variables */
unsigned int instancesGoingLeft(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &x_class,
																const Ref<const VectorXf> &split_val, Ref<VectorXi> lclass, Ref<VectorXi> rclass);
/* Determine the number of instances going left, based on discrete variables */
unsigned int instancesGoingLeft(const Ref<const VectorXi> &x_d, const Ref<const VectorXi> &x_class,
																int split_coef, Ref<VectorXi> lclass, Ref<VectorXi> rclass);
/* Split the node, by continuous variables */
void splitNode(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &row_ind,
							 const Ref<const VectorXf> &split_val, Ref<VectorXi> l_row_ind, Ref<VectorXi> r_row_ind);
/* Split the node, by discrete variables */
void splitNode(const Ref<const VectorXi> &x_d, const Ref<const VectorXi> &row_ind,
							 int split_coef, Ref<VectorXi> l_row_ind, Ref<VectorXi> r_row_ind);
/* Evaluate node for splitting, based on the chi-squared distribution law */
float evaluateNodeChiSquared(float lfrac, const Ref<const VectorXi> &lclass, const Ref<const VectorXi> &cclass);
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* SPLIT_NODE_H */