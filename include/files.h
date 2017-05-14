#ifndef FILES_H
#define FILES_H

/*------------------------------------------------ Included libraries -----------------------------------------------*/
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;
using std::endl;
/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------- derfcnd namespace - Files definition --------------------------------------*/
namespace derfcnd
{
/*------------------------------------------ Constant variables definition ------------------------------------------*/
/* File openning success */
#define FOPEN_SUCCESS 0
/* File openning fail */
#define FOPEN_FAIL -1
/* File openning not allowed */
#define FOPEN_NOT_ALLOWED -2

#define FTEST "ftest"
#define FTRAIN "ftrain"
#define MODEL "m"
#define RESULT "r"
#define ARFF ".arff"
#define BIN ".bin"
#define TXT ".txt"
#define UNDERSCORE "_"
/*-------------------------------------------------------------------------------------------------------------------*/
}
/*-------------------------------------------------------------------------------------------------------------------*/

#endif /* FILES_H */