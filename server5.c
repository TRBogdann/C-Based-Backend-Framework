#include "lf-server//lf_app.h"

int main()
{
    LfApp app = lfNewApp(1024);
    lfStartApp(app,2020);
    return 0;
}