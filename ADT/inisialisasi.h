#include "boolean.h"
#include "pengguna.h"

extern boolean isLogin;
extern boolean isClosed;

void init();
void prosesCmd(Word cmd);
void parseMultiCmd(Word w, Word *cmd, Word** param);

