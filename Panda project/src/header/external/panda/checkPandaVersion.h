
# ifndef CHECKPANDAVERSION_H
# define CHECKPANDAVERSION_H
# include "dtoolbase.h"
extern EXPCL_DTOOL_DTOOLBASE int panda_version_1_10;
// Hack to forcibly depend on the check
template<typename T>
class CheckPandaVersion {
public:
  int check_version() { return panda_version_1_10; }
};
template class CheckPandaVersion<void>;
# endif
