/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include "set_struct.h"
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------ derfcnd namespace - Set Structure methods ------------------------------------*/
namespace derfcnd
{
/*------------------------------------------- Set Data methods definition -------------------------------------------*/
/* Constructors */
SetData::SetData(void)
{
	resize(0, 0, 0, 0);
}
SetData::SetData(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int nclass)
{
	resize(nrow, ncol_c, ncol_d, nclass);
}
SetData::SetData(const SetData &data)
{
	resize(data);
}
/* Destructor */
SetData::~SetData(void)
{
	clear();
}

/* Clear method */
void SetData::clear(void)
{
	resize(0, 0, 0, 0);
}

/* Resize methods */
void SetData::resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int nclass)
{
	x_c_.resize(nrow, ncol_c);
	x_d_.resize(nrow, ncol_d);
	x_class_.resize(nrow);
	x_d_lbl_.resize(ncol_d);
	class_lbl_.resize(nclass);
}
void SetData::resize(const SetData &data)
{
	x_c_=data.x_c_;
	x_d_=data.x_d_;
	x_class_=data.x_class_;
	x_d_lbl_=data.x_d_lbl_;
	class_lbl_=data.class_lbl_;
}

/* Read method */
char SetData::read(const string &fpath, const string &fname)
{
	unsigned int i, j, nrow, ncol_c, ncol_d, nclass, nx_d_lbl;
	string aux_str, fname_path=fpath;
	fname_path+=fname;
	/* opening file for reading */
	ifstream fin(fname_path.c_str(), ios::in);

	/* opening file failure */
	if(fin.fail())
	{
		return FOPEN_FAIL;
	}

	/* reading the file name in the first line */
	fin>>aux_str;
	if(aux_str!=fname)
	{
		/* closing file */
		fin.close();
		return FOPEN_NOT_ALLOWED;
	}

	/* reading the size of the data */
	fin>>nrow>>ncol_c>>ncol_d>>nclass;
	getline(fin, aux_str);
	/* resize the data structure */
	resize(nrow, ncol_c, ncol_d, nclass);
	/* reading the discrete features labels */
	for(i=0; i<ncol_d; i++)
	{
		fin>>nx_d_lbl;
		getline(fin, aux_str, ' ');
		x_d_lbl_[i].clear();
		for(j=0; j<nx_d_lbl-1; j++)
		{
			getline(fin, aux_str, ' ');
			x_d_lbl_[i].push_back(aux_str);
		}
		getline(fin, aux_str);
		x_d_lbl_[i].push_back(aux_str);
	}

	/* reading the class labels */
	for(i=0; i<nclass; i++)
	{
		fin>>class_lbl_[i];
	}
	/* reading the data */
	for(i=0; i<nrow; i++)
	{
		/* reading the continuous variables */
		for(j=0; j<ncol_c; j++)
		{
			fin>>x_c_(i, j);
		}
		/* reading the quantized variables */
		for(j=0; j<ncol_d; j++)
		{
			fin>>x_d_(i, j);
		}
		/* reading the respective class label */
		fin>>x_class_(i);
	}

	/* closing file */
	fin.close();
	return FOPEN_SUCCESS;
}
/* Write method */
char SetData::write(const string &fpath, const string &fname) const
{
	unsigned int i, j, nx_d_lbl;
	unsigned int nrow=x_class_.size(), ncol_c=x_c_.cols(), ncol_d=x_d_.cols(), nclass=class_lbl_.size();
	string fname_path=fpath;
	fname_path+=fname;
	/* opening file for writing */
	ofstream fout(fname_path.c_str(), ios::out|ios::trunc);

	/* opening file failure */
	if(fout.fail())
	{
		return FOPEN_FAIL;
	}

	/* writing the file name in the first line */
	fout<<fname<<endl;
	/* writing the size of the data */
	fout<<nrow<<" "<<ncol_c<<" "<<ncol_d<<" "<<nclass<<endl;

	/* writing the discrete features labels */
	for(i=0; i<ncol_d; i++)
	{
		nx_d_lbl=x_d_lbl_[i].size();
		fout<<nx_d_lbl;
		for(j=0; j<nx_d_lbl; j++)
		{
			fout<<" "<<x_d_lbl_[i][j];
		}
		fout<<endl;
	}
	/* writing the class labels */
	for(i=0; i<nclass; i++)
	{
		fout<<class_lbl_[i]<<endl;
	}
	/* writing the data */
	for(i=0; i<nrow; i++)
	{
		/* writing the continuous variables */
		for(j=0; j<ncol_c; j++)
		{
			fout<<x_c_(i, j)<<" ";
		}
		/* writing the quantized variables */
		for(j=0; j<ncol_d; j++)
		{
			fout<<x_d_(i, j)<<" ";
		}
		/* writing the respective class label */
		fout<<x_class_(i)<<endl;
	}

	/* closing file */
	fout.close();
	return FOPEN_SUCCESS;
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------- Set Structure methods definition ----------------------------------------*/
/* Constructors */
SetStruct::SetStruct(void)
{
	resize(0, 0, 0, 0, 0);
}
SetStruct::SetStruct(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d,
										 unsigned int n_rand_var, unsigned int nclass)
{
	resize(nrow, ncol_c, ncol_d, n_rand_var, nclass);
}
SetStruct::SetStruct(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
										 const vector< vector<string> > &x_d_lbl, const vector<string> &class_lbl)
{
	resize(nrow, ncol_c, ncol_d, n_rand_var, x_d_lbl, class_lbl);
}
SetStruct::SetStruct(const SetStruct &set_struct)
{
	resize(set_struct);
}
/* Destructor */
SetStruct::~SetStruct(void)
{
	clear();
}

/* Clear method */
void SetStruct::clear(void)
{
	resize(0, 0, 0, 0, 0);
}

/* Resize methods */
void SetStruct::resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d,
											 unsigned int n_rand_var, unsigned int nclass)
{
	nrow_=nrow;
	ncol_c_=ncol_c;
	ncol_d_=ncol_d;
	n_rand_var_=n_rand_var;
	x_d_lbl_.resize(ncol_d);
	class_lbl_.resize(nclass);
}
void SetStruct::resize(unsigned int nrow, unsigned int ncol_c, unsigned int ncol_d, unsigned int n_rand_var,
											 const vector< vector<string> > &x_d_lbl, const vector<string> &class_lbl)
{
	nrow_=nrow;
	ncol_c_=ncol_c;
	ncol_d_=ncol_d;
	n_rand_var_=n_rand_var;
	x_d_lbl_=x_d_lbl;
	class_lbl_=class_lbl;
}
void SetStruct::resize(const SetStruct &set_struct)
{
	nrow_=set_struct.nrow_;
	ncol_c_=set_struct.ncol_c_;
	ncol_d_=set_struct.ncol_d_;
	n_rand_var_=set_struct.n_rand_var_;
	x_d_lbl_=set_struct.x_d_lbl_;
	class_lbl_=set_struct.class_lbl_;
}

