/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "node.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------- derfcnd namespace - Node methods ----------------------------------------*/
namespace derfcnd
{
/*---------------------------- Node consisting of continuous variables methods definition ---------------------------*/
/* Constructors */
NodeC::NodeC(void)
{}
NodeC::NodeC(unsigned int len) : var_ind_(len), split_coef_(len)
{}
NodeC::NodeC(const Ref<const VectorXi> &var_ind, const Ref<const VectorXf> &split_coef)
			: var_ind_(var_ind.size()), split_coef_(split_coef.size())
{
	var_ind_=var_ind;
	split_coef_=split_coef;
}
/*Destructor */
NodeC::~NodeC(void)
{}

/* Get methods */
VectorXi NodeC::getVarInd(void) const
{
	return var_ind_;
}
int NodeC::getVarInd(unsigned int ind) const
{
	return var_ind_(ind);
}
VectorXf NodeC::getSplitCoef(void) const
{
	return split_coef_;
}
float NodeC::getSplitCoef(unsigned int ind) const
{
	return split_coef_(ind);
}
unsigned int NodeC::getLength(void) const
{
	return var_ind_.size();
}

/* Set methods */
void NodeC::setVarInd(const Ref<const VectorXi> &val)
{
	var_ind_.resize(val.size());
	var_ind_=val;
}
void NodeC::setVarInd(int val, unsigned int ind)
{
	var_ind_(ind)=val;
}
void NodeC::setSplitCoef(const Ref<const VectorXf> &val)
{
	split_coef_.resize(val.size());
	split_coef_=val;
}
void NodeC::setSplitCoef(float val, unsigned int ind)
{
	split_coef_(ind)=val;
}

/* Resize methods */
void NodeC::resize(unsigned int len)
{
	var_ind_.resize(len);
	split_coef_.resize(len);
}
void NodeC::resize(const Ref<const VectorXi> &var_ind, const Ref<const VectorXf> &split_coef)
{
	var_ind_.resize(var_ind.size());
	var_ind_=var_ind;
	split_coef_.resize(split_coef.size());
	split_coef_=split_coef;
}

/* Read method */
void NodeC::read(ifstream &fin)
{
	unsigned int i, len;

	fin>>len;
	resize(len);
	for(i=0; i<len; i++)
	{
		fin>>var_ind_(i)>>split_coef_(i);
	}
}
void NodeC::readBin(ifstream &fin)
{
	unsigned int i, len;

	fin.read((char*) &len, sizeof(len));
	resize(len);
	for(i=0; i<len; i++)
	{
		fin.read((char*) &var_ind_(i), sizeof(var_ind_(i)));
		fin.read((char*) &split_coef_(i), sizeof(split_coef_(i)));
	}
}
/* Write method */
void NodeC::write(ofstream &fout) const
{
	unsigned int i, len=var_ind_.size();

	fout<<"c "<<len;
	for(i=0; i<len; i++)
	{
		fout<<" "<<var_ind_(i)<<" "<<split_coef_(i);
	}
	fout<<endl;
}
void NodeC::writeBin(ofstream &fout)
{
	unsigned int i, len=var_ind_.size();

	fout.write("c", 1);
	fout.write(reinterpret_cast<char*>(&len), sizeof(len));
	for(i=0; i<len; i++)
	{
		fout.write(reinterpret_cast<char*>(&var_ind_(i)), sizeof(var_ind_(i)));
		fout.write(reinterpret_cast<char*>(&split_coef_(i)), sizeof(split_coef_(i)));
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------- Node consisting of discrete variables methods definition ----------------------------*/
/* Constructors */
NodeD::NodeD(void)
{
	var_ind_=0;
	split_coef_=0;
}
NodeD::NodeD(int var_ind, int split_coef)
{
	var_ind_=var_ind;
	split_coef_=split_coef;
}
/*Destructor */
NodeD::~NodeD(void)
{}

/* Get methods */
int NodeD::getVarInd(void) const
{
	return var_ind_;
}
int NodeD::getSplitCoef(void) const
{
	return split_coef_;
}

/* Set methods */
void NodeD::setVarInd(int val)
{
	var_ind_=val;
}
void NodeD::setSplitCoef(int val)
{
	split_coef_=val;
}

/* Read method */
void NodeD::read(ifstream &fin)
{
	fin>>var_ind_>>split_coef_;
}
void NodeD::readBin(ifstream &fin)
{
	fin.read((char*) &var_ind_, sizeof(var_ind_));
	fin.read((char*) &split_coef_, sizeof(split_coef_));
}
/* Write method */
void NodeD::write(ofstream &fout) const
{
	fout<<"d "<<var_ind_<<" "<<split_coef_<<endl;
}
void NodeD::writeBin(ofstream &fout)
{
	fout.write("d", 1);
	fout.write(reinterpret_cast<char*>(&var_ind_), sizeof(var_ind_));
	fout.write(reinterpret_cast<char*>(&split_coef_), sizeof(split_coef_));
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------- Leaf methods definition ---------------------------------------------*/
/* Constructors */
Leaf::Leaf(void)
{}
Leaf::Leaf(unsigned int len) : instance_frac_(len)
{}
Leaf::Leaf(const Ref<const RowVectorXf> &instance_frac) : instance_frac_(instance_frac.size())
{
	instance_frac_=instance_frac;
}
/* Destructor */
Leaf::~Leaf(void)
{}

/* Get method */
RowVectorXf Leaf::get(void) const
{
	return instance_frac_;
}
float Leaf::get(unsigned int ind) const
{
	return instance_frac_(ind);
}
unsigned int Leaf::getLength(void) const
{
	return instance_frac_.size();
}

/* Set method */
void Leaf::set(const Ref<const RowVectorXf> &val)
{
	instance_frac_.resize(val.size());
	instance_frac_=val;
}
void Leaf::set(float val, unsigned int ind)
{
	instance_frac_(ind)=val;
}

/* Resize methods */
void Leaf::resize(unsigned int len)
{
	instance_frac_.resize(len);
}
void Leaf::resize(const Ref<const RowVectorXf> &instance_frac)
{
	instance_frac_.resize(instance_frac.size());
	instance_frac_=instance_frac;
}

/* Read method */
void Leaf::read(ifstream &fin, unsigned int len)
{
	resize(len);
	for(unsigned int i=0; i<len; i++)
	{
		fin>>instance_frac_(i);
	}
}
void Leaf::readBin(ifstream &fin, unsigned int len)
{
	resize(len);
	for(unsigned int i=0; i<len; i++)
	{
		fin.read((char*) &instance_frac_(i), sizeof(instance_frac_(i)));
	}
}
/* Write method */
void Leaf::write(ofstream &fout) const
{
	fout<<"l";
	for(unsigned int i=0; i<instance_frac_.size(); i++)
	{
		fout<<" "<<instance_frac_(i);
	}
	fout<<endl;
}
void Leaf::writeBin(ofstream &fout)
{
	fout.write("l", 1);
	for(unsigned int i=0; i<instance_frac_.size(); i++)
	{
		fout.write(reinterpret_cast<char*>(&instance_frac_(i)), sizeof(instance_frac_(i)));
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------- General Node methods definition -----------------------------------------*/
/* Constructors */
Node::Node(void)
{
	lnode_=NULL;
	rnode_=NULL;
	nodec_=NULL;
	noded_=NULL;
	leaf_=NULL;
}
/*Destructor */
Node::~Node(void)
{
	clear();
}

/* Clear method */
void Node::clear(void)
{
	if(nodec_!=NULL)
	{
		delete nodec_;
		nodec_=NULL;
	}
	if(noded_!=NULL)
	{
		delete noded_;
		noded_=NULL;
	}
	if(leaf_!=NULL)
	{
		delete leaf_;
		leaf_=NULL;
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/