/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "random_forest.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------ derfcnd namespace - Random Forest methods ------------------------------------*/
namespace derfcnd
{
/*----------------------------------------- Random Forest methods definition ----------------------------------------*/
/* Allocate space for n_trees_ trees in the forest */
void RandomForest::allocate(unsigned int n_trees)
{
	n_trees_=n_trees;
	if(trees_!=NULL)
	{
		delete [] trees_;
	}
	trees_=new RandomTree[n_trees_];
}

/* Constructors */
RandomForest::RandomForest(void)
{
	conf_lvl_=CONF_LVL;
	n_trees_=0;
	trees_=NULL;
}
RandomForest::RandomForest(float conf_lvl)
{
	conf_lvl_=conf_lvl;
	n_trees_=0;
	trees_=NULL;
}
RandomForest::RandomForest(unsigned int n_pop, unsigned int n_iter, float f, float c)
						: de_params_(n_pop, n_iter, f, c)
{
	conf_lvl_=CONF_LVL;
	n_trees_=0;
	trees_=NULL;
}
RandomForest::RandomForest(unsigned int n_pop, unsigned int n_iter, float f, float c, float conf_lvl)
						: de_params_(n_pop, n_iter, f, c)
{
	conf_lvl_=conf_lvl;
	n_trees_=0;
	trees_=NULL;
}
RandomForest::RandomForest(const DEParams &de_params) : de_params_(de_params)
{
	conf_lvl_=CONF_LVL;
	n_trees_=0;
	trees_=NULL;
}
RandomForest::RandomForest(const DEParams &de_params, float conf_lvl) : de_params_(de_params)
{
	conf_lvl_=conf_lvl;
	n_trees_=0;
	trees_=NULL;
}

/* Destructor */
RandomForest::~RandomForest(void)
{
	clear();
}

/* Clear method */
void RandomForest::clear(void)
{
	conf_lvl_=CONF_LVL;
	n_trees_=0;
	if(trees_!=NULL)
	{
		delete [] trees_;
	}
}

/* Get methods */
float RandomForest::getConfLvl(void) const
{
	return conf_lvl_;
}
unsigned int RandomForest::getNTrees(void) const
{
	return n_trees_;
}
unsigned int RandomForest::getNRow(void) const
{
	return set_struct_.nrow_;
}
unsigned int RandomForest::getNColC(void) const
{
	return set_struct_.ncol_c_;
}
unsigned int RandomForest::getNColD(void) const
{
	return set_struct_.ncol_d_;
}
unsigned int RandomForest::getNRandVar(void) const
{
	return set_struct_.n_rand_var_;
}
vector< vector<string> > RandomForest::getXDLabel(void) const
{
	return set_struct_.x_d_lbl_;
}
vector<string> RandomForest::getClassLabel(void) const
{
	return set_struct_.class_lbl_;
}
unsigned int RandomForest::getDEParamNPop(void) const
{
	return de_params_.getNPop();
}
unsigned int RandomForest::getDEParamNIter(void) const
{
	return de_params_.getNIter();
}
float RandomForest::getDEParamF(void) const
{
	return de_params_.getF();
}
float RandomForest::getDEParamC(void) const
{
	return de_params_.getC();
}

/* Set methods */
void RandomForest::setConfLvl(float conf_lvl)
{
	conf_lvl_=conf_lvl;
}
void RandomForest::setDEParamNPop(unsigned int n_pop)
{
	de_params_.setNPop(n_pop);
}
void RandomForest::setDEParamNIter(unsigned int n_iter)
{
	de_params_.setNIter(n_iter);
}
void RandomForest::setDEParamF(float f)
{
	de_params_.setF(f);
}
void RandomForest::setDEParamC(float c)
{
	de_params_.setC(c);
}
void RandomForest::setDEParams(unsigned int n_pop, unsigned int n_iter, float f, float c)
{
	de_params_.setParams(n_pop, n_iter, f, c);
}
void RandomForest::setDEParams(const DEParams &de_params)
{
	de_params_.setParams(de_params);
}

/* Generate random decision tree */
void RandomForest::generate(const SetData &set_data)
{
	unsigned int n_rand_var=-1;
	generate(set_data, N_TREES, n_rand_var);
}
void RandomForest::generate(const SetData &set_data, unsigned int n_trees)
{
	unsigned int n_rand_var=-1;
	generate(set_data, n_trees, n_rand_var);
}
void RandomForest::generate(const SetData &set_data, unsigned int n_trees, unsigned int n_rand_var)
{
	unsigned int i, nrow=set_data.x_class_.size(), ncol_c=set_data.x_c_.cols();
	VectorXi boot_sample(nrow);

	/* allocate space for the forest */
	allocate(n_trees);
	if((int) n_rand_var==-1)
	{
		n_rand_var=log2(ncol_c)+1;
	}
	set_struct_.resize(nrow, ncol_c, set_data.x_d_.cols(),
						 				 n_rand_var, set_data.x_d_lbl_, set_data.class_lbl_);

	/* generate random forest */
	for(i=0; i<n_trees_; i++)
	{
		/* bootstrap sample indexes */
		randomUniform(0, (int) nrow, boot_sample);
		/* generate random decision tree model */
		trees_[i].setConfLvl(conf_lvl_);
		trees_[i].setDEParams(de_params_);
		trees_[i].generate(set_data, boot_sample, n_rand_var);
	}
}
/* Test random decision tree */
void RandomForest::test(const Ref<const RowVectorXf> &x_c_row, const Ref<const RowVectorXi> &x_d_row,
												Ref<RowVectorXf> certainty)
{
	unsigned int i, nclass=set_struct_.class_lbl_.size();

	if(trees_!=NULL &&
		 x_c_row.size()==set_struct_.ncol_c_ && x_d_row.size()==set_struct_.ncol_d_ &&
		 certainty.size()==nclass)
	{
		RowVectorXf certainty_tree(nclass);

		certainty.setZero();
		for(i=0; i<n_trees_; i++)
		{
			trees_[i].test(x_c_row, x_d_row, certainty_tree);
			certainty+=certainty_tree;
		}
		certainty/=n_trees_;
	}
	else
	{
		certainty.fill(-1);
	}
}
void RandomForest::test(const SetData &set_data, Ref<MatrixXf> certainty)
{
	unsigned int i, nrow=set_data.x_class_.size(), nclass=set_struct_.class_lbl_.size();

	if(trees_!=NULL &&
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
void RandomForest::read(ifstream &fin, const SetStruct &set_struct, unsigned int n_trees)
{
	/* allocate space for the forest */
	allocate(n_trees);
	set_struct_.resize(set_struct);
	if(fin.peek()!='e')
	{
		for(unsigned int i=0; i<n_trees; i++)
		{
			trees_[i].read(fin, set_struct);
		}
	}
}
char RandomForest::read(const string &fpath, const string &fname)
{
	unsigned int n_trees;
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

	/* reading the number of trees of the forest */
	fin>>n_trees;
	/* reading the training set data information */
	set_struct.read(fin);
	/* reading from file */
	read(fin, set_struct, n_trees);

	/* closing file */
	fin.close();
	return FOPEN_SUCCESS;
}
void RandomForest::readBin(ifstream &fin, const SetStruct &set_struct, unsigned int n_trees)
{
	/* allocate space for the forest */
	allocate(n_trees);
	set_struct_.resize(set_struct);
	if(fin.peek()!='e')
	{
		for(unsigned int i=0; i<n_trees; i++)
		{
			trees_[i].readBin(fin, set_struct);
		}
	}
}
char RandomForest::readBin(const string &fpath, const string &fname)
{
	unsigned int n_trees;
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

	/* reading the number of trees of the forest */
	fin.read((char*) &n_trees, sizeof(n_trees));
	/* reading the training set data information */
	set_struct.readBin(fin);
	/* reading from file */
	readBin(fin, set_struct, n_trees);

	/* closing file */
	fin.close();
	return FOPEN_SUCCESS;
}
/* Write model methods */
void RandomForest::write(ofstream &fout) const
{
	if(trees_!=NULL)
	{
		for(unsigned int i=0; i<n_trees_; i++)
		{
			trees_[i].write(fout);
		}
	}
	else
	{
		fout<<"e"<<endl;
	}
}
char RandomForest::write(const string &fpath, const string &fname) const
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
	/* writing the number of trees in the forest */
	fout<<n_trees_<<endl;
	/* writing the training set data information */
	set_struct_.write(fout);
	/* writing to file */
	write(fout);

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
void RandomForest::writeBin(ofstream &fout)
{
	if(trees_!=NULL)
	{
		for(unsigned int i=0; i<n_trees_; i++)
		{
			trees_[i].writeBin(fout);
		}
	}
	else
	{
		fout.write("e", 1);
	}
}
char RandomForest::writeBin(const string &fpath, const string &fname)
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
	/* writing the number of trees in the forest */
	fout.write(reinterpret_cast<char*>(&n_trees_), sizeof(n_trees_));
	/* writing the training set data information */
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