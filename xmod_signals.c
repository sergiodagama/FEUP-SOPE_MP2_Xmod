#include "xmod_signals.h"

/**
 * @brief Function to handle the interrupt signal in xmod program
 * 
 * @param signal the signal to be handled
 */
void handle_sigint(int signal){
    printf("Caught signal %d\n");
}
