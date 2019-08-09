#define OUTPUT
#define OUTPUT2
//#define DETERMINIST
#include <mecacell/mecacell.h>
#include "core/GingiScenario.hpp"

int main(int argc, char *argv[]) {

    int AS = 8;
    int AI = 8;
    int DS = 4;
    int DI = 4;
    using scenario_t = GingiScenario;
    for (double apopStroma = 0.00000001; apopStroma<=0.001; apopStroma*=10) {
        for (double apopImmune = 0.00000001; apopImmune<=0.001; apopImmune*=10) {
            for (double divStroma = 0.0001; divStroma<=0.1; divStroma*=10) {
                for (double divImmune = 0.0001; divImmune<=0.1; divImmune*=10) {
                    scenario_t scenar;
		
                    scenar.init(apopStroma,apopImmune,divStroma,divImmune,AS,AI,DS,DI);
                    
                    DI--;

                    int i = 0;
                    while (!scenar.stop()) {
                        scenar.loop();
                        i++;
                    }
                }
                DS--;
                DI = 4;
            }
            AI--;
            DS = 4;
        }
        AS--;
        AI = 8;
    }

        

}
