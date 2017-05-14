#ifndef BASIC_STATS_H
#define BASIC_STATS_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>
#include <Eigen/Core>
#include "files.h"
using std::setprecision;
using std::setw;
using std::string;
using std::vector;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------ derfcnd namespace - Basic stats definition -----------------------------------*/
namespace derfcnd
{
/* Auxiliar random distributions functions definition */
template <class T> T randomUniform(T min, T max)
{
	T rand_num;
	do
	{
		rand_num=((float) rand()/RAND_MAX)*(max-min)+min;
	} while(rand_num==max);
	return rand_num;
}
template <class T1, class T2> void randomUniform(T1 min, T1 max, MatrixBase<T2> &randmat)
{
	unsigned int i, j, nrow=randmat.rows(), ncol=randmat.cols();

	if(nrow!=0 && ncol!=0)
	{
		for(i=0; i<nrow; i++)
		{
			for(j=0; j<ncol; j++)
			{
				randmat(i, j)=randomUniform(min, max);
			}
		}
	}
}
template <class T1, class T2> void randomUniformUnique(T1 min, T1 max, MatrixBase<T2> &randmat)
{
	bool flag;
	unsigned int i, j, ii, jj, nrow=randmat.rows(), ncol=randmat.cols();

	if(nrow!=0 && ncol!=0)
	{
		for(i=0, flag=0; i<nrow; i++)
		{
			for(j=0; j<ncol; j++, flag=0)
			{
				randmat(i, j)=randomUniform(min, max);

				for(ii=0; ii<i && !flag; ii++)
				{
					for(jj=0; jj<=j && !flag; jj++)
					{
						if(randmat(i, j)==randmat(ii, jj))
						{
							j--;
							flag=1;
						}
					}
				}
				for(jj=0; jj<j && !flag; jj++)
				{
					if(randmat(i, j)==randmat(ii, jj))
					{
						j--;
						flag=1;
					}
				}
			}
		}
	}
}

/* Determine the number of digits of an integer number */
template <class T> unsigned int numDigits(T num)
{
  unsigned int digits=0, abs_num=abs(num);

  if(abs_num<10)
  {
  	return 1;
  }
  while(abs_num){
    abs_num/=10;
    digits++;
  }
  return digits;
}

/* Copy certain elements of a matrix/vector, whose indexes are given */
template <class T1, class T2> void slice(const MatrixBase<T1> &m, const Ref<const VectorXi> &row_ind,
															const Ref<const VectorXi> &col_ind, MatrixBase<T2> &new_m)
{
	unsigned int i, j, nrow_ind=row_ind.size(), ncol_ind=col_ind.size();

	if(nrow_ind!=0 && ncol_ind!=0)
	{
		for(i=0; i<nrow_ind; i++)
		{
			for(j=0; j<ncol_ind; j++)
			{
				new_m(i, j)=m(row_ind(i), col_ind(j));
			}
		}
	}
}

/* Generate confusion matrix */
void confusionMatrix(const Ref<const VectorXi> &ground_truth, const Ref<const MatrixXf> &certainty,
										 float thresh, Ref<MatrixXi> confmat);
/* Compute precision based on the confusion matrix */
void precision(const Ref<const MatrixXi> &confmat, Ref<VectorXf> prec);
/* Compute recall based on the confusion matrix */
void recall(const Ref<const MatrixXi> &confmat, Ref<VectorXf> rec);

/* Write results to file */
char writeResults(const string &fpath, const string &fname, const Ref<const MatrixXi> &confmat,
									const Ref<const VectorXf> &prec, const Ref<const VectorXf> &rec, const vector<string> &class_lbl,
									float train_time, float test_time, unsigned int size_model);
char writeResults(const string &fpath, const string &fname);
char writeResults(const string &fpath, const string &fname, char a,
									const Ref<const VectorXf> &prec, const Ref<const VectorXf> &rec, const vector<string> &class_lbl);
char writeResults(const string &fpath, const string &fname, float prec, float rec,
									float train_time, float test_time, unsigned int size_model);

/* Determine the size of a file */
unsigned int sizeFile(const string &fpath, const string &fname);
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* BASIC_STATS_H */