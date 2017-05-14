/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "random_tree.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------- derfcnd namespace - Random Decision Tree methods --------------------------------*/
namespace derfcnd
{
/*------------------------------------- Random Decision Tree methods definition -------------------------------------*/
/* Clear method */
void RandomTree::clear(Node *node)
{
	if(node!=NULL)
	{
		/* clear left branch nodes */
		clear(node->lnode_);
		/* clear right branch nodes */
		clear(node->rnode_);
		/* clear node */
		node->clear();
		delete node;
	}
}

/* Find best splits */
float RandomTree::findBestContinuousSplit(const Ref<const MatrixXf> &x_c, const Ref<const VectorXi> &x_class,
																					const Ref<const VectorXi> &cclass, Ref<VectorXf> split_val)
{
	DENode node_eval(x_c.cols(), de_params_);
	node_eval.setParams(x_c, x_class, cclass);
	return -node_eval.solve(split_val);
}
float RandomTree::findBestDiscreteSplit(const Ref<const MatrixXi> &x_d, const Ref<const VectorXi> &x_class,
																				const Ref<const VectorXi> &cclass, int *split_coef, int *split_var)
{
	unsigned int i, j, nrow=x_class.size(), nclass=set_struct_.class_lbl_.size(), lcounter;
	float eval, best_eval, lfrac;
	VectorXi lclass(nclass), rclass(nclass);
	Impurity node_impurity, aux_impurity;

	node_impurity.entropy(cclass, nrow);

	for(i=0, *split_coef=0, *split_var=0, best_eval=1000; i<x_d.cols(); i++)
	{
		for(j=0; j<set_struct_.x_d_lbl_[i].size(); j++)
		{
			lcounter=instancesGoingLeft(x_d.col(i), x_class, j, lclass, rclass);
			lfrac=(float) lcounter/nrow;
			eval=-node_impurity.drop(aux_impurity.entropy(lclass, lcounter),
															 aux_impurity.entropy(rclass, nrow-lcounter), lfrac);
			if(eval<best_eval)
			{
				best_eval=eval;
				*split_coef=j;
				*split_var=i;
			}
		}
	}

	return -best_eval;
}

/* Generate random decision tree recursively */
void RandomTree::generate(Node *node, const SetData &set_data, const Ref<const VectorXi> &row_ind)
{
	char node_op='c';
	int split_coef, split_var;
	float best_continuous_eval=0, best_discrete_eval=0, lfrac, chi;
	unsigned int i, i_c, i_d;
	unsigned int nrow=row_ind.size(), nclass=set_struct_.class_lbl_.size(), lcounter;
	MatrixXf x_c;
	MatrixXi x_d;
	VectorXf split_val;
	VectorXi x_class(nrow);
	VectorXi col_ind(set_struct_.n_rand_var_), cclass(nclass), lclass(nclass), rclass(nclass);
	VectorXi col_ind_c(set_struct_.n_rand_var_), col_ind_d(set_struct_.n_rand_var_);

	/* random variables indexes */
	randomUniformUnique(0, (int) (set_struct_.ncol_c_+set_struct_.ncol_d_), col_ind);
	for(i=0, i_c=0, i_d=0; i<set_struct_.n_rand_var_; i++)
	{
		if(col_ind(i)<(int) set_struct_.ncol_c_)
		{
			col_ind_c(i_c)=col_ind(i);
			i_c++;
		}
		else
		{
			col_ind_d(i_d)=col_ind(i)-set_struct_.ncol_c_;
			i_d++;
		}
	}
	col_ind_c.conservativeResize(i_c);
	col_ind_d.conservativeResize(i_d);
	x_c.resize(nrow, i_c);
	x_d.resize(nrow, i_d);
	split_val.resize(i_c);
	slice(set_data.x_c_, row_ind, col_ind_c, x_c);
	slice(set_data.x_d_, row_ind, col_ind_d, x_d);
	slice(set_data.x_class_, row_ind, VectorXi::Zero(1), x_class);

	/* find best split */
	countClasses(x_class, cclass);
	/* find best continuous split */
	if(i_c>0)
	{
		best_continuous_eval=findBestContinuousSplit(x_c, x_class, cclass, split_val);
	}
	/* find best discrete split */
	if(i_d>0)
	{
		best_discrete_eval=findBestDiscreteSplit(x_d, x_class, cclass, &split_coef, &split_var);
	}

	/* evaluate if it is advantageous to split */
	/* best split provided by continuous variables */
	if(best_continuous_eval>best_discrete_eval || i_d==0)
	{
		lcounter=instancesGoingLeft(x_c, x_class, split_val, lclass, rclass);
	}
	/* best split provided by discrete variable */
	else
	{
		lcounter=instancesGoingLeft(x_d.col(split_var), x_class, split_coef, lclass, rclass);
		node_op='d';
	}
	lfrac=(float) lcounter/nrow;
	chi=evaluateNodeChiSquared(lfrac, lclass, cclass);

	/* it is advantageous to split */
	if(chi>=conf_lvl_)
	{
		VectorXi l_row_ind(lcounter), r_row_ind(nrow-lcounter);

		/* split provided by continuous variables */
		if(node_op=='c')
		{
			node->nodec_=new NodeC(col_ind_c, split_val);
			/* find the falling branch of each instance */
			splitNode(x_c, row_ind, split_val, l_row_ind, r_row_ind);
		}
		/* split provided by discrete variables */
		else
		{
			node->noded_=new NodeD(split_var, split_coef);
			/* find the falling branch of each instance */
			splitNode(x_d.col(split_var), row_ind, split_coef, l_row_ind, r_row_ind);
		}

		/* generate left branch recursively */
		node->lnode_=new Node;
		generate(node->lnode_, set_data, l_row_ind);
		/* generate right branch recursively */
		node->rnode_=new Node;
		generate(node->rnode_, set_data, r_row_ind);
	}
	/* it is not advantageous to split */
	else
	{
		unsigned int i;
		RowVectorXf instance_frac=RowVectorXf::Zero(nclass);

		/* class votes */
		for(i=0; i<nrow; i++)
		{
			instance_frac(x_class(i))++;
		}
		instance_frac/=nrow;

		/* create new leaf */
		node->leaf_=new Leaf(instance_frac);
	}
}

/* Test random decision tree */
void RandomTree::test(Node *node, const Ref<const RowVectorXf> &x_c_row, const Ref<const RowVectorXi> &x_d_row,
											Ref<RowVectorXf> certainty)
{
	/* continuous splitting node condition */
	if(node->nodec_!=NULL)
	{
		RowVectorXf x_c_nrand;
		x_c_nrand.resize(node->nodec_->getLength());
		slice(x_c_row, VectorXi::Zero(1), node->nodec_->getVarInd(), x_c_nrand);

		/* continuous condition falling to the left branch */
		if(x_c_nrand*node->nodec_->getSplitCoef()<SPLIT_COEF)
		{
			test(node->lnode_, x_c_row, x_d_row, certainty);
		}
		/* continuous condition falling to the right branch */
		else
		{
			test(node->rnode_, x_c_row, x_d_row, certainty);
		}
	}
	/* discrete splitting node condition */
	else if(node->noded_!=NULL)
	{
		/* discrete condition falling to the left branch */
		if(node->noded_->getSplitCoef()!=x_d_row(node->noded_->getVarInd()))
		{
			test(node->lnode_, x_c_row, x_d_row, certainty);
		}
		/* discrete condition falling to the right branch */
		else
		{
			test(node->rnode_, x_c_row, x_d_row, certainty);
		}
	}
	/* leaf node */
	else if(node->leaf_!=NULL)
	{
		certainty=node->leaf_->get();
	}
	else
	{
		certainty.fill(-1);
	}
}

/* Read model methods */
void RandomTree::read(Node *node, ifstream &fin)
{
	char op;
	
	fin>>op;
	/* node case */
	if(op=='c'||op=='d')
	{
		/* continuous node */
		if(op=='c')
		{
			node->nodec_=new NodeC;
			node->nodec_->read(fin);
		}
		/* discrete node */
		else
		{
			node->noded_=new NodeD;
			node->noded_->read(fin);
		}

		/* left node */
		node->lnode_=new Node;
		read(node->lnode_, fin);
		/* right node */
		node->rnode_=new Node;
		read(node->rnode_, fin);
	}
	/* leaf case */
	else if(op=='l')
	{
		node->leaf_=new Leaf;
		node->leaf_->read(fin, set_struct_.class_lbl_.size());
	}
}
void RandomTree::readBin(Node *node, ifstream &fin)
{
	char op;
	
	fin.read((char*) &op, sizeof(op));
	/* node case */
	if(op=='c'||op=='d')
	{
		/* continuous node */
		if(op=='c')
		{
			node->nodec_=new NodeC;
			node->nodec_->readBin(fin);
		}
		/* discrete node */
		else
		{
			node->noded_=new NodeD;
			node->noded_->readBin(fin);
		}

		/* left node */
		node->lnode_=new Node;
		readBin(node->lnode_, fin);
		/* right node */
		node->rnode_=new Node;
		readBin(node->rnode_, fin);
	}
	/* leaf case */
	else if(op=='l')
	{
		node->leaf_=new Leaf;
		node->leaf_->readBin(fin, set_struct_.class_lbl_.size());
	}
}

/* Write model methods */
void RandomTree::write(Node *node, ofstream &fout) const
{
	/* write node information */
	if(node!=NULL)
	{
		/* leaf data */
		if(node->leaf_!=NULL)
		{
			node->leaf_->write(fout);
		}
		else
		{
			/* continuous node data */
			if(node->nodec_!=NULL)
			{
				node->nodec_->write(fout);
			}
			/* discrete node data */
			else if(node->noded_!=NULL)
			{
				node->noded_->write(fout);
			}
			/* write left branch */
			write(node->lnode_, fout);
			/* write right branch */
			write(node->rnode_, fout);
		}
	}
}
void RandomTree::writeBin(Node *node, ofstream &fout)
{
	/* write node information */
	if(node!=NULL)
	{
		/* leaf data */
		if(node->leaf_!=NULL)
		{
			node->leaf_->writeBin(fout);
		}
		else
		{
			/* continuous node data */
			if(node->nodec_!=NULL)
			{
				node->nodec_->writeBin(fout);
			}
			/* discrete node data */
			else if(node->noded_!=NULL)
			{
				node->noded_->writeBin(fout);
			}
			/* write left branch */
			writeBin(node->lnode_, fout);
			/* write right branch */
			writeBin(node->rnode_, fout);
		}
	}
}

/* Constructors */
RandomTree::RandomTree(void)
{
	conf_lvl_=CONF_LVL;
	root_=NULL;
}
RandomTree::RandomTree(float conf_lvl)
{
	conf_lvl_=conf_lvl;
	root_=NULL;
}
RandomTree::RandomTree(unsigned int n_pop, unsigned int n_iter, float f, float c)
					: de_params_(n_pop, n_iter, f, c)
{
	conf_lvl_=CONF_LVL;
	root_=NULL;
}
RandomTree::RandomTree(unsigned int n_pop, unsigned int n_iter, float f, float c, float conf_lvl)
					: de_params_(n_pop, n_iter, f, c)
{
	conf_lvl_=conf_lvl;
	root_=NULL;
}
RandomTree::RandomTree(const DEParams &de_params) : de_params_(de_params)
{
	conf_lvl_=CONF_LVL;
	root_=NULL;
}
RandomTree::RandomTree(const DEParams &de_params, float conf_lvl) : de_params_(de_params)
{
	conf_lvl_=conf_lvl;
	root_=NULL;
}
/* Destructor */
RandomTree::~RandomTree(void)
{
	clear();
}

/* Clear method */
void RandomTree::clear(void)
{
	clear(root_);
}

/* Get methods */
float RandomTree::getConfLvl(void) const
{
	return conf_lvl_;
}
unsigned int RandomTree::getNRow(void) const
{
	return set_struct_.nrow_;
}
unsigned int RandomTree::getNColC(void) const
{
	return set_struct_.ncol_c_;
}
unsigned int RandomTree::getNColD(void) const
{
	return set_struct_.ncol_d_;
}
unsigned int RandomTree::getNRandVar(void) const
{
	return set_struct_.n_rand_var_;
}
vector< vector<string> > RandomTree::getXDLabel(void) const
{
	return set_struct_.x_d_lbl_;
}
vector<string> RandomTree::getClassLabel(void) const
{
	return set_struct_.class_lbl_;
}
unsigned int RandomTree::getDEParamNPop(void) const
{
	return de_params_.getNPop();
}
unsigned int RandomTree::getDEParamNIter(void) const
{
	return de_params_.getNIter();
}
float RandomTree::getDEParamF(void) const
{
	return de_params_.getF();
}
float RandomTree::getDEParamC(void) const
{
	return de_params_.getC();
}

/* Set methods */
void RandomTree::setConfLvl(float conf_lvl)
{
	conf_lvl_=conf_lvl;
}
void RandomTree::setDEParamNPop(unsigned int n_pop)
{
	de_params_.setNPop(n_pop);
}
void RandomTree::setDEParamNIter(unsigned int n_iter)
{
	de_params_.setNIter(n_iter);
}
void RandomTree::setDEParamF(float f)
{
	de_params_.setF(f);
}
void RandomTree::setDEParamC(float c)
{
	de_params_.setC(c);
}
void RandomTree::setDEParams(unsigned int n_pop, unsigned int n_iter, float f, float c)
{
	de_params_.setParams(n_pop, n_iter, f, c);
}
void RandomTree::setDEParams(const DEParams &de_params)
{
	de_params_.setParams(de_params);
}

/* Generate random decision tree */
void RandomTree::generate(const SetData &set_data, const Ref<const VectorXi> &row_ind, unsigned int n_rand_var)
{
	if(set_data.x_c_.cols()>0 || set_data.x_d_.cols()>0)
	{
		clear();
		set_struct_.resize(set_data.x_class_.size(), set_data.x_c_.cols(), set_data.x_d_.cols(),
											 n_rand_var, set_data.x_d_lbl_, set_data.class_lbl_);
		root_=new Node;
		generate(root_, set_data, row_ind);
	}
}

/* Test random decision tree */
void RandomTree::test(const Ref<const RowVectorXf> &x_c_row, const Ref<const RowVectorXi> &x_d_row,
											Ref<RowVectorXf> certainty)
{
	if(root_!=NULL &&
		 x_c_row.size()==set_struct_.ncol_c_ && x_d_row.size()==set_struct_.ncol_d_ &&
		 certainty.size()==(int) set_struct_.class_lbl_.size())
	{
		test(root_, x_c_row, x_d_row, certainty);
	}
	else
	{
		certainty.fill(-1);
	}
}
void RandomTree::test(const SetData &set_data, Ref<MatrixXf> certainty)
{
	unsigned int i, nrow=set_data.x_class_.size(), nclass=set_struct_.class_lbl_.size();

	if(root_!=NULL &&
		 set_data.x_c_.cols()==set_struct_.ncol_c_ && set_data.x_d_.cols()==set_struct_.ncol_d_ &&
		 certainty.rows()==nrow && certainty.cols()==nclass)
	{
		RowVectorXf certainty_row(nclass);

		for(i=0; i<nrow; i++)
		{
			test(set_data.x_c_.row(i), set_data.x_d_.row(i), certainty_row);
			certainty.row(i)=certainty_row;
		}
	}
	else
	{
		certainty.fill(-1);
	}
}

/* Read model methods */
void RandomTree::read(ifstream &fin, const SetStruct &set_struct)
{
	clear();
	set_struct_.resize(set_struct);
	if(fin.peek()!='e')
	{
		root_=new Node;
		read(root_, fin);
	}
}
char RandomTree::read(const string &fpath, const string &fname)
{
	SetStruct set_struct;
	string fname_aux, fname_path=fpath;
	fname_path+=fname;
	ifstream fin(fname_path.c_str(), ios::in);

	/* opening file failure */
	if(fin.fail())
	{
		return FOPEN_FAIL;
	}

	/* reading the file name in the first line */
	fin>>fname_aux;
	if(fname_aux!=fname)
	{
		/* closing file */
		fin.close();
		return FOPEN_NOT_ALLOWED;
	}

	/* reading the set structure information */
	set_struct.read(fin);
	/* reading from file */
	read(fin, set_struct);

	/* closing file */
	fin.close();
	return FOPEN_SUCCESS;
}
void RandomTree::readBin(ifstream &fin, const SetStruct &set_struct)
{
	clear();
	set_struct_.resize(set_struct);
	if(fin.peek()!='e')
	{
		root_=new Node;
		readBin(root_, fin);
	}
}
char RandomTree::readBin(const string &fpath, const string &fname)
{
	SetStruct set_struct;
	string fname_aux, fname_path=fpath;
	fname_path+=fname;
	ifstream fin(fname_path.c_str(), ios::in | ios::binary);

	/* opening file failure */
	if(fin.fail())
	{
		return FOPEN_FAIL;
	}

	/* reading the file name in the first line */
	getline(fin, fname_aux, '\0');
	if(fname_aux!=fname)
	{
		/* closing file */
		fin.close();
		return FOPEN_NOT_ALLOWED;
	}

	/* reading the set structure information */
	set_struct.readBin(fin);
	/* reading from file */
	readBin(fin, set_struct);

	/* closing file */
	fin.close();
	return FOPEN_SUCCESS;
}

/* Write model methods */
void RandomTree::write(ofstream &fout) const
{
	if(root_!=NULL)
	{
		write(root_, fout);
	}
	else
	{
		fout<<"e"<<endl;
	}
}
char RandomTree::write(const string &fpath, const string &fname) const
{
	string fname_path=fpath;
	fname_path+=fname;
	ofstream fout(fname_path.c_str(), ios::out);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	/* writing the file name in the first line */
	fout<<fname<<endl;
	/* writing the set structure information */
	set_struct_.write(fout);
	/* writing to file */
	write(fout);

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
void RandomTree::writeBin(ofstream &fout)
{
	if(root_!=NULL)
	{
		writeBin(root_, fout);
	}
	else
	{
		fout.write("e", 1);
	}
}
char RandomTree::writeBin(const string &fpath, const string &fname)
{
	string fname_path=fpath;
	fname_path+=fname;
	ofstream fout(fname_path.c_str(), ios::out | ios::binary);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	/* writing the file name in the first line */
	fout.write(fname.c_str(), fname.size()+1);
	/* writing the set structure information */
	set_struct_.writeBin(fout);
	/* writing to file */
	writeBin(fout);

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/