/* Read methods */
void SetStruct::read(ifstream &fin)
{
	unsigned int i, j, nclass, nx_d_lbl;
	string aux_str;

	/* reading the training set data information */
	fin>>nrow_>>ncol_c_>>ncol_d_>>nclass>>n_rand_var_;
	getline(fin, aux_str);
	x_d_lbl_.resize(ncol_d_);
	class_lbl_.resize(nclass);
	/* reading the discrete features labels */
	for(i=0; i<ncol_d_; i++)
	{
		fin>>nx_d_lbl;
		getline(fin, aux_str, ' ');
		x_d_lbl_[i].clear();
		for(j=0; j<nx_d_lbl-1; j++)
		{
			getline(fin, aux_str, ' ');
			x_d_lbl_[i].push_back(aux_str);
		}
		getline(fin, aux_str);
		x_d_lbl_[i].push_back(aux_str);
	}
	/* reading the class labels */
	for(i=0; i<nclass; i++)
	{
		fin>>class_lbl_[i];
	}
}
void SetStruct::readBin(ifstream &fin)
{
	unsigned int i, j, nclass, nx_d_lbl;
	string aux_str;

	/* reading the training set data information */
	fin.read((char*) &nrow_, sizeof(nrow_));
	fin.read((char*) &ncol_c_, sizeof(ncol_c_));
	fin.read((char*) &ncol_d_, sizeof(ncol_d_));
	fin.read((char*) &nclass, sizeof(nclass));
	fin.read((char*) &n_rand_var_, sizeof(n_rand_var_));
	x_d_lbl_.resize(ncol_d_);
	class_lbl_.resize(nclass);
	/* reading the discrete features labels */
	for(i=0; i<ncol_d_; i++)
	{
		fin.read((char*) &nx_d_lbl, sizeof(nx_d_lbl));
		x_d_lbl_[i].clear();
		for(j=0; j<nx_d_lbl; j++)
		{
			getline(fin, aux_str, '\0');
			x_d_lbl_[i].push_back(aux_str);
		}
	}
	/* reading the class labels */
	for(i=0; i<nclass; i++)
	{
		getline(fin, class_lbl_[i], '\0');
	}
}

/* Write methods */
void SetStruct::write(ofstream &fout) const
{
	unsigned int i, j, nclass=class_lbl_.size(), nx_d_lbl;

	/* writing the training set data information */
	fout<<nrow_<<" "<<ncol_c_<<" "<<ncol_d_<<" "<<nclass<<" "<<n_rand_var_<<endl;
	/* writing the discrete features labels */
	for(i=0; i<ncol_d_; i++)
	{
		nx_d_lbl=x_d_lbl_[i].size();
		fout<<nx_d_lbl;
		for(j=0; j<nx_d_lbl; j++)
		{
			fout<<" "<<x_d_lbl_[i][j];
		}
		fout<<endl;
	}
	/* writing the class labels */
	for(i=0; i<nclass; i++)
	{
		fout<<class_lbl_[i]<<endl;
	}
}
void SetStruct::writeBin(ofstream &fout)
{
	unsigned int i, j, nclass=class_lbl_.size(), nx_d_lbl;

	/* writing the training set data information */
	fout.write(reinterpret_cast<char*>(&nrow_), sizeof(nrow_));
	fout.write(reinterpret_cast<char*>(&ncol_c_), sizeof(ncol_c_));
	fout.write(reinterpret_cast<char*>(&ncol_d_), sizeof(ncol_d_));
	fout.write(reinterpret_cast<char*>(&nclass), sizeof(nclass));
	fout.write(reinterpret_cast<char*>(&n_rand_var_), sizeof(n_rand_var_));
	/* writing the discrete features labels */
	for(i=0; i<ncol_d_; i++)
	{
		nx_d_lbl=x_d_lbl_[i].size();
		fout.write(reinterpret_cast<char*>(&nx_d_lbl), sizeof(nx_d_lbl));
		for(j=0; j<nx_d_lbl; j++)
		{
			fout.write(x_d_lbl_[i][j].c_str(), x_d_lbl_[i][j].size()+1);
		}
	}
	/* writing the class labels */
	for(i=0; i<nclass; i++)
	{
		fout.write(class_lbl_[i].c_str(), class_lbl_[i].size()+1);
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/