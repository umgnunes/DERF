#ifndef NODE_H
#define NODE_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <fstream>
#include <Eigen/Core>
using std::ifstream;
using std::ofstream;
using std::endl;
using namespace Eigen;
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------- derfcnd namespace - Node definition ---------------------------------------*/
namespace derfcnd
{
/*-------------------------------- Node consisting of continuous variables definition -------------------------------*/
class NodeC
{
private:

	VectorXi var_ind_;
	VectorXf split_coef_;

public:

	/* Constructors */
	NodeC(void);
	NodeC(unsigned int len);
	NodeC(const Ref<const VectorXi> &var_ind, const Ref<const VectorXf> &split_coef);
	/*Destructor */
	~NodeC(void);

	/* Get methods */
	VectorXi getVarInd(void) const;
	int getVarInd(unsigned int ind) const;
	VectorXf getSplitCoef(void) const;
	float getSplitCoef(unsigned int ind) const;
	unsigned int getLength(void) const;

	/* Set methods */
	void setVarInd(const Ref<const VectorXi> &val);
	void setVarInd(int val, unsigned int ind);
	void setSplitCoef(const Ref<const VectorXf> &val);
	void setSplitCoef(float val, unsigned int ind);

	/* Resize methods */
	void resize(unsigned int len);
	void resize(const Ref<const VectorXi> &var_ind, const Ref<const VectorXf> &split_coef);

	/* Read method */
	void read(ifstream &fin);
	void readBin(ifstream &fin);
	/* Write method */
	void write(ofstream &fout) const;
	void writeBin(ofstream &fout);
};
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------- Node consisting of discrete variables definition --------------------------------*/
class NodeD
{
private:

	int var_ind_;
	int split_coef_;

public:

	/* Constructors */
	NodeD(void);
	NodeD(int var_ind, int split_coef);
	/*Destructor */
	~NodeD(void);

	/* Get methods */
	int getVarInd(void) const;
	int getSplitCoef(void) const;

	/* Set methods */
	void setVarInd(int val);
	void setSplitCoef(int val);

	/* Read method */
	void read(ifstream &fin);
	void readBin(ifstream &fin);
	/* Write method */
	void write(ofstream &fout) const;
	void writeBin(ofstream &fout);
};
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------- Leaf definition -------------------------------------------------*/
class Leaf
{
private:

	RowVectorXf instance_frac_;

public:

	/* Constructors */
	Leaf(void);
	Leaf(unsigned int len);
	Leaf(const Ref<const RowVectorXf> &instance_frac);
	/* Destructor */
	~Leaf(void);

	/* Get method */
	RowVectorXf get(void) const;
	float get(unsigned int ind) const;
	unsigned int getLength(void) const;

	/* Set method */
	void set(const Ref<const RowVectorXf> &val);
	void set(float val, unsigned int ind);

	/* Resize methods */
	void resize(unsigned int len);
	void resize(const Ref<const RowVectorXf> &instance_frac);

	/* Read method */
	void read(ifstream &fin, unsigned int len);
	void readBin(ifstream &fin, unsigned int len);
	/* Write method */
	void write(ofstream &fout) const;
	void writeBin(ofstream &fout);
};
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------- General Node definition ---------------------------------------------*/
class Node
{
public:

	Node *lnode_;
	Node *rnode_;
	NodeC *nodec_;
	NodeD *noded_;
	Leaf *leaf_;

	/* Constructor */
	Node(void);
	/*Destructor */
	~Node(void);

	/* Clear method */
	void clear(void);
};
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* NODE_H